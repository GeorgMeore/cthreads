#include <stdio.h>
#include "threads.h"

#define STACKSIZE 16384

void printer(void *argp)
{
	char *msg = argp;
	for (;;) {
		printf("%s\n", msg);
		yield();
	}
}

void threadmain(int argc, char **)
{
	printf("argc = %d\n", argc);
	threadcreate(printer, "foo", STACKSIZE);
	threadcreate(printer, "bar", STACKSIZE);
}
