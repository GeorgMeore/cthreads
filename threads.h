void threadmain(void *);
void threadcreate(void (*fn)(void *), void *arg, unsigned stacksize);
void yield(void);
