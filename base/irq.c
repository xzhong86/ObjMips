#include <stdio.h>

#include <cache.h>
#include <mipsop.h>
#include <irq.h>
#include <intc.h>
#include <smp.h>
#include <base.h>

int setup_irq(void)
{

	setup_ipi();

	return 0;
}


#define IRQ_NR 128
static int get_irq(void);
static unsigned int irqs[CPU_NR][IRQ_NR];
static irqfun irqfuns[IRQ_NR];
static const char *irqnames[IRQ_NR];

void irq_entry(struct cpu_regs *reg)
{
	unsigned int cs = reg->cp0_cause;
	int cpu = smp_cpu_id();
	int irq;
	
	cs &= reg->cp0_status & 0xff00;
	if(cs & 0x800) {
		smp_ipi_interrupt();
		irq = 3;
		irqs[cpu][irq] ++;
	}
	else if(cs & 0x400) {
		irq = get_irq();
		if(irqfuns[irq])
			(irqfuns[irq])();
		irqs[cpu][irq] ++;
		irqs[cpu][2] ++;
	}
	else if(cs & 0x100) {
		irq = 0;
		irqs[cpu][irq] ++;
		reg->cp0_epc += 4;
	}
	else if(cs & 0x200) {
		irq = 1;
		irqs[cpu][irq] ++;
		reg->cp0_epc += 4;
	}
}

static int get_irq(void)
{
	int no = intc_get_no();
	if(no)
		return no + 8;
	else
		return 0;
}
int tst_irq(unsigned int sr1,unsigned int sr0)
{
	return intc_tst_no(sr1,sr0) + 8;
}

int register_irqfun(int irq,irqfun fun,const char *name)
{
	if(irqfuns[irq])
		return -1;
	irqfuns[irq] = fun;
	irqnames[irq] = name;
	return 0;
}
void unregister_irqfun(int irq)
{
	irqfuns[irq] = NULL;
	return;
}

void print_irqs(void)
{
	int cpu = smp_cpu_id();
	int i;
	printk("cpu%d irqs info:\n",cpu);
	for(i=0;i<IRQ_NR;i++) {
		if(irqfuns[i] || irqs[cpu][i])
			printk("%-3d:%d\t%s\n",i,irqs[cpu][i],
			       irqnames[i]?irqnames[i]:"no name");
	}
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
