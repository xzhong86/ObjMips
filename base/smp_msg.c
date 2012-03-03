
#include <string.h>
#include "smp.h"
#include "irq.h"
#include "pcpu.h"

static void push_message(char n);

#define BUILD_MSG_FUNC(n)			\
	static void msg_func_##n(void)		\
	{					\
		push_message(n);		\
	}
BUILD_MSG_FUNC(0)
BUILD_MSG_FUNC(1)
BUILD_MSG_FUNC(2)
BUILD_MSG_FUNC(3)
BUILD_MSG_FUNC(4)
BUILD_MSG_FUNC(5)
BUILD_MSG_FUNC(6)
BUILD_MSG_FUNC(7)

struct smp_msg {
	char buf[256];
	volatile short rd,wt;
};
#define PCPU_MSG() ( (struct smp_msg*)(PCPU_VAR()->smp_msg_st_p) )

static void push_message(char n)
{
	int flag;
	struct smp_msg *sm;

	sm = PCPU_MSG();
	local_irq_save(&flag);
	sm->buf[sm->wt] = n;
	sm->wt = (sm->wt+1) % 256;
	if(sm->wt == sm->rd) 
		sm->rd = (sm->rd+1) % 256;
	local_irq_restore(&flag);
}
static int pop_message(int wait)
{
	int n,flag;
	struct smp_msg *sm;
	sm = PCPU_MSG();
	do {
		if(sm->wt != sm->rd) {
			local_irq_save(&flag);
			n = sm->buf[sm->rd];
			sm->rd = (sm->rd+1) % 256;
			local_irq_restore(&flag);
			return n;
		} else {
			if (wait)
				cpu_wait();
		}
	} while(wait);
	return -1;
}

int smp_msg_init(void)
{
	struct per_cpu_var *pv;
	pv = PCPU_VAR();
	struct smp_msg *sm;
	sm = (struct smp_msg*)pcpu_malloc(sizeof(*sm));
	memset(sm,0,sizeof(struct smp_msg));
	pv->smp_msg_st_p = sm;
	return 0;
}

int smp_msg_send(int cpumask,int msg)
{
	void (*fun)(void);
	switch(msg) {
#define CASE(n) case n: fun = msg_func_##n; break
		CASE(0); CASE(1); CASE(2); CASE(3);
		CASE(4); CASE(5); CASE(6); CASE(7);
	default:
		return -1;
	}
	smp_ipi_func(cpumask,fun);
	return 0;
}

/* if get message, *msgp=msg return 0; else return -1 */
int smp_msg_get(int *msgp)
{
	int n = pop_message(0);
	if(n>=0) {
		if(msgp)
			*msgp = n;
		return 0;
	}
	return -1;
}
/* wait until here is a message */
int smp_msg_receive(int *msgp)
{
	int msg;
	while ((msg = pop_message(1)) < 0) ;
	if (msgp) *msgp = msg;
	return 0;
}
/* wait until here are n messages, *msgp save the last message */
int smp_msg_receive_n(int *msgp, int n)
{
	int msg = 0;
	while (n-- > 0)
		while ((msg = pop_message(1)) < 0) ;
	if (msgp) *msgp = msg;
	return 0;
}

void smp_msg_clear(void)
{
	int flag;
	struct smp_msg *sm;

	sm = PCPU_MSG();
	local_irq_save(&flag);
	sm->wt = 0;
	sm->rd = 0;
	local_irq_restore(&flag);
}
