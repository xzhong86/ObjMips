#include <stdio.h>
#include <smp.h>
#include <smp_stat.h>
#include <smp_io.h>

char *cpu_stat_str[CPU_NR] = { 
	NULL, NULL
};

static inline char * get_str(int cpu)
{
	char **p = K0_TO_K1(&cpu_stat_str[cpu]);
	return *p;
}
static inline void set_str(int cpu,char *str)
{
	char **p = K0_TO_K1(&cpu_stat_str[cpu]);
	*p = str;
}

void smp_stat_show(int cpumask)
{
	int i;
	for(i=0;i<CPU_NR;i++) {
		char *str;
		if(!(cpumask & (1<<i)))
			continue;
		str = get_str(i);
		smp_printf("cpu%d: %s\n",i,str);
	}

}
void smp_stat_clear(int cpumask)
{
	int i;
	for(i=0;i<CPU_NR;i++)
		if(cpumask & (1<<i))
			set_str(i,NULL);
}


