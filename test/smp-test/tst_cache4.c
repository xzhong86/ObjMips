#include <test/test-head.h>
#include <linux/spinlock.h>
#include <stdlib.h>
#include <string.h>
#include <cache.h>
#include <mips.h>
#include <mem.h>

struct datablk {
	arch_spinlock_t lock;
	unsigned int stat;
	unsigned int value;
#define DATA_NUM 12
	unsigned int data[DATA_NUM];
};
#define DATA_BLK_NUM 6
struct datablk *datas;
static int work_step[CPU_MAX];
#define ADD_TIMES 64

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

//#define USR_BASE  0x10000000
#ifdef  USR_BASE
#define TO_USR(addr) ((typeof(addr))(K0_TO_PHYS(addr) + USR_BASE))
#else
#define TO_USR(addr) (addr)
#endif

static void print_err_data(struct datablk *d,unsigned *p,int i)
{
	static arch_spinlock_t lock;
	int k, cpu = smp_cpu_id();

	ncsim_error_addr(d,p,(void*)i,0);
	arch_spin_lock(&lock);
	printk("CPU%d datas[%d] @ %p %d data[%d]:\n\t", 
	       cpu, d-datas, d, d->value, i);
	for (k = 0; k < DATA_NUM; k++)
		printk("data[%d]=%d ",k,p[k]);
	printk("\n");
	arch_spin_unlock(&lock);
}
static void process_data(struct datablk *d)
{
	int i;
	if (!is_in_sets(d))
		return;
	arch_spin_lock(&d->lock);
	if (d->stat) {
		unsigned int *p = TO_USR(&d->data[0]);
		flush_dcache_range((unsigned)p, (unsigned)(p+DATA_NUM));
		for (i = 0; i < DATA_NUM; i++)
			if (p[i] != d->value) {
				print_err_data(d,p,i);
				break;
			}
		flush_dcache_range((unsigned)p, (unsigned)(p+DATA_NUM));
		d->stat = 0;
	} else {
		unsigned int *p = K0_TO_K1(&d->data[0]);
		d->value ++;
		for (i = 0; i < DATA_NUM; i++)
			p[i] = d->value;
		d->stat = 1;
	}
	arch_spin_unlock(&d->lock);
}
static void work_loop(int step)
{
	int i,j,n,cpu;
	cpu = smp_cpu_id();
	printk("working %d\n",cpu);
	n = ADD_TIMES*DATA_BLK_NUM;
	for(i = 0,j = 0; i < n; i++,j += step) {
		while (j >= DATA_BLK_NUM)
			j -= DATA_BLK_NUM;
		process_data(&datas[j]);
	}
	blast_dcache();
}
static int check_result(void)
{
	int i,j,e;
	for(i = 0,e = 0; i < DATA_BLK_NUM; i++) {
		if (!is_in_sets(&datas[i]))
			continue;
		for (j = 0; j < DATA_NUM; j++)
			e += datas[i].value != datas[i].data[j];
	}
	return e;
}

static void fun_slave(void)
{
	int msg;
	smp_msg_send(0x1,1);
	smp_msg_receive(&msg);
	work_loop(work_step[smp_cpu_id()]);
	smp_msg_send(0x1,1);
}
static void push_slave(void)
{
	blast_dcache();
	smp_fun_push(0,fun_slave);
}

static int fun_main(void)
{
	int msg,ret = 0;
	smp_msg_receive_n(&msg,CPUS-1);
	smp_msg_send(0xff,1);
	smp_msg_receive(&msg);
	work_loop(work_step[smp_cpu_id()]);
	smp_msg_receive_n(&msg,CPUS-1);
	
	ret = check_result();
	return ret;
}

static int init_datas(void)
{
	int i;
	for(i = 0; i < DATA_BLK_NUM; i++) {
		if (!is_in_sets(&datas[i]))
			continue;
		memset(&datas[i],0,sizeof(datas[0]));
		datas[i].value = 5;
	}
	blast_dcache();
	return 0;
}

int test_cache4(void)
{
	int ret,size;
	int slavemask;

	slavemask = (1<<CPUS) - 1 - 1;
	size = DATA_BLK_NUM*sizeof(datas[0]);
	datas = (typeof(datas))malloc(size);
	printk("datas %p %d\n",datas,size);
#ifdef  USR_BASE
	map_mem_range((unsigned)datas, size, (unsigned)TO_USR(datas), PG_NORMAL);
#endif

	set_start = 0;
	set_end   = 128;
	init_datas();

	work_step[0] = 1;
	work_step[1] = 1;
	work_step[2] = 1;
	work_step[3] = 1;

	smp_ipi_func(0xff,smp_msg_clear);
	smp_ipi_func(slavemask,push_slave);

	ret = fun_main();
	smp_fun_wait(slavemask);
#ifdef  USR_BASE
	unmap_mem_range((unsigned)TO_USR(datas));
#endif
	free(datas);
	return ret;
}

DEF_TEST(test_cache4, 10, TESTFM_MLOOP);
