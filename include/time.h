
#ifndef TIME_H_my
#define TIME_H_my

extern unsigned int rtc_get_sec(void);
extern unsigned int get_cpu_fre(void);
extern void print_cpu_fre(void);

extern unsigned int ost_get_counter(void);
extern unsigned long long ost_get_cyc(void);
extern unsigned int ost_get_cyc_per_sec(void);

extern unsigned int ost_get_second(void);
extern unsigned int ost_get_nsecond(void);
extern unsigned int ost_cnt_to_nsec(unsigned int cnt);
extern void ost_wait_usec(unsigned int usec);


static inline unsigned int rtc_read_second(void)
{
	volatile unsigned int *rtcsr = (unsigned int*)0xB0003004;
	return *rtcsr;
}
static inline unsigned int ost_read_counter(void)
{
	volatile unsigned int *ost_cnt = (unsigned int*)0xB00020e8;
	return *ost_cnt;
}

#endif
