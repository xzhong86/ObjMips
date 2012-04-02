#include <test/test-head.h>
#include <stdlib.h>
#include <linux/spinlock.h>

struct queue {
#define QUEUE_SIZE 64
	int buf[QUEUE_SIZE];
	int head,tail;
	arch_spinlock_t lock;
};
static struct queue gq;
static arch_spinlock_t splock;
static int all_loop;
static int all_cost;
static inline int put_in_queue(struct queue *q,int val)
{
	int ret = 0;
	arch_spin_lock(&q->lock);
	if((q->tail+1)%QUEUE_SIZE == q->head) {
		ret = -1;
		goto out;
	}
	q->buf[q->tail] = val;
	q->tail = (q->tail+1)%QUEUE_SIZE;
out:
	arch_spin_unlock(&q->lock);
	return ret;
}
static inline int get_from_queue(struct queue *q,int *valp)
{
	int ret = 0;
	arch_spin_lock(&q->lock);
	if(q->head == q->tail) {
		ret = -1;
		goto out;
	}
	*valp = q->buf[q->head];
	q->head = (q->head+1)%QUEUE_SIZE;
out:
	arch_spin_unlock(&q->lock);
	return ret;
}

#define COST_LOOP(loop) do { (loop)--; } while((int)(loop) >0)
/* produce time range */
#define P_MIN 10
#define P_MAX (P_MIN + 16)
/* consume time range */
#define C_MIN 23
#define C_MAX (C_MIN + 16)
static void slave_func(void)
{
	int cost,msg=0;
	int loop=0,sum=0;
next:
	while(get_from_queue(&gq,&cost) != 0) ;
	if(cost > 0) {
		COST_LOOP(cost);
		loop++;
		sum += cost;
		goto next;
	}

	smp_msg_send_other(1);
	smp_msg_receive(NULL);
	arch_spin_lock(&splock);
	msg += all_loop != loop;
	msg += all_cost != sum;
	arch_spin_unlock(&splock);
	smp_msg_send_other(msg);
	return ;
}

static int main_func(void)
{
	int loop = 10000,sum=0;
	int i,msg;

	for(i=0;i<loop;i++) {
		int p = rand()%(P_MAX-P_MIN) + P_MIN;
		COST_LOOP(p);
		int c = rand()%(C_MAX-C_MIN) + C_MIN;
		sum += c;
		while(put_in_queue(&gq,c) != 0) ;
	}
	while(put_in_queue(&gq,-1) != 0) ; // stop slave
	smp_msg_receive(&msg);
	arch_spin_lock(&splock);
	all_loop = loop;
	all_cost = sum;
	arch_spin_unlock(&splock);
	smp_msg_send_other(1);
	smp_msg_receive(&msg);
	return msg==0? 0: -1;
}

static void push_slave(void)
{
	smp_fun_push(0,slave_func);
}

int test_consume(void)
{
	smp_ipi_func(0xf,smp_msg_clear);
	smp_ipi_func(0x2,push_slave);
	return 0;
}

DEF_TEST(test_consume, 20, (TESTF_2BIG | TESTF_2CORE));
