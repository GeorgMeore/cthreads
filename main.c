#include <stdio.h>
#include "pt.h"

PTBEGIN(first)
	for (;;) {
		printf("first\n");
		YIELD();
	}
PTEND

PTBEGIN(second)
	for (;;) {
		printf("second\n");
		YIELD();
	}
PTEND

int main(void)
{
	printf("before scheduling\n");
	PTSCHED(PT(first), PT(first), PT(second));
	printf("after scheduling\n");
}
