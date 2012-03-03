

#ifndef SMP_FUN_H
#define SMP_FUN_H

#include "config.h"
#ifndef CONFIG_SMP
# warning "Use smp_fun.h without CONFIG_SMP!"
#endif

typedef void (*fun_t)(void);

int smp_fun_init(void);
int smp_fun_push(int lvl,fun_t fun);
int smp_fun_run(void);

/* test if current cpu fun queue is empty */
int smp_fun_empty(void);
void smp_fun_wait(int cpumask);

#endif
