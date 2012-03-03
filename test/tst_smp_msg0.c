#include "test-head.h"

static void fun_slave(void)
{
	int msg;
	smp_msg_send_other(1);
	do {
		smp_msg_receive(&msg);
		smp_msg_send_other(msg);
	} while(msg);

}
static void push_slave(void)
{
	smp_fun_push(0,fun_slave);
}

static int main_test(void)
{
	int ret,msg,stp=0;
	smp_msg_receive(&msg);
	PRINTF("received msg from slave:%d\n",msg);
	stp++;
	if(msg != 1) goto failed;

	smp_msg_send(0x2,3);
	smp_msg_send(0x2,2);
	smp_msg_receive(&msg);
	stp++;
	if(msg != 3) goto failed;

	smp_msg_receive(&msg);
	stp++;
	if(msg != 2) goto failed;

	ret = smp_msg_get(&msg);
	stp++;
	if(ret == 0) goto failed;
	smp_msg_send(0x2,0);
	smp_msg_receive(&msg);
	stp++;
	if(msg != 0) goto failed;


	smp_msg_send(0x1,4);
	ret = smp_msg_get(&msg);
	stp++;
	if(msg != 4 || ret) goto failed;

	ret = smp_msg_get(&msg);
	stp++;
	if(ret == 0) goto failed;

	PRINTF("smp_msg test passed!\n");
	return 0;

failed:
	PRINTF("test failed at step%d\n",stp);
	return -1;
}

int test_smp_msg0(void)
{
	int ret;

	smp_ipi_func(0xff,smp_msg_clear);
	smp_ipi_func(0x2,push_slave);
	ret = main_test();
	smp_fun_wait(0x2);
	return ret;
}

DEF_TEST(test_smp_msg0, 10, TESTFM_2ONCE);
