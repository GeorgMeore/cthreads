#include <stdio.h>
#include "threads.h"

#define STACKSIZE 16384

void printer(void *msg)
{
	for (;;) {
		printf("%s\n", msg);
		yield();
	}
}

void threadmain(void *)
{
	threadcreate(printer, "foo", STACKSIZE);
	threadcreate(printer, "bar", STACKSIZE);
}
