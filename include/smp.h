
#ifndef SMP_H
#define SMP_H

#include <config.h>
#include <base.h>

#ifdef CONFIG_SMP

#include <mipsop.h>

#define CPU_NR 4
#define smp_cpu_id()  (read_c0_ebase() & 0x3ff)
extern int setup_smp(void);
extern int setup_ipi(void);

extern int smp_ipi_func(int cpumask,void (*fun)(void));
extern int smp_call_func(int cpumask,void (*fun)(void *),void *data);

extern int smp_ipi_func_spec(int cpumask,void (*fun)(void));

extern int smp_running_cpus;
#define RUNNING_CPUS  ({smp_running_cpus;}) /* Cann't write */

#else  // !CONFIG_SMP

#define CPU_NR 1
#define smp_cpu_id() (0)
#define setup_smp()  do { } while(0)
#define setup_ipi()  do { } while(0)
#define smp_ipi_interrupt() do { } while(0)
#define smp_ipi_func(a,b)   do { } while(0)
#define smp_call_func(a,b)  do { } while(0)

#define smp_ipi_func_spec(a,b)   do { } while(0)

#define RUNNING_CPUS  (1)

#endif


#endif
