#include <stdio.h>
#include "irq.h"
#include "time.h"
#include "clock.h"

typedef unsigned long u32;

static inline int write_rtc_reg(u32 *addr,u32 val)
{
	volatile u32 *rtccr  = (u32*)0xB0003000;
	volatile u32 *rtcwen = (u32*)0xB000303C;

	while(! (*rtccr & 0x80)) ;
	*rtcwen = 0xa55a;
	while(! (*rtcwen & 0x80000000)) ;
	*addr = val;
	return 0;
}
static long rtc_sec;
static unsigned int rtc_ost_cnt;
static void rtc_irq(int irq, void *d)
{
	volatile u32 *rtccr = (u32*)0xB0003000;
	unsigned int ccr = *rtccr;
	ccr &= ~0x40;
	*rtccr = ccr;
	//write_rtc_reg((u32*)rtccr,ccr);
	rtc_sec ++;
	rtc_ost_cnt = ost_read_counter();
}
int rtc_init(void)
{
	volatile u32 *rtccr = (u32*)0xB0003000;
	volatile u32 *rtcsr = (u32*)0xB0003004;
	unsigned int ctrl = *rtccr;
	write_rtc_reg((u32*)rtcsr,0);
	register_irqfun(tst_irq(1,0),rtc_irq,"RTC",NULL);
	ctrl = ctrl | 1 | 0x20;  // enable 1Hz interrupt
	ctrl &= ~0x40;           // clear 1Hz flag
	write_rtc_reg((u32*)rtccr,ctrl);
	//intc_unmask(1,0);

	return 0;
}

unsigned int rtc_get_sec(void)
{
#if 0
	static unsigned int sec_base = 0;
	unsigned int sec;
	volatile u32 *rtcsr = (u32*)0xB0003004;
	sec = *rtcsr;
	if(sec_base == 0) sec_base = sec;
	return sec - sec_base;
#else
	return rtc_sec;
#endif
}



#define OST_OVR_FRE  ( 1 )   // 1Hz ost interrupt frequency
#define OST_OVR_VAL  ( 512*1024*1024 )
static int ost_irq_times = 0;
static unsigned int ost_ovr_val = 0;
static unsigned int ost_base_second;
static void ost_irq(int irq, void *d)
{
	volatile u32 *tcu_tfcr = (u32*)0xB0002028;
	*tcu_tfcr = 0x8000;
	ost_irq_times ++;
}
static unsigned int cyc_per_sec = 0;
unsigned int real_cyc_per_sec(void)
{
	volatile u32 *ost_cnt = (u32*)0xB00020e8;
	unsigned int cnt0,cnt1,sec,cyc;
	volatile u32 *rtccr = (u32*)0xB0003000;
	volatile u32 *rtcsr = (u32*)0xB0003004;

	if(*rtccr & 1) {  // rtc enable
		sec = *rtcsr;
		while(sec == *rtcsr) ;
		cnt0 = *ost_cnt;
		sec = *rtcsr;
		while(sec == *rtcsr) ;
		cnt1 = *ost_cnt;
		if(cnt1 > cnt0)
			cyc = cnt1 - cnt0;
		else
			cyc = ost_ovr_val - cnt0 + cnt1;
	} else {
		/* depend on ost_csr in ost_init() */
		cyc = get_pclk_fre();
	}
	if(cyc != cyc_per_sec)
		cyc_per_sec = cyc;
	return cyc;
}
static int calc_cyc_per_sec(void)
{
	cyc_per_sec = real_cyc_per_sec();
	return 0;
}
int ost_init(void)
{
	volatile u32 *tcu_tesr= (u32*)0xB0002014;
	volatile u32 *tcu_tscr= (u32*)0xB000203c;
	volatile u32 *tcu_tmcr= (u32*)0xB0002038;
	volatile u32 *ost_csr = (u32*)0xB00020ec;
	volatile u32 *ost_dr  = (u32*)0xB00020e0;
	volatile u32 *ost_cnt = (u32*)0xB00020e8;
	
	*ost_csr = 0;
	*ost_cnt = 0;
#if OST_OVR_FRE
	ost_ovr_val = get_pclk_fre() / OST_OVR_FRE;
	cyc_per_sec = get_pclk_fre();
	*ost_dr  = ost_ovr_val;
#else
	ost_ovr_val = OST_OVR_VAL;
	cyc_per_sec = get_pclk_fre();
	*ost_dr  = OST_OVR_VAL;
#endif
	*ost_csr = 1;        // clk = pclk/1
	*tcu_tscr = 0x8000;  // clear stop
	*tcu_tmcr = 0x8000;  // clear mask
	*tcu_tesr = 0x8000;  // enable counter

	register_irqfun(tst_irq(0,0x08000000),ost_irq,"OST",NULL);
	ost_base_second = rtc_read_second();

	return 0;
}

unsigned int ost_get_counter(void)
{
	unsigned int cnt;
	volatile u32 *ost_cnt = (u32*)0xB00020e8;
	cnt = *ost_cnt;
	return cnt;
}
unsigned long long ost_get_cyc(void)
{
	unsigned long long cyc;
	cyc = ost_get_counter();
	cyc += ost_irq_times * ost_ovr_val;
	return cyc;
}
unsigned int ost_get_cyc_per_sec(void)
{
	if(!cyc_per_sec) calc_cyc_per_sec();
	return cyc_per_sec;
}


#if OST_OVR_FRE
#define NSEC  ( 1000000000UL )
unsigned int ost_get_second(void)
{
	unsigned int sec;
	sec = ost_irq_times / OST_OVR_FRE;
	return sec + ost_base_second;
}
unsigned int ost_get_nsecond(void)
{
	unsigned long long nsec = 0;
	unsigned long long cyc;
	cyc = ost_read_counter();
	cyc = (cyc * NSEC) / cyc_per_sec;
	if(OST_OVR_FRE != 1) {
		nsec = ost_irq_times % OST_OVR_FRE;
		nsec = (nsec * NSEC) / OST_OVR_FRE;
	}
	nsec += cyc;
	return (unsigned int)nsec;
}
unsigned int ost_cnt_to_nsec(unsigned int cnt)
{
	unsigned long long cyc = cnt;
	cyc = (cyc * NSEC) / cyc_per_sec;
	return cyc;
}
void ost_wait_usec(unsigned int usec)
{
	unsigned long long cyc;
	cyc = usec * cyc_per_sec / 1000000;
	cyc += ost_get_cyc();
	while(ost_get_cyc() < cyc) ;
}
#endif
