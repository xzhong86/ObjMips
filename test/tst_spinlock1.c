#include "test-head.h"
#include <string.h>
#include <linux/spinlock.h>

static struct lock_cnt {
	arch_spinlock_t lock;
	int counter;
} lcnt;

static int loop_fun(int *arr)
{
	struct lock_cnt *lc = &lcnt;
	int i,cnt;
	for(i=0; i<TIMES*LOOPS; i++) {
		arch_spin_lock(&lc->lock);
		cnt = ++lc->counter;
		arch_spin_unlock(&lc->lock);
		if(i%LOOPS == LOOPS-1)
			arr[i/LOOPS] = cnt;
	}
	return 0;
}
static void fun_slave(void)
{
	int msg;
	int *arr;
	arr = (int*)pcpu_malloc(TIMES*sizeof(int));
	smp_msg_send_other(1);
	smp_msg_receive(&msg);
	loop_fun(arr);
	smp_msg_send_other(1);
	
	pcpu_free(arr);
}
static void push_slave(void)
{
	smp_fun_push(0,fun_slave);
}

static int main_test(void)
{
	int msg,ret = 0;
	int *arr;
	arr = (int*)pcpu_malloc(TIMES*sizeof(int));
	smp_msg_receive(&msg);
	smp_msg_send(0xff,1);
	smp_msg_receive(&msg);
	loop_fun(arr);
	smp_msg_receive(&msg);
	PRINTF("spinlock add %d times, counter=%d\n"
	       ,2*TIMES*LOOPS,lcnt.counter);
	if(2*TIMES*LOOPS != lcnt.counter)
		ret = -1;
	
	pcpu_free(arr);
	return ret;
}

int test_spinlock1(void)
{
	int ret;

	memset(&lcnt, 0, sizeof(lcnt));
	smp_ipi_func(0xff,smp_msg_clear);
	smp_ipi_func(0x2,push_slave);
	ret = main_test();
	smp_fun_wait(0x2);
	return ret;
}

DEF_TEST(test_spinlock1, 20, TESTFM_2LOOP);
