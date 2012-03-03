#include "test-head.h"

static void fun_slave(void)
{
	smp_msg_send(0x1,1);
}
static void push_slave(void)
{
	smp_fun_push(0,fun_slave);
}

int test_smp_msg1(void)
{
	int i,j,msg;
	int slavemask;

	slavemask = (1<<CPUS) - 1 - 1;
	smp_ipi_func(0xff,smp_msg_clear);

	for (i = 0; i < 100; i++) {
		smp_ipi_func(slavemask,push_slave);
		smp_msg_receive_n(&msg,CPUS-1);
		for(j = 0; j < i*1000; j++)
			asm volatile ("nop");
	}

	smp_fun_wait(slavemask);
	return 0;
}

DEF_TEST(test_smp_msg1, 10, (TESTF_REPEAT | TESTF_MCORE));
