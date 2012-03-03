#include "test-head.h"
#include <linux/atomic.h>

static atomic_t counter;

static void fun_slave(void)
{
	int i,msg;
	int *arr;
	arr = (int*)pcpu_malloc(TIMES*sizeof(int));
	smp_msg_send(0x1,1);
	smp_msg_receive(&msg);
	for(i=0; i<TIMES*LOOPS; i++) {
		if(i%LOOPS == LOOPS-1)
			arr[i/LOOPS] = atomic_add_return(1,&counter);
		else
			atomic_add(1,&counter);
	}
	smp_msg_send(0x1,1);
	
	pcpu_free(arr);
}
static void push_slave(void)
{
	smp_fun_push(0,fun_slave);
}

static int fun_main(void)
{
	int i,msg,ret = 0;
	int *arr;
	arr = (int*)pcpu_malloc(TIMES*sizeof(int));
	smp_msg_receive_n(&msg, CPUS - 1);
	smp_msg_send(0xff,1);
	smp_msg_receive(&msg);
	for(i=0; i<TIMES*LOOPS; i++) {
		if(i%LOOPS == LOOPS-1)
			arr[i/LOOPS] = atomic_add_return(1,&counter);
		else
			atomic_add(1,&counter);
	}
	smp_msg_receive_n(&msg, CPUS - 1);
	printk("atomic add %d times, counter=%d\n"
	       ,CPUS*TIMES*LOOPS,counter.counter);
	if(CPUS*TIMES*LOOPS != counter.counter)
		ret = -1;
	
	pcpu_free(arr);
	return ret;
}

int test_atomic1(void)
{
	int ret;
	int cpumask;

	cpumask = (1<<CPUS) - 1;
	counter.counter = 0;
	smp_ipi_func(0xff,smp_msg_clear);
	smp_ipi_func(cpumask-1,push_slave);

	ret = fun_main();
	smp_fun_wait(cpumask-1);
	return ret;
}

DEF_TEST(test_atomic1, 10, TESTFM_MLOOP);
