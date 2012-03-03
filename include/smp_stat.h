
#ifndef SMP_STAT_H
#define SMP_STAT_H

#include <smp.h>
#include <pcpu.h>
extern char *cpu_stat_str[CPU_NR];

static inline void smp_stat_set(const char* str)
{
	int cpu = smp_cpu_id();
	char **p = K0_TO_K1(&cpu_stat_str[cpu]);
	*p = (char*)str;
}
#define SMP_STAT_FUNC() smp_stat_set(__func__)

void smp_stat_show(int cpumask);
void smp_stat_clear(int cpumask);

#endif
