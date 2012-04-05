
#include <sys/time.h>
#include <stdio.h>

#include <base.h>
#include <smp_io.h>
#include <cache.h>
#include "time.h"

int sys_gettimeofday(struct timeval *tv,struct timezone *tz)
{
	int ret = 0;
	if(tv) {
		tv->tv_sec = ost_get_second();
		tv->tv_usec = ost_get_nsecond() / 1000;
	}
	return ret;
}

typedef unsigned int time_t;
time_t sys_time(time_t *t)
{
	unsigned int sec = ost_get_second();
	if(t) 
		*t = sec;
	return sec;
}

int sys_exit(int exit_code)
{
	smp_printf("[SYS] Program exit, with code = %d.\n",exit_code);
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
