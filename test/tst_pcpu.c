#include "test-head.h"

static int self_test(void)
{
	void *p1,*p2,*p3;
#if 1
	PRINTF("var &p1=%p\n",&p1);
	p1 = pcpu_malloc(16);
	pcpu_free(p1);
	p2 = pcpu_malloc(8);
	pcpu_free(p2);
	PRINTF("[test 1] p1:%p  p2:%p\n",p1,p2);
	if(p1 != p2) return -1;

	p1 = pcpu_malloc(16);
	p2 = pcpu_malloc(8);
	pcpu_free(p1);
	p3 = pcpu_malloc(5);
	pcpu_free(p2);
	pcpu_free(p3);
	PRINTF("[test 2] p1:%p  p2:%p  p3:%p\n",p1,p2,p3);
	if(p1 != p3) return -1;
#endif
	p1 = pcpu_malloc(160);
	p2 = pcpu_malloc(8);
	pcpu_free(p1);
	pcpu_dump();
	p3 = pcpu_malloc(5);
	pcpu_dump();
	pcpu_free(p2);
	pcpu_dump();
	p1 = pcpu_malloc(10);
	pcpu_dump();

	pcpu_free(p3);
	pcpu_free(p1);
	pcpu_dump();
	PRINTF("[test 3] p1:%p  p2:%p  p3:%p\n",p1,p2,p3);
	
	return 0;
}
static volatile int slave_ret;

static void run_successed(void)
{
	slave_ret = 1;
}
static void run_failed(void)
{
	slave_ret = -1;
}
static void slave_fun(void)
{
	if(self_test())
		smp_ipi_func(0x1,run_failed);
	else
		smp_ipi_func(0x1,run_successed);
}
int test_pcpu(void)
{
	int ret = 0;
	ret = self_test();
	if(ret) return ret;

#ifdef CONFIG_SMP
	slave_ret = 0;
	smp_ipi_func(0x2,slave_fun);
	while(slave_ret == 0) {
		cpu_wait();
	}
	ret = slave_ret==1? 0: -1;
#endif
	return ret;
}

DEF_TEST(test_pcpu, 10, (TESTF_2CORE));
