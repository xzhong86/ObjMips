#include <stdio.h>
#include <mipsop.h>
#include <irq.h>
#include <time.h>
#include <clock.h>
#include <config.h>
#include <smp_io.h>


int pll_init(void);
int serial_init(void);
int intc_init(void);
int rtc_init(void);
int ost_init(void);
int pm_init(void);
int dma_init(void);

//#define DBG_INIT(fun,ret) (0)
#define DBG_INIT(fun,ret) PRINTF(#fun " %s\n",ret?"failed":"seuccess")

int drivers_init(void)
{
	int ret;
	irq_disable();
	ret = pll_init();
	if(ret) return ret;

	ret = serial_init();
	if(ret) return ret;

	ret = intc_init();
	DBG_INIT(intc_init,ret);
	if(ret) return ret;

#ifndef CONFIG_SIM
	ret = rtc_init();
	DBG_INIT(rtc_init,ret);
	if(ret) return ret;

	ret = ost_init();
	DBG_INIT(ost_init,ret);
	if(ret) return ret;

	ret = dma_init();
	DBG_INIT(dma_init,ret);
	if(ret) return ret;

	//ret = pm_init();
	//DBG_INIT(pm_init,ret);
	//if(ret) return ret;
#endif

	return ret;
}

extern unsigned int real_cyc_per_sec(void);
extern void pm_show_clkgr(void);
void hardware_init_fun(void)
{
#if !defined CONFIG_NCSIM
	int ret;
	ret = drivers_init();

	//ost_wait_usec(100*1000);
	PRINTF("        \n");
	PRINTF("drivers_init() %s.\n",
	       ret?"failed":"successed");

	unsigned int fre = get_cpu_fre();
	PRINTF("jz4760 @ %dMHz\n",fre/1000000);
	print_cpu_fre();
	//pm_show_clkgr();
	PRINTF("get ost cycs per second ...\n");
	fre = real_cyc_per_sec();
	PRINTF("  %u ost cycs per second.\n",fre);
#endif
}

