
#include <stdio.h>
#include <pcpu.h>
#include <smp.h>
#include <smp_fun.h>
#include <base.h>
#include <bitops.h>
#include <irq.h>

struct smp_fun {
#define FUN_QUEUE_MAX 16
	fun_t funq[FUN_QUEUE_MAX];
	volatile int qs, qe;
};
#define PCPU_FUN() ( (struct smp_fun*)(PCPU_VAR()->smp_fun_st_p) )

int smp_fun_init(void)
{
	int i;
	struct smp_fun *sf;
	sf = (struct smp_fun*)pcpu_malloc(sizeof(*sf));

	PCPU_VAR()->smp_fun_st_p = sf;
	for (i = 0; i < FUN_QUEUE_MAX; i++) 
		sf->funq[i] = NULL;
	sf->qs = 0;
	sf->qe = 0;
	return 0;
}

static unsigned long fun_busy;
int smp_fun_push(int lvl,fun_t fun)
{
	struct smp_fun *sf;
	sf = PCPU_FUN();

	sf->funq[sf->qe] = fun;
	sf->qe = (sf->qe+1) % FUN_QUEUE_MAX;

	return 0;
}

int smp_fun_run(void)
{
	int cpu = smp_cpu_id();
	int ret = 0;
	struct smp_fun *sf;
	sf = PCPU_FUN();

	set_bit(cpu, &fun_busy);
	while (sf->qs != sf->qe) {
		void (*fun)(void);
		
		fun = sf->funq[sf->qs];
		sf->qs = (sf->qs+1) % FUN_QUEUE_MAX;
		
		fun();
		ret ++;
	}
	clear_bit(cpu, &fun_busy);
	return ret;
}

int smp_fun_empty(void)
{
	struct smp_fun *sf;
	sf = PCPU_FUN();
	return sf->qe == sf->qs;
}

void smp_fun_wait(int cpumask)
{
	int times = 0xffff;
	int i,wait;

wait:
	wait = 0;
	for (i = 0; i < CPU_MAX; i++) {
		if((cpumask & (1<<i)) && test_bit(i, &fun_busy))
			wait ++;
	}
	if (wait && --times)
		goto wait;
	
	if (!times)
		printk("[FUN] Error!!! wait too long!\n");
}
