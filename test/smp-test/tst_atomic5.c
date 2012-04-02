#include <test/test-head.h>
#include <linux/atomic.h>

#define __LL		"ll	"
#define __SC		"sc	"
#define __INS		"ins    "
#define __EXT		"ext    "
static inline void _set_bit(unsigned long bit, volatile unsigned long *m)
{
	unsigned long temp;

	do {
		__asm__ __volatile__(
			"	" __LL "%0, %1		# set_bit	\n"
			"	" __INS "%0, %3, %2, 1			\n"
			"	" __SC "%0, %1				\n"
			: "=&r" (temp), "+m" (*m)
			: "ir" (bit), "r" (~0));
	} while ((!temp));
}
static inline void set_bit(unsigned long bit, volatile unsigned long *m)
{
	if (bit == 0) _set_bit(0, m);
	else if (bit == 1) _set_bit(1, m);
	else if (bit == 2) _set_bit(2, m);
	else if (bit == 3) _set_bit(3, m);
}
static inline void _clear_bit(unsigned long bit, volatile unsigned long *m)
{
	unsigned long temp;

	do {
		__asm__ __volatile__(
			"	" __LL "%0, %1		# clear_bit	\n"
			"	" __INS "%0, $0, %2, 1			\n"
			"	" __SC "%0, %1				\n"
			: "=&r" (temp), "+m" (*m)
			: "ir" (bit));
	} while ((!temp));
}
static inline void clear_bit(unsigned long bit, volatile unsigned long *m)
{
	if (bit == 0) _clear_bit(0, m);
	else if (bit == 1) _clear_bit(1, m);
	else if (bit == 2) _clear_bit(2, m);
	else if (bit == 3) _clear_bit(3, m);
}
static inline int test_bit(unsigned long bit, volatile unsigned long *m)
{
	return (*m) & (1 << bit);
}

#include <cache.h>
static void do_nothing(void) { return; }
#define WAY_SIZE (4*1024)
#define L2C_SIZE (512*1024)
static void harass_cache(unsigned long addr)
{
	int seed;
	seed = pcpu_rand();
	if (seed % 2048 == 0)
		smp_ipi_func(0xff,do_nothing);
	if (seed % 32 == 0)
		cache_unroll(addr, Hit_Writeback_Inv_D);
}

#define T_TIMES (TIMES*LOOPS)
atomic_t start;
volatile unsigned long flagbits;
static void fun_slave(void)
{
	int i,cpu = smp_cpu_id();
	int hold = 0;
	
	smp_msg_send(0x1,1);
	pcpu_srand(cpu);
	atomic_add(1, &start);
	while (*(volatile int*)&start.counter != CPUS) ;
	
	for (i = 0; i < T_TIMES; ) {
		if (test_bit(cpu, &flagbits)) {
			clear_bit(cpu, &flagbits);
			hold = 0;
			i ++;
		} else {
			if (hold ++ > 10000) {
				printk("error %d:%d %lx\n",cpu,i,flagbits);
				hold = 0;
			}
		}
		harass_cache((unsigned long)&flagbits);
	}
	printk("finish %d\n",cpu);
	smp_msg_send(0x1,1);
}
static void push_slave(void)
{
	smp_fun_push(0,fun_slave);
}

static int fun_main(void)
{
	int i,j,msg,cpu = smp_cpu_id();
	smp_msg_receive_n(&msg,CPUS-1);
	
	pcpu_srand(cpu);
	atomic_add(1, &start);
	while (*(volatile int*)&start.counter != CPUS) ;
	
	for (i = 0; i < (T_TIMES+1)*(CPUS-1); ) {
		for (j = 1; j < CPUS; j++)
			if (!test_bit(j, &flagbits)) {
				set_bit(j, &flagbits);
				i ++;
			}
		harass_cache((unsigned long)&flagbits);
	}

	printk("finish %d\n",cpu);
	smp_msg_receive_n(&msg,CPUS-1);

	return 0;
}

int test_atomic5(void)
{
	int ret;
	int cpumask;

	cpumask = (1<<CPUS) - 1;
	start.counter = 0;
	flagbits = 0;
	smp_ipi_func(0xff,smp_msg_clear);
	smp_ipi_func(cpumask-1,push_slave);

	ret = fun_main();
	smp_fun_wait(cpumask-1);
	return ret;
}

DEF_TEST(test_atomic5, 10, TESTFM_MLOOP);
