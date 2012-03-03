#include "test-head.h"
#include <linux/spinlock.h>

struct mem_blk {
#define ST_BUSY 	0xff
#define ST_FILL 	0x1
#define ST_CHECK 	0x2
	int stat;
	arch_spinlock_t lock;
	int start,step;
	int seed;
	int size;
	void *addr;
};
#define NUM_BLKS 128
static struct mem_blk mblks[NUM_BLKS][2];
#define hash_val(seed,no) ((seed) ^ (((no)+5)*1103515245))


static int fill_blk(struct mem_blk *blk)
{
	int seed = pcpu_rand();
	int *buf = blk->addr;
	int i;

	for(i=blk->start; i<blk->size/4; i+=blk->step) {
		buf[i] = hash_val(seed,i);
	}

	arch_spin_lock(&blk->lock);
	blk->seed = seed;
	blk->stat = ST_FILL;
	arch_spin_unlock(&blk->lock);
	return 0;
}
static int check_blk(struct mem_blk *blk)
{
	int seed = blk->seed;
	int *buf = blk->addr;
	int cpu = smp_cpu_id();
	int i, ret = 0;
	
	for(i=blk->start; i<blk->size/4; i+=blk->step) {
		if(buf[i] != hash_val(seed,i)) {
			PRINTF("check err at %p cpu%d\n",buf+i,cpu);
			PRINTF("\tread %x, need %x\n",buf[i],hash_val(seed,i));
			ret ++;
		}
	}

	arch_spin_lock(&blk->lock);
	blk->stat = ST_CHECK;
	arch_spin_unlock(&blk->lock);
	return ret;
}
static int duel_blk(struct mem_blk *blk)
{
	int st, ret = 0;
	arch_spin_lock(&blk->lock);
	st = blk->stat;
	if(st != ST_BUSY) {
		blk->stat = ST_BUSY;
	}
	arch_spin_unlock(&blk->lock);
	
	if(st == ST_FILL)
		ret += check_blk(blk);
	else if (st == ST_CHECK)
		fill_blk(blk);
	return ret;
}
static int mem_test(void)
{
	int i, ret = 0;
	pcpu_srand(smp_cpu_id()+2);
	for(i=0; i<NUM_BLKS*2*1024; i++) {
		int n = pcpu_rand();
		n = n % (NUM_BLKS*2);
		if(mblks[n/2][n%2].size == 0)
			continue;
		ret += duel_blk(&mblks[n/2][n%2]);
	}
	return ret;
}
static void init_mem_blk(void)
{
	int i;
	for(i=0; i<NUM_BLKS; i++) {
		ARCH_SPIN_LOCK_INIT(mblks[i][0].lock);
		mblks[i][0].size = 0x100000; /* 1M */
		mblks[i][0].stat = ST_CHECK;
		mblks[i][0].addr = (void*)(0x80000000 + i * 0x100000);
		mblks[i][0].start = 0;
		mblks[i][0].step  = 2;

		ARCH_SPIN_LOCK_INIT(mblks[i][1].lock);
		mblks[i][1].size = 0x100000; /* 1M */
		mblks[i][1].stat = ST_CHECK;
		mblks[i][1].addr = (void*)(0x80000000 + i * 0x100000);
		mblks[i][1].start = 1;
		mblks[i][1].step  = 2;

		if(i <= 20) {
			mblks[i][0].size = 0;
			mblks[i][1].size = 0;
		}
	}
}

static void fun_slave(void)
{
	mem_test();
}
static void push_slave(void)
{
	smp_fun_push(0,fun_slave);
}

static int fun_main(void)
{
	int ret = mem_test();
	return ret;
}

int test_mem0(void)
{
	int ret;
	init_mem_blk();
#ifdef CONFIG_SMP
	smp_ipi_func(0xff,smp_msg_clear);
	smp_ipi_func(0x2,push_slave);
#endif
	ret = fun_main();
#ifdef CONFIG_SMP
	smp_fun_wait(0x2);
#endif
	return ret;
}

DEF_TEST(test_mem0, 20, (TESTF_REPEAT | TESTF_2BIG | TESTF_2CORE));
