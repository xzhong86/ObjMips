#include <test/test-head.h>
#include <pmon.h>

static volatile int share_int;

static void fun_slave(void)
{
	int msg;
	smp_msg_send_other(share_int);   //  1>
	if(share_int) return;
	msg = 0;
	while(share_int == 0) msg++;
	share_int = 0;
	smp_msg_send_other(msg!=0); //  2>
}
static int main_test(void)
{
	int msg;
	unsigned int lc,rc;
	pmon_prepare(PMON_EVENT_CYCLE);
	smp_msg_receive(&msg);   //  <1
	if(share_int!=0 || msg!=0) 
		return -1;

	pmon_start();
	share_int = 1;
	while(share_int == 1) ;
	pmon_stop();
	pmon_get_cnt32(lc,rc);

	smp_msg_receive(&msg);   //  <2

	printk("cache2 test, cyc:%d\n",rc);
	if(msg==0) return -1;

	return 0;
}

static void push_slave(void)
{
	smp_fun_push(0,fun_slave);
}

int test_cache2(void)
{
	int ret;

	share_int = 0;
	smp_ipi_func(0xff,smp_msg_clear);
	smp_ipi_func(0x2,push_slave);

	ret = main_test();
	smp_fun_wait(0x2);
	return ret;
}

DEF_TEST(test_cache2, 10, TESTFM_2ONCE);
