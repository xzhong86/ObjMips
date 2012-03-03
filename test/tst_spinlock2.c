#include "test-head.h"
#include <string.h>
#include <stdlib.h>
#include <linux/spinlock.h>

struct lock_cnt {
	arch_spinlock_t lock;
#define COUNTER_NUM 4
	int counter[COUNTER_NUM];
};
static struct lock_cnt *lcnts;
static int lcnts_num;
static int lcnts_step[CPU_NR];
#define ADD_TIMES 2

static void print_error_lcnt(struct lock_cnt *lcnt)
{
	static arch_spinlock_t lock;
	int k;
	arch_spin_lock(&lock);
	printk("lcnts[%d] @ %p:\n\t", lcnt-lcnts, lcnt);
	for (k = 0; k < COUNTER_NUM; k++)
		printk("counter[%d]=%d ",k,lcnt->counter[k]);
	printk("\n");
	arch_spin_unlock(&lock);
}
static int add_loop(int step)
{
	int i,j,k;
	for (i = 0, j = 0; i < lcnts_num*ADD_TIMES; i++) {
		struct lock_cnt *lcnt = &lcnts[j];
		arch_spin_lock(&lcnt->lock);

		for (k = 1; k < COUNTER_NUM; k++)
			if (lcnt->counter[k] != lcnt->counter[0] * (k+1)) {
				print_error_lcnt(lcnt);
				lcnt->counter[k] = lcnt->counter[0] * (k+1);
			}

		for (k = 0; k < COUNTER_NUM; k++)
			lcnt->counter[k] += k+1;
		arch_spin_unlock(&lcnt->lock);
		
		j = (j + step) % lcnts_num;
	}
	return 0;
}
static int check_result(void)
{
	int i,k,err = 0;
	for (i = 0; i < lcnts_num; i++) {
		struct lock_cnt *lcnt = &lcnts[i];
		if (lcnt->counter[0] != ADD_TIMES*CPUS) {
			err ++;
			print_error_lcnt(lcnt);
			continue;
		}
		for (k = 1; k < COUNTER_NUM; k++)
			if (lcnt->counter[k] != lcnt->counter[0] * (k+1)) {
				err ++;
				print_error_lcnt(lcnt);
				break;
			}
	}
	return err;
}
static void fun_slave(void)
{
	int cpu = smp_cpu_id();
	int msg;
	smp_msg_send(1, 1);
	smp_msg_receive(&msg);
	
	add_loop(lcnts_step[cpu]);
	smp_msg_send(1, 1);
}
static int main_test(void)
{
	int msg,ret = 0;
	smp_msg_receive_n(&msg, CPUS - 1);
	smp_msg_send(0xff,1);
	smp_msg_receive(&msg);
	
	add_loop(lcnts_step[0]);
	smp_msg_receive_n(&msg, CPUS - 1);
	
	ret = check_result();
	
	return ret;
}

static void push_slave(void)
{
	smp_fun_push(0,fun_slave);
}

static void fast_memset(void *dst, unsigned int val, int size)
{
	unsigned int *data = dst;
	int i;

	while (size > 256) {
		asm ("pref 0,%0"::"m"(data[128/4]));
		for (i = 0; i < 128; i += 4, data ++)
			*(int*)data = val;
		size -= 128;
	}
	for (i = 0; i < size; i += 4, data ++)
		*(int*)data = val;
}
int test_spinlock2(void)
{
	int ret, size;
	int slavemask;

	slavemask = (1<<CPUS) - 1 - 1;
	size = 1024*1024;	/* (L2 cache 512KB) * 2 */
	lcnts = (typeof(lcnts))malloc(size);
	lcnts_num = size / sizeof(*lcnts);
	printk("lock_cnt num: %d @ %p, lock_cnt size: %d\n",
	       lcnts_num, lcnts, sizeof(*lcnts));

	lcnts_step[0] = 13;
	lcnts_step[1] = 23;
	lcnts_step[2] = 43;
	lcnts_step[3] = 83;
	
	fast_memset(lcnts, 0, size);
	
	smp_ipi_func(0xff,smp_msg_clear);
	smp_ipi_func(slavemask,push_slave);
	ret = main_test();
	smp_fun_wait(slavemask);
	
	free(lcnts);
	return ret;
}

DEF_TEST(test_spinlock2, 20, (TESTF_REPEAT | TESTF_2BIG | TESTF_MCORE));
