#include <ktime.h>

unsigned long jiffies;
void ktimer_tick(void)
{
	jiffies ++;
}

unsigned long long __attribute__((weak)) sched_clock(void)
{
	return jiffies * (1000000000UL / HZ);
}

ktime_t *ktime_gettime(ktime_t *kt)
{
	static ktime_t mykt;

	kt = kt? kt: &mykt;
	kt->sec  = jiffies / HZ;
	kt->nsec = (jiffies % HZ) * (1000000000UL / HZ);
	return kt;
}
