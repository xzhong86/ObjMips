#include <stdio.h>

#include <pmon.h>

extern int main(int ,char**);
int _call_main(int ac,char *av[])
{
	int ret;

	//pmon_btb_disable();
	pmon_prepare(PMON_EVENT_CYCLE);
	pmon_start();
	
	/* call main function */
	ret = main(ac,av);
	
	pmon_stop();
	pmon_report();
	//pmon_btb_enable();
	
	return ret;
}
