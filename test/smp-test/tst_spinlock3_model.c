/* This is a super version of tst_spinlock2_model.c */

#include <test/test-head.h>
#include <string.h>
#include <stdlib.h>
#include <linux/spinlock.h>
#include <cache.h>
#include <mem.h>
#include <mips.h>

struct lock_cnt {
	arch_spinlock_t lock;
	int counter[COUNTER_NUM];
};
static struct lock_cnt *lcnts;
static int lcnts_step[CPU_MAX];
#ifndef ALWAYS_CACHE
static int attr[COUNTER_NUM] = CNT_ATTR;
#endif
static int set_start,set_end;

#define get_set(addr) (((addr) % CFG_DCACHEWAY_SIZE) / CFG_DCACHELINE_SIZE)
#define is_in_sets(p) _is_in_sets((p), sizeof(*(p)))
static inline int _is_in_sets(void *d,int len)
{
	unsigned long addr = (unsigned)d;
	if (get_set(addr) < set_end && get_set(addr+len) >= set_start)
		return 1;
	return 0;
}

#define USR_BASE  0x10000000
#define TO_USR(addr) ((typeof(addr))(K0_TO_PHYS(addr) + USR_BASE))

#ifdef ALWAYS_CACHE
#define r_cnt(lcnt,cnt)     ((lcnt)->counter[cnt])
#define w_cnt(lcnt,cnt,val)  ((lcnt)->counter[cnt] = (val))
#define a_cnt(lcnt,cnt,add)  ((lcnt)->counter[cnt] += (add))
#else
static inline int r_cnt(struct lock_cnt *lcnt, int cnt)
{
	int *p = &lcnt->counter[cnt];

	if (attr[cnt] == ATTR_UNCAC) 
		p = (int*)K0_TO_K1(p);
	else if (attr[cnt] == ATTR_USR) 
		p = (int*)TO_USR(p);
	return *p;
}
static inline void w_cnt(struct lock_cnt *lcnt, int cnt, int val)
{
	int *p = &lcnt->counter[cnt];

	if (attr[cnt] == ATTR_UNCAC) 
		p = (int*)K0_TO_K1(p);
	else if (attr[cnt] == ATTR_USR) 
		p = (int*)TO_USR(p);

	*p = val;
}
static inline void a_cnt(struct lock_cnt *lcnt, int cnt, int add)
{
	int *p = &lcnt->counter[cnt];

	if (attr[cnt] == ATTR_UNCAC) 
		p = (int*)K0_TO_K1(p);
	else if (attr[cnt] == ATTR_USR) 
		p = (int*)TO_USR(p);

	*p += add;
}
#endif

static void print_error_lcnt(struct lock_cnt *lcnt, int cnt)
{
	static arch_spinlock_t lock;
	int k, cpu = smp_cpu_id();

	ncsim_error_addr(lcnt,(void*)cnt,0,0);
	arch_spin_lock(&lock);
	printk("CPU%d lcnts[%d] @ %p counter[%d]:\n\t", 
	       cpu, lcnt-lcnts, lcnt, cnt);
	for (k = 0; k < COUNTER_NUM; k++)
		printk("counter[%d]=%d ",k,r_cnt(lcnt,k));
	printk("\n");
	arch_spin_unlock(&lock);
}
static int add_loop(int step)
{
	int f,i,j,k;
	for (i = 0, j = 0; i < LCNTS_NUM*ADD_TIMES; i++) {
		struct lock_cnt *lcnt = &lcnts[j];

		if (!is_in_sets(lcnt))
			goto next;

		arch_spin_lock(&lcnt->lock);
		for (k = 1, f = 0; k < COUNTER_NUM; k++) {
			int cnt = r_cnt(lcnt,0);
			if (r_cnt(lcnt,k) != cnt * (k+1)) {
				if (!f) {
					print_error_lcnt(lcnt, k);
					f = 1;
				}
				w_cnt(lcnt,k,cnt * (k+1));
			}
		}

		for (k = 0; k < COUNTER_NUM; k++)
			a_cnt(lcnt, k, k+1);
		arch_spin_unlock(&lcnt->lock);
	next:
		j = (j + step) % LCNTS_NUM;
	}
	return 0;
}
static int check_result(void)
{
	int i,k,err = 0;
	for (i = 0; i < LCNTS_NUM; i++) {
		struct lock_cnt *lcnt = &lcnts[i];

		if (!is_in_sets(lcnt))
			continue;
		if (lcnt->counter[0] != ADD_TIMES*CPUS) {
			err ++;
			print_error_lcnt(lcnt, 0);
			continue;
		}
		for (k = 1; k < COUNTER_NUM; k++)
			if (lcnt->counter[k] != lcnt->counter[0] * (k+1)) {
				err ++;
				print_error_lcnt(lcnt, k);
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
	blast_dcache();
	smp_msg_send(1, 1);
}
static int main_test(void)
{
	int msg,ret = 0;
	smp_msg_receive_n(&msg, CPUS - 1);
	smp_msg_send(0xff,1);
	smp_msg_receive(&msg);
	
	add_loop(lcnts_step[0]);
	blast_dcache();
	smp_msg_receive_n(&msg, CPUS - 1);
	
	printk("Check result ...\n");
	ret = check_result();
	
	return ret;
}

static void push_slave(void)
{
	blast_dcache();
	smp_fun_push(0,fun_slave);
}

static void lcnts_init(void)
{
	int i;
	for (i = 0; i < LCNTS_NUM; i++) {
		struct lock_cnt *lcnt = &lcnts[i];
		if (is_in_sets(lcnt))
			memset(lcnt,0,sizeof(*lcnt));
	}
	blast_dcache();
}
#ifdef STEP_TEST
static int step_test(int *arr, int step, int num)
{
	int i,j;

	for (i = 0; i < num; i++)
		arr[i] = 0;
	for (i = 0, j = 0; i < num * ADD_TIMES; i++) {
		arr[j]++;
		j += step;
		if (j >= num)
			j -= num;
	}
	for (i = 0, j = 0; i < num; i++)
		j += arr[i] != ADD_TIMES;
	if (j)
		printk("step test error %d %d\n",step,num);
	return j;
}
#endif
int TEST_FUN(void)
{
	int ret, size, num;
	int slavemask;

	slavemask = (1<<CPUS) - 1 - 1;
	size = TEST_SIZE;
	num = size / sizeof(*lcnts);

	if (num != LCNTS_NUM) {
		printk("macro defined error! %d!=%d\n",num,LCNTS_NUM);
		return -1;
	}
	lcnts = (typeof(lcnts))malloc(size);
	//ret = map_mem_range((unsigned)lcnts, size, (unsigned)TO_USR(lcnts));
	//if (ret)
	//	goto free_m;
	printk("lock_cnt num: %d @ %p, lock_cnt size: %d\n",
	       num, lcnts, sizeof(*lcnts));

	lcnts_step[0] = 13;
	lcnts_step[1] = 23;
	lcnts_step[2] = 43;
	lcnts_step[3] = 83;
#ifdef STEP_TEST
	step_test((int*)lcnts, 13, LCNTS_NUM);
	step_test((int*)lcnts, 23, LCNTS_NUM);
	step_test((int*)lcnts, 43, LCNTS_NUM);
	step_test((int*)lcnts, 83, LCNTS_NUM);
#endif	
#if SET_START >= SET_END
# if defined ALWAYS_CACHE
	set_start = 2;
# else
	set_start = get_set((unsigned)attr);
# endif
	set_end = set_start+1;
	if (set_end >= CFG_DCACHEWAY_SIZE/CFG_DCACHELINE_SIZE) {
		set_start = 0;
		set_end = 1;
	}
#else
	set_start = SET_START;
	set_end = SET_END;
#endif
	printk("test sets: %d - %d\n",set_start,set_end);
	lcnts_init();
	
	smp_ipi_func(0xff,smp_msg_clear);
	smp_ipi_func(slavemask,push_slave);
	ret = main_test();
	smp_fun_wait(slavemask);
	
	//unmap_mem_range((unsigned)TO_USR(lcnts));
//free_m:
	free(lcnts);
	return ret;
}

DEF_TEST(TEST_FUN, 20, (TESTF_REPEAT | TESTF_BIG | TESTF_MCORE));
