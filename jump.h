typedef struct {
	unsigned long rbx;
	void          *rsp;
	void          *rbp;
	unsigned long r12;
	unsigned long r13;
	unsigned long r14;
	unsigned long r15;
	void          *rip;
} State;

#define SETIP(st, ip) ((st)->rip = ip)
#define SETSP(st, sp) ((st)->rsp = sp)

long save(State *s);
void jump(State *s, long v);
