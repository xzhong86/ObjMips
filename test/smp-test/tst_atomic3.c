#include <test/test-head.h>
#include <linux/atomic.h>

static atomic_t counter;

#define nop_loop(loop) do {			\
		int i;				\
		for(i=0; i<(loop); i++) ;	\
	} while(0)

static int loop_func(int seed)
{
	int i,j,r;
	pcpu_srand(seed);
	for(i=0; i<TIMES; i++) {
		r = pcpu_rand();
		for(j=0; j<LOOPS; j++) {
			atomic_add(1,&counter);
			r ^= r >> 1;
			r ^= j;
			nop_loop(r%16);
		}
	}
	return 0;
}

static void fun_slave(void)
{
	int msg;
	smp_msg_send_other(1);
	smp_msg_receive(&msg);
	loop_func(2);
	smp_msg_send_other(1);
	
}
static void push_slave(void)
{
	smp_fun_push(0,fun_slave);
}

static int fun_main(void)
{
	int msg,ret = 0;
	smp_msg_receive(&msg);
	smp_msg_send(0xff,1);
	smp_msg_receive(&msg);
	loop_func(1);
	smp_msg_receive(&msg);
	PRINTF("atomic add %d times, counter=%d\n"
	       ,2*TIMES*LOOPS,counter.counter);
	if(2*TIMES*LOOPS != counter.counter)
		ret = -1;
	return ret;
}

int test_atomic3(void)
{
	int ret;

	counter.counter = 0;
	smp_ipi_func(0xff,smp_msg_clear);
	smp_ipi_func(0x2,push_slave);

	ret = fun_main();
	smp_fun_wait(0x2);
	return ret;
}

DEF_TEST(test_atomic3, 10, TESTFM_2LOOP);
