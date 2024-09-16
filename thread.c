#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/user.h>
#include "jump.h"
#include "threads.h"


#define MAINSTACKSIZE 4096

typedef enum {
	ThreadYield = 1,
	ThreadExit
} Status;

typedef struct Thread Thread;

struct Thread {
	State    cont;
	void     *stack;
	unsigned stacksize;
	void     (*fn)(void*);
	void     *arg;
	Thread   *next;
};

typedef struct {
	Thread *first;
	Thread *last;
} Queue;

static Thread *qpop(Queue *q)
{
	if (!q->first)
		return 0;
	Thread *t = q->first;
	if (q->first == q->last)
		q->first = q->last = 0;
	else
		q->first = t->next;
	return t;
}

static void qpush(Queue *q, Thread *t)
{
	if (!q->first) {
		q->first = q->last = t;
	} else {
		q->last->next = t;
		q->last = t;
	}
}

static Thread *current;
static Queue runqueue;
static State mainst;

void yield(void)
{
	if (save(&current->cont))
		return;
	jump(&mainst, ThreadYield);
}

static void trampoline(void)
{
	current->fn(current->arg);
	jump(&mainst, ThreadExit);
}

void threadcreate(void (*fn)(void *), void *arg, unsigned stacksize)
{
	Thread *t = malloc(sizeof(*t));
	memset(t, 0, sizeof(*t));
	t->fn = fn;
	t->arg = arg;
	t->stacksize = (stacksize/PAGE_SIZE + 3) * PAGE_SIZE;
	t->stack = mmap(0, t->stacksize, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, -1, 0);
	/* add guard pages */
	mmap(t->stack, PAGE_SIZE, PROT_NONE, MAP_PRIVATE|MAP_ANON|MAP_FIXED, -1, 0);
	mmap(t->stack+t->stacksize-PAGE_SIZE, PAGE_SIZE, PROT_NONE, MAP_PRIVATE|MAP_ANON|MAP_FIXED, -1, 0);
	SETIP(&t->cont, trampoline);
	SETSP(&t->cont, t->stack+t->stacksize-PAGE_SIZE);
	qpush(&runqueue, t);
}

static void threadfree(Thread *t)
{
	munmap(t->stack, t->stacksize);
	free(t);
}

static void mainrunner(void *argp)
{
	int argc = 0;
	char **argv = argp;
	while (argv[argc])
		argc += 1;
	threadmain(argc, argv);
}

int main(int, char **argv)
{
	threadcreate(mainrunner, argv, MAINSTACKSIZE);
	for (;;) {
		Status s = save(&mainst);
		if (s == ThreadExit)
			threadfree(current);
		else if (s == ThreadYield)
			qpush(&runqueue, current);
		current = qpop(&runqueue);
		if (!current)
			break;
		jump(&current->cont, 1);
	}
	return 0;
}
