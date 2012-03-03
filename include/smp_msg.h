
#ifndef SMP_MSG_H
#define SMP_MSG_H

#include "smp.h"
#ifndef CONFIG_SMP
# warning "Use smp_fun.h without CONFIG_SMP!"
#endif

/* 0 <= msg < 8 */
int smp_msg_send(int cpumask,int msg);
static inline int smp_msg_send_other(int msg)
{
	int cpumask;
	cpumask = ~(1<<smp_cpu_id());
	return smp_msg_send(cpumask,msg);
}

int smp_msg_init(void);

/* if get message, *msgp=msg return 0; else return -1 */
int smp_msg_get(int *msgp);
/* wait until here is a message */
int smp_msg_receive(int *msgp);
/* wait until here are n messages, *msgp save the last message */
int smp_msg_receive_n(int *msgp, int n);

void smp_msg_clear(void);


#endif
