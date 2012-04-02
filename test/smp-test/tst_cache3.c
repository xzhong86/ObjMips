#include <test/test-head.h>
#include <pmon.h>

static volatile int share_int0;
static volatile int share_int1;

static void fun_slave(void)
{
	int msg=0;
	smp_msg_send_other(share_int0);   //  1>
	if(share_int0) return;
	while(share_int0 == 0) ;
	if(share_int1) msg = 1;
	while(share_int1 == 0) ;
	smp_msg_send_other(msg); //  2>

	while(share_int1 == 1) ;
	if(share_int0) msg = 1;
	smp_msg_send_other(msg); //  3>
}
static int main_test(void)
{
	int ret = 0,msg;
	smp_msg_receive(&msg);   //  <1
	if(share_int0!=0 || msg!=0) 
		return -1;

	share_int0 = 1;
	asm("nop\n\tnop\n\tnop\n\tnop\n\tnop");
	share_int1 = 1;

	smp_msg_receive(&msg);   //  <2
	ret += msg;

	share_int0 = 0;
	asm("nop;");
	share_int1 = 0;

	smp_msg_receive(&msg);   //  <3
	ret += msg;
	PRINTF("cache3 test, ret:%d\n",ret);
	if(ret) return -1;

	return 0;
}

static void push_slave(void)
{
	smp_fun_push(0,fun_slave);
}

int test_cache3(void)
{
	int ret;

	share_int0 = 0;
	share_int1 = 0;
	smp_ipi_func(0xff,smp_msg_clear);
	smp_ipi_func(0x2,push_slave);

	ret = main_test();
	smp_fun_wait(0x2);
	return ret;
}

DEF_TEST(test_cache3, 10, TESTFM_2ONCE);
