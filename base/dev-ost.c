#include <stdio.h>
#include <irq.h>
#include <iomap.h>
#include <ktime.h>
#include <jzsoc.h>

static void *ost_base;
#define DEV_REG_BASE	ost_base

#define TFCR	0x28
#define TESR	0x14
#define TSCR	0x3c
#define TMCR	0x38
#define OSTDR	0xe0
#define OSTCNT	0xe8
#define OSTCSR	0xec


extern void ktimer_tick(void);
static volatile unsigned long ost_ticks;
static void ost_irq(int irq, void *d)
{
	REG32(TFCR) = 0x8000;
	ost_ticks ++;
	ktimer_tick();
}
unsigned long long sched_clock(void)
{
	unsigned long long time;
	unsigned long tick;

	do {
		tick = ost_ticks;
		/* in case 12MHz */
		time = REG32(OSTCNT) * (1000*HZ/12);
		time += tick * (1000000000UL / HZ);
	} while (tick != ost_ticks);
	return time;
}
int ost_init(void)
{
	ost_base = ioremap(jzsoc_devs[JZSOC_OST].base, 0x1000);

	REG32(OSTCSR) = 0;
	REG32(OSTCNT) = 0;

	REG32(OSTDR) = 12000000 / HZ;
	REG32(OSTCSR) = 4;     // clk = extal/1
	REG32(TSCR) = 0x8000;  // clear stop
	REG32(TMCR) = 0x8000;  // clear mask
	REG32(TESR) = 0x8000;  // enable counter

	register_irqfun(jzsoc_devs[JZSOC_OST].irq, ost_irq,"OST",NULL);

	return 0;
}
