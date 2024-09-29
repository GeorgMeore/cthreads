#include <stdio.h>
#include "threads.h"

#define STACKSIZE 4096

void printer(void *argp)
{
	char *msg = argp;
	int i;
	for (i = 0; i < 10; i++) {
		printf("%s\n", msg);
		yield();
	}
}

void threadmain(int, char **)
{
	threadcreate(printer, "foo", STACKSIZE);
	threadcreate(printer, "bar", STACKSIZE);
}
