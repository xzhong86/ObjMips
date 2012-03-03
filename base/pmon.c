#include <stdio.h>

#include <smp_io.h>
#include <pmon.h>

#define _print_val(val) __print_val(val)
static void __print_val(unsigned long long);
int _pmon_report(int event)
{
	unsigned long long lc,rc;
	pmon_get_cnt64(lc,rc);
	
	if(event == PMON_EVENT_CACHE) {
		smp_printf("Pmon report, event CACHE.\n");
		smp_printf(" cache miss. i-cc: ");
		_print_val(lc);
		smp_printf(", d-cc: ");
		_print_val(rc);
		smp_printf("\n");
	}
	else if(event == PMON_EVENT_CYCLE) {
		smp_printf("Pmon report, event CYCLE.\n");
		smp_printf(" freeze cycle: ");
		_print_val(lc);
		smp_printf(", total cycle: ");
		_print_val(rc);
		unsigned long long rate = lc * 10000 / rc;
		smp_printf("  (%lld%%%%)\n",rate);
	}
	else if(event == PMON_EVENT_INST) {
		smp_printf("Pmon report, event INST.\n");
		smp_printf(" useless insn: ");
		_print_val(lc);
		smp_printf(", total insn: ");
		_print_val(rc);
		unsigned long long rate = (lc * 10000) / rc;
		smp_printf("  (%lld%%%%)\n",rate);
	}

	return 0;
}

/* print val like 12,345,678 */
static void __print_val(unsigned long long val)
{
	char buf[40],str[40];
	int i = 0,j = 0,n = 0;
	while(val) {
		buf[n++] = val % 10 + '0';
		val = val / 10;
		if((j++)%3 == 2 && val) buf[n++] = ',';
	}
	for(i=0;i<n;i++)
		str[i] = buf[n-i-1];
	
	str[i] = '\0';
	smp_printf("%s",str);
}

