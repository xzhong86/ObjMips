#include <stdio.h>

#include <cache.h>
#include <mipsop.h>
#include <irq.h>
#include <smp.h>
#include <base.h>

int setup_irq(void)
{

	setup_ipi();

	return 0;
}


#define IRQ_NR 128
static int get_irq(void);

struct irq_isr {
	irqfun fun;
	void *data;
	const char *name;
};
static struct irq_isr isrs[IRQ_NR];
static unsigned int irqs[CPU_NR][IRQ_NR];
static int bad_irqs;

extern int intc_mask(int int_no);
extern int intc_unmask(int int_no);
extern int intc_get_no(void);
extern int intc_tst_no(unsigned int sr1,unsigned int sr0);

extern void smp_ipi_interrupt(int irq, void *data);

void irq_entry(struct cpu_regs *reg)
{
	unsigned int cs = reg->cp0_cause;
	int cpu = smp_cpu_id();
	int irq;
	
	cs &= reg->cp0_status & 0xff00;
	if (cs & 0x800) {
		irq = 3;
		smp_ipi_interrupt(irq, isrs[irq].data);
		irqs[cpu][irq] ++;
	}
	else if (cs & 0x400) {
		irq = get_irq();
		if (isrs[irq].fun) 
			(isrs[irq].fun)(irq, isrs[irq].data);
		else {
			intc_mask(irq - IRQ_INTC_BASE);
			bad_irqs ++;
		}
		irqs[cpu][irq] ++;
		//irqs[cpu][2] ++;
	}
	else if (cs & 0x100) {
		irq = 0;
		irqs[cpu][irq] ++;
		reg->cp0_epc += 4;
	}
	else if (cs & 0x200) {
		irq = 1;
		irqs[cpu][irq] ++;
		reg->cp0_epc += 4;
	}
	else
		bad_irqs ++;

	if (bad_irqs > 1000000) {
		printk("Too many bad irqs!\n");
		print_irqs();
		*((volatile int*)0);
	}
}

static int get_irq(void)
{
	int no = intc_get_no();
	if (no)
		return no + IRQ_INTC_BASE;
	else
		return 0;
}
int tst_irq(unsigned int sr1,unsigned int sr0)
{
	return intc_tst_no(sr1,sr0) + IRQ_INTC_BASE;
}

int register_irqfun(int irq, irqfun fun,
		    const char *name, void *data)
{
	if (isrs[irq].fun)
		return -1;
	isrs[irq].fun = fun;
	isrs[irq].data = data;
	isrs[irq].name = name;
	intc_unmask(irq - IRQ_INTC_BASE);
	return 0;
}
void unregister_irqfun(int irq)
{
	isrs[irq].fun = NULL;
	intc_mask(irq - IRQ_INTC_BASE);
	return;
}

void print_irqs(void)
{
	int cpu = smp_cpu_id();
	int i;
	printk("cpu%d irqs info:\n",cpu);
	for (i = 0; i < IRQ_NR; i++) {
		if (isrs[i].fun || irqs[cpu][i])
			printk("%-3d:%d\t%s\n",i,irqs[cpu][i],
			       isrs[i].name?isrs[i].name:"no name");
	}
	printk("bad irq: %d\n", bad_irqs);
}

#include <command.h>
#include <string.h>
static void do_irq_help(char *str)
{
	printk("irq show\n");
}
static int do_irq(int argc,char *argv[])
{
	if (argc < 2) {
		do_irq_help(NULL);
		return 0;
	}

	if (strcmp(argv[1],"show") == 0)
		print_irqs();

	return 0;
}
DEF_COMMAND("irq",do_irq,do_irq_help);
