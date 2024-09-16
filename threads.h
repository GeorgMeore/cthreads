void threadmain(int argc, char **argv);
void threadcreate(void (*fn)(void *), void *arg, unsigned stacksize);
void yield(void);
