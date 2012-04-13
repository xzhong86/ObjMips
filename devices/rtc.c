
typedef unsigned long u32;

unsigned int get_pclk_fre(void)
{
	return 0;
}

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
