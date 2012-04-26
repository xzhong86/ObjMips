
#include <sys/time.h>
#include <stdio.h>

#include <base.h>
#include <cache.h>
#include <ktime.h>

int sys_gettimeofday(struct timeval *tv,struct timezone *tz)
{
	ktime_t	kt;

	ktime_gettime(&kt);
	if(tv) {
		tv->tv_sec = kt.sec;
		tv->tv_usec = kt.nsec / 1000;
	}
	return 0;
}

//typedef unsigned int time_t;
time_t sys_time(time_t *t)
{
	ktime_t	kt;

	ktime_gettime(&kt);
	if(t) 
		*t = kt.sec;
	return kt.sec;
}

int sys_exit(int exit_code)
{
	printk("[SYS] Program exit, with code = %d.\n",exit_code);
	//cpu_idle();
	while(1) {
		cpu_wait();
	}
}

/* from mips sim interp.c
 * void get_mem_info(unsigned int *ptr)
 * in:  A0 = pointer to three word memory location 
 * out: [A0 + 0] = size 
 *      [A0 + 4] = instruction cache size 
 *      [A0 + 8] = data cache size 
 */
void sys_get_mem_info(unsigned int *ptr)
{
	ptr[0] = 0x800000; // 8MB
	ptr[1] = L1_icache.size;
	ptr[2] = L1_dcache.size;
}

/* for compiler like mipsel 4.1.2 */
extern unsigned long strtoul(char *, char **, int);
unsigned long __attribute__((weak))
__strtoul_internal(char *str, char **p, int x)
{
	return strtoul(str, p, x);
}
