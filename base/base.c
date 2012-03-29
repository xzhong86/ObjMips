#include <stdio.h>

#include <mipsop.h>
#include <cache.h>
#include <smp.h>
#include <smp_io.h>
#include <smp_msg.h>
#include <smp_fun.h>
#include <pcpu.h>

/* also called in start_secondary(void) */
int per_cpu_init_0(void)
{
	pcpu_init();
	smp_msg_init();
	smp_fun_init();
	return 0;
}
extern int setup_trap(void);
extern int cache_init(void);
extern int mmu_init(void);
static void soft_init(void)
{
	//blast_cache_all();
	per_cpu_init_0();
	cache_init();
	mmu_init();
	setup_smp();
	setup_trap();
	smp_printf("soft_init() finished.\n");
}

extern void param_init(void);
extern void init_initcalls(void);
void software_init_fun(void)
{
	param_init();
	soft_init();
	init_initcalls();
}

void print_c0_regs(void)
{
	unsigned int status,cause,ebase,epc;
	status = read_c0_status();
	cause = read_c0_cause();
	ebase = read_c0_ebase();
	epc = read_c0_epc();

	smp_printf("status \t:%08x, cause\t:%08x\n",status,cause);
	smp_printf("ebase  \t:%08x, epc  \t:%08x\n",ebase,epc);
}

void cpu_wait(void)
{
	unsigned long tmp, save, bits;

	bits = 0xfffffff8;
	asm volatile (
		"	.set noreorder		\n"
		"	mfc0	%1, $12, 0	\n"
		"	and	%0, %1, %2	\n"
		"	mtc0	%0, $12, 0	\n"
		"	sync			\n"
		"	wait			\n"
		"	ssnop			\n"
		"	mtc0	%1, $12, 0	\n"
		"	.set reorder		\n"
		:"=r"(tmp),"=r"(save):"r"(bits));
}

