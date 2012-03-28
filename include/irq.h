
#ifndef IRQ_H
#define IRQ_H

struct cpu_regs {
	unsigned long *saved_regs;
	unsigned long cp0_epc, cp0_badvaddr;
	unsigned long cp0_status, cp0_cause;
};

extern int setup_irq(void);
extern void irq_entry(struct cpu_regs *reg);



#define IRQ_INTC_BASE 8
/* API to register a irq handle function, the IRQ NO. is
 * the bit in INTC reigster ICSR0/1 plus IRQ_INTC_BASE,
 * for example: DMA0 is bit 24 in ICSR0, so irq NO. is 
 * 24 + 8 = 32, and IRQ in ICSR1 will plus 32 more, like:
 * MSC0 is bit 5 of ICSR1, irq NO. is 45
 */
typedef void (*irqfun)(void);
extern int register_irqfun(int irq,irqfun fun,const char *name);
extern void unregister_irqfun(int irq);

/* get irq NO. from ICSR register value */
extern int tst_irq(unsigned int sr1,unsigned int sr0);

extern void print_irqs(void);

#include "mipsop.h"

static inline int irq_isenable(void)
{
	unsigned int st;
	st = read_c0_status();
	return st & 1;
}
static inline void irq_enable(void)
{
	unsigned int st;
	st = read_c0_status();
	st |= 1;
	write_c0_status(st);
}
static inline void irq_disable(void)
{
	unsigned int st;
	st = read_c0_status();
	st &= ~1;
	write_c0_status(st);
}

static inline void local_irq_save(int *flag)
{
	unsigned int st;
	st = read_c0_status();
	*flag = st & 1;
	st &= ~1;
	write_c0_status(st);
}
static inline void local_irq_restore(int *flag)
{
	unsigned int st;
	st = read_c0_status();
	st |= *flag & 1;
	write_c0_status(st);
}


#endif
