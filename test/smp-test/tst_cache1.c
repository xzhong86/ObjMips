#include <test/test-head.h>
#include <cache.h>

static volatile int counter;
static volatile int counter2;

static void fun_slave(void)
{
	int i,j,msg;
	int *arr;
	arr = (int*)pcpu_malloc(TIMES*sizeof(int));
	smp_msg_send_other(1);
	smp_msg_receive(&msg);
	for(i=0; i<TIMES; i++) {
		for(j=0; j<LOOPS; j++) {
			counter++;
			counter2++;
		}
		arr[i] = counter;
	}
	smp_msg_send_other(1);
	pcpu_free(arr);
}
static void push_slave(void)
{
	smp_fun_push(0,fun_slave);
}

static int fun_main(void)
{
	int i,j,msg,ret = 0;
	int *arr;
	arr = (int*)pcpu_malloc(TIMES*sizeof(int));
	smp_msg_receive(&msg);
	smp_msg_send(0xff,1);  // send to all
	smp_msg_receive(&msg);
	for(i=0; i<TIMES; i++) {
		for(j=0; j<LOOPS; j++) {
			counter++;
			counter2++;
		}
		arr[i] = counter;
	}
	smp_msg_receive(&msg);
	printk("cache1 add %d times, counter=%d, counter2=%d\n"
	       ,2*TIMES*LOOPS,counter,counter2);
	if(2*TIMES*LOOPS == counter && counter == counter2)
		ret = -1;
	
	pcpu_free(arr);
	return ret;
}

int test_cache1(void)
{
	int ret;

	counter = 0;
	counter2 = 0;
	smp_ipi_func(0xff,smp_msg_clear);
	smp_ipi_func(0x2,push_slave);

	ret = fun_main();
	smp_fun_wait(0x2);
	return ret;
}

DEF_TEST(test_cache1, 10, TESTFM_2LOOP);
