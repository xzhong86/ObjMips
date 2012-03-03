
#include <sys/time.h>
#include <stdio.h>

#include <base.h>
#include <smp_io.h>
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
