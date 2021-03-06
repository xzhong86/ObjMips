#include <stdio.h>

#include <mipsop.h>
#include <cache.h>
#include <irq.h>
#include <base.h>
#include <smp.h>
#include <smp_msg.h>
#include <smp_fun.h>
#include <pcpu.h>
#include <thread.h>

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
extern int mem_init(void);
extern int mmu_init(void);
extern int thread_init(void);
static void soft_init(void)
{
	//blast_cache_all();
	per_cpu_init_0();
	cache_init();
	setup_smp();
	setup_trap();
	mem_init();
	mmu_init();
	thread_init();
}

extern int ddrc_init(void);
extern int intc_init(void);
extern int uart_init(void);
extern int ost_init(void);
static void base_device_init(void)
{
	ddrc_init();
	intc_init();
	uart_init();
	ost_init();
}

#include <jzsoc.h>
extern void uart_early_init(void);
static void early_device_init(void)
{
	unsigned int prid = read_c0_prid();
	jzsoc_probe(prid);
	uart_early_init();
	printk("\nProduct : %s\n", jzsoc_name[jzsoc_ver]);
	printk("CPU Prid : %08x\n", prid);
}

extern void param_init(void);
extern void init_initcalls(void);
extern int _call_main(int ac,char *av[]);
static int main_thread(void *data)
{
	thread_new_group(current_thread());
	return _call_main(0, NULL);
}

extern void cpu_idle_loop(void);
void base_entry_fun(void)
{
	early_device_init();
	param_init();
	soft_init();
	base_device_init();

	irq_enable();
	init_initcalls();

	__thread_create(main_thread, NULL, "main", 0x1, 16*1024);
	cpu_idle_loop();
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
