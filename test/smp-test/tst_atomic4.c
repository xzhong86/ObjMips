#include <test/test-head.h>
#include <linux/atomic.h>
#include <string.h>

static atomic_t counters[CPU_MAX];
static atomic_t start;

static void atomic_loop_sync(int cpu)
{
	volatile int tmp;
	int i;
	atomic_add(1, &start);
	while (*(volatile int*)&start.counter != CPUS) ;
	for(i=0; i<TIMES*LOOPS; i++) {
		atomic_add(1, &counters[cpu]);
		tmp = counters[i%CPU_MAX].counter;
	}
}
static void fun_slave(void)
{
	smp_msg_send(0x1,1);
	atomic_loop_sync(smp_cpu_id());
	smp_msg_send(0x1,1);
}
static void push_slave(void)
{
	smp_fun_push(0,fun_slave);
}

static int fun_main(void)
{
	int i,msg,ret = 0;
	smp_msg_receive_n(&msg,CPUS-1);
	atomic_loop_sync(smp_cpu_id());
	smp_msg_receive_n(&msg,CPUS-1);

	for (i = 0; i < CPUS; i++) {
		printk("atomic add %d times, counter[%d]=%d\n"
		       ,TIMES*LOOPS,i,counters[i].counter);
		if(TIMES*LOOPS != counters[i].counter)
			ret -= 1;
	}

	return ret;
}

int test_atomic4(void)
{
	int ret;
	int cpumask;

	cpumask = (1<<CPUS) - 1;
	start.counter = 0;
	memset(counters, 0, sizeof(counters));
	smp_ipi_func(0xff,smp_msg_clear);
	smp_ipi_func(cpumask-1,push_slave);

	ret = fun_main();
	smp_fun_wait(cpumask-1);
	return ret;
}

DEF_TEST(test_atomic4, 10, TESTFM_MLOOP);
