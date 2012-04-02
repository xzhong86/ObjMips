#include <test/test-head.h>

#include <time.h>
#include <pmon.h>
#include <smp_stat.h>

#ifdef CONFIG_SMP

volatile static int fun1_val;
static void fun1_cpu0(void)
{
	pmon_stop();
	fun1_val = 3;
}
static void fun1_cpu1(void)
{
	fun1_val = 2;
	smp_ipi_func(0x1,fun1_cpu0);
}
static int test_base_ipi(void)
{
	int val;
	unsigned int lc,rc;
	fun1_val = 0;
	pmon_prepare(PMON_EVENT_CYCLE);
	pmon_start();
	smp_ipi_func(0x2,fun1_cpu1);
	val = fun1_val;
	while(fun1_val != 3) {
		if(val != fun1_val) {
			val = fun1_val;
			//PRINTF("val: %d\n",val);
		}
	}
	pmon_get_cnt32(lc,rc);
	
	PRINTF("ipi message take %d cpu cyc\n",rc);
	return 0;
}


volatile static int fun2_val;
static void fun2_cpu0(void)
{
	fun2_val = 1;
	//PRINTF("fun2_cpu0\n");
}
static void fun2_cpu1(void)
{
	smp_ipi_func(0x1,fun2_cpu0);
	//PRINTF("fun2_cpu1\n");
}
static int test_wakeup(void)
{
	fun2_val = 0;
	smp_ipi_func(0x2,fun2_cpu1);
	cpu_wait();
	if(fun2_val == 0) {
		PRINTF("sleep failed\n");
		while(fun2_val == 0) ;
		return -1;
	}
	PRINTF("wake up successed\n");
	return 0;
}

#endif

int test_smp_ipi(void)
{
#ifdef CONFIG_SMP
	int cpu = smp_cpu_id();
	if(cpu != 0) return -1;
	
	test_base_ipi();
	test_wakeup();
#endif
	return 0;
}

DEF_TEST(test_smp_ipi, 10, TESTFM_2ONCE);
