#include <test/test-head.h>
#include <linux/spinlock.h>
#include <cache.h>
#include <pmon.h>

static void invalid_icache(void)
{
	int idx,n;
	idx = pcpu_rand();
	idx = (idx%CFG_ICACHE_LINES) * CFG_ICACHELINE_SIZE;
	idx = idx | 0x80000000;
	n = pcpu_rand();
	n = n % CFG_ICACHE_LINES;
	while(n>0) {
		cache_unroll(idx,Index_Invalidate_I);
		idx += CFG_ICACHELINE_SIZE;
		n --;
	}
}
static void invalid_dcache(void)
{
	int idx,n;
	idx = pcpu_rand();
	idx = (idx%CFG_DCACHE_LINES) * CFG_DCACHELINE_SIZE;
	idx = idx | 0x80000000;
	n = pcpu_rand();
	n = n % CFG_DCACHE_LINES;
	while(n>0) {
		cache_unroll(idx,Index_Writeback_Inv_D);
		idx += CFG_DCACHELINE_SIZE;
		n --;
	}
}
static void invalid_btb(void)
{
	pmon_btb_invalid();
}

static void random_events(void)
{
	int rand = pcpu_rand();
	int cpumask;
	cpumask = rand % 4;
	rand /= 4;
	
	if(1 && rand%7 == 0)
		smp_ipi_func(cpumask,invalid_icache);
	if(0 && rand%8 == 0)
		smp_ipi_func(cpumask,invalid_dcache);
	if(0 && rand%9 == 0)
		smp_ipi_func(cpumask,invalid_btb);
}

static struct lock_cnt {
	arch_spinlock_t lock;
	int counter1;
	int counter2;
	int counter3;
	int counter4;
} lcnt;

#undef  TIMES
#define TIMES CALC_TEST(100)

static void random_self(void)
{
	unsigned int addr = (unsigned int)&lcnt;
	int rand = pcpu_rand();
	if(rand%4 != 0)
		return;
	rand /= 4;
	addr += rand%5 * 4;
	cache_unroll(addr,Hit_Writeback_Inv_D);
}

static int loop_fun(int *arr)
{
	struct lock_cnt *lc = &lcnt;
	int i,cnt;
	for(i=0; i<TIMES*LOOPS; i++) {
		arch_spin_lock(&lc->lock);
		cnt = ++lc->counter1;
		lc->counter2 += 5;
		lc->counter3 += i%5;
		lc->counter4 += i%8;
		arch_spin_unlock(&lc->lock);
		random_events();
		//random_self();
		if(i%LOOPS == LOOPS-1)
			arr[i/LOOPS] = cnt;
	}
	return 0;
}
static void fun_slave(void)
{
	int msg;
	int *arr;
	pcpu_srand(2);
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
	pcpu_srand(1);
	lcnt.counter1 = 0;
	arr = (int*)pcpu_malloc(TIMES*sizeof(int));
	smp_msg_receive(&msg);
	smp_msg_send(0xff,1);
	smp_msg_receive(&msg);
	loop_fun(arr);
	smp_msg_receive(&msg);
	printk("spinlock add %d times, counter=%d\n"
	       ,2*TIMES*LOOPS,lcnt.counter1);
	if(2*TIMES*LOOPS != lcnt.counter1)
		ret = -1;
	
	pcpu_free(arr);
	return ret;
}

#include <linux/atomic.h>
static atomic_t counter;
#define DEF_FUN(N,CPU,TN) static void add##TN(void);	\
	static void add##N(void)			\
	{						\
		atomic_add(1,&counter);			\
		smp_ipi_func(CPU,add##TN);		\
	}
DEF_FUN(00,0x2,10)
DEF_FUN(01,0x2,11)
DEF_FUN(02,0x0,00)
DEF_FUN(10,0x1,01)
DEF_FUN(11,0x1,02)
static int ipi_add_test(void)
{
	int i;
	counter.counter = 0;
	for(i=0; i<2000; i++) {
		add00();
		if(0 && counter.counter != i*5+5) {
			printk("loop %d: counter=%d,fix it!\n"
			       ,i,counter.counter);
			counter.counter = i*5+5;
		}
	}
	printk("ipi_add_test add %d times, counter=%d\n"
	       ,5*2000,counter.counter);
	return 0;
}

static void empty_fun(void)
{
	return;
}
static int spin_ipi_test(void)
{
	int i;
	for(i=0; i<100; i++)
		smp_ipi_func_spec(0x2,empty_fun);
	printk("spin_ipi_test pass\n");
	return 0;
}

int test_mixed(void)
{
	int ret;
	ipi_add_test();
	spin_ipi_test();
	smp_ipi_func(0xff,smp_msg_clear);
	smp_ipi_func(0x2,push_slave);
	ret = main_test();
	smp_fun_wait(0x2);
	return ret;
}

DEF_TEST(test_mixed, 20, TESTFM_2LOOP);
