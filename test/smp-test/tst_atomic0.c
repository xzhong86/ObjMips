#include <test/test-head.h>
#include <linux/atomic.h>

static atomic_t counter;

static void fun_slave(void)
{
	int i,msg;
	smp_msg_send(0x1,1);
	smp_msg_receive(&msg);
	for(i=0; i<TIMES*LOOPS; i++) {
		atomic_sub(1,&counter);
	}
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
	smp_msg_send(0xff,1);
	smp_msg_receive(&msg);
	for(i=0; i<TIMES*LOOPS; i++) {
		atomic_sub(1,&counter);
	}
	smp_msg_receive_n(&msg,CPUS-1);
	printk("atomic sub %d times, counter=%d\n"
	       ,CPUS*TIMES*LOOPS,counter.counter);
	if(0 != counter.counter)
		ret = -1;
	
	return ret;
}

int test_atomic0(void)
{
	int ret;
	int cpumask;

	cpumask = (1<<CPUS) - 1;
	counter.counter = CPUS*TIMES*LOOPS;
	smp_ipi_func(0xff,smp_msg_clear);
	smp_ipi_func(cpumask-1,push_slave);

	ret = fun_main();
	smp_fun_wait(cpumask-1);
	return ret;
}

DEF_TEST(test_atomic0, 10, TESTFM_MLOOP);
