#include "test-head.h"
#include <string.h>
#include <linux/spinlock.h>

static struct lock_cnt {
	arch_spinlock_t lock;
	int counter;
} lcnt;

static void fun_slave(void)
{
	int i,msg;
	smp_msg_send_other(1);   //  1>
	smp_msg_receive(&msg);   // <2
	for(i=0,msg=0; i<100; i++) {
		msg += arch_spin_trylock(&lcnt.lock);
	}
	if(msg>7) msg = 7;
	if(msg) arch_spin_unlock(&lcnt.lock);
	smp_msg_send_other(msg); //  3>

	smp_msg_receive(&msg);   // <4
	msg = arch_spin_trylock(&lcnt.lock);
	smp_msg_send_other(msg); //  5>
	if(msg) arch_spin_unlock(&lcnt.lock);
}
static int main_test(void)
{
	int msg;
	smp_msg_receive(&msg);   //  <1
	arch_spin_lock(&lcnt.lock);
	smp_msg_send_other(1);   //   2>
	smp_msg_receive(&msg);   //  <3
	PRINTF("spinlock, locked test, %s\n",
	       msg==0?"successed":"failed");
	if(msg) return -1;

	arch_spin_unlock(&lcnt.lock);
	smp_msg_send_other(1);   //   4>
	smp_msg_receive(&msg);   //  <5
	PRINTF("spinlock, unlocked test, %s\n",
	       msg==1?"successed":"failed");
	if(msg != 1) return -1;
	
	return 0;
}

static void push_slave(void)
{
	smp_fun_push(0,fun_slave);
}

int test_spinlock0(void)
{
	int ret;

	memset(&lcnt, 0, sizeof(lcnt));
	smp_ipi_func(0xff,smp_msg_clear);
	smp_ipi_func(0x2,push_slave);
	ret = main_test();
	smp_fun_wait(0x2);
	return ret;
}

DEF_TEST(test_spinlock0, 20, TESTFM_2ONCE);
