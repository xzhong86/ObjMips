
#ifndef PCPU_H
#define PCPU_H

#define __SMP_SIZE (0x100000)
#define __SMP_MASK (__SMP_SIZE - 1)
#define PCPU_BASE(cpu)  (0x81000000+(cpu)*__SMP_SIZE)
#define PCPU_HIGH(cpu)  (PCPU_BASE(cpu)+__SMP_SIZE-1)
#define PCPU_SP_TOP(cpu)  (PCPU_BASE(cpu)+__SMP_SIZE-16)

#ifndef K0_TO_K1
#define K0_TO_K1(a)   ((typeof(a))((unsigned)(a) + 0x20000000))
#endif

#ifndef __ASSEMBLY__

#ifndef __CURRENT_SP
register unsigned int __current_sp __asm__("$29");
#define __CURRENT_SP
#endif

#define PCPU_SP_CPU() ((__current_sp - 0x81000000) / __SMP_SIZE)
#define PCPU_SP_BASE() (__current_sp & ~__SMP_MASK)

struct per_cpu_var {
	/* use for smp_msg.c */
	void *smp_msg_st_p;
	/* use for smp_fun.c */
	void *smp_fun_st_p;
	/* use for pcpu_rand() */
	unsigned long rand_seed;
};
/* use this macro to get the pointer to per_cpu_var */
#define PCPU_VAR() ((struct per_cpu_var*)PCPU_SP_BASE())

int pcpu_init(void);
void *pcpu_malloc(int size);
int pcpu_free(void *p);
void pcpu_dump(void);

int pcpu_rand(void);
void pcpu_srand(unsigned int seed);

#endif /* IN_ASM */

#endif
