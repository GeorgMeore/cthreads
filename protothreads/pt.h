typedef struct PThread PThread;
struct PThread {
	unsigned line;
	int      running;
	void     (*fn)(PThread *self);
};

#define PTBEGIN(name) void name(PThread *self) {\
	switch (self->line) {\
	case 0:

#define YIELD()\
		self->line = __LINE__;\
		return;\
	case __LINE__:

#define RESTART()\
	self->line = 0;\
	return;

#define PTEND\
	}\
	self->running = 0;\
	return;\
}

#define PT(fn) {0, 1, fn}

#define PTSCHED(...) ({\
	unsigned i, cont;\
	PThread threads[] = {__VA_ARGS__};\
	do {\
		cont = 0;\
		for (i = 0; i < sizeof(threads)/sizeof(threads[0]); i++) {\
			if (threads[i].running) {\
				threads[i].fn(&threads[i]);\
				cont = cont || threads[i].running;\
			}\
		}\
	} while (cont);\
})
