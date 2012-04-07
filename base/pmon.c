#include <stdio.h>

#include <base.h>
#include <pmon.h>

#define _print_val(val) __print_val(val)
static void __print_val(unsigned long long);
int _pmon_report(int event)
{
	unsigned long long lc,rc;
	pmon_get_cnt64(lc,rc);
	
	if(event == PMON_EVENT_CACHE) {
		printk("Pmon report, event CACHE.\n");
		printk(" cache miss. i-cc: ");
		_print_val(lc);
		printk(", d-cc: ");
		_print_val(rc);
		printk("\n");
	}
	else if(event == PMON_EVENT_CYCLE) {
		printk("Pmon report, event CYCLE.\n");
		printk(" freeze cycle: ");
		_print_val(lc);
		printk(", total cycle: ");
		_print_val(rc);
		unsigned long long rate = lc * 10000 / rc;
		printk("  (%lld%%%%)\n",rate);
	}
	else if(event == PMON_EVENT_INST) {
		printk("Pmon report, event INST.\n");
		printk(" useless insn: ");
		_print_val(lc);
		printk(", total insn: ");
		_print_val(rc);
		unsigned long long rate = (lc * 10000) / rc;
		printk("  (%lld%%%%)\n",rate);
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
	printk("%s",str);
}

