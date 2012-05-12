#include <bitops.h>
#include <iomap.h>

#define DEV_REG_BASE	intc_base

#define ICSR0	0x0
#define ICMSR0	0x8
#define ICMCR0	0xC
#define ICPR0	0x10

#define ICSR1	0x20
#define ICMSR1	0x28
#define ICMCR1	0x2C
#define ICPR1	0x30

/* interrupt controller */
static void *intc_base;
int intc_mask(int int_no)
{
	if (int_no < 0 || int_no > 64)
		return -1;

	if (int_no >= 32)
		REG32(ICMSR1) = 1 << (int_no - 32);
	else
		REG32(ICMSR0) = 1 << int_no;
	return 0;
}
int intc_unmask(int int_no)
{
	if (int_no < 0 || int_no > 64)
		return -1;

	if (int_no >= 32)
		REG32(ICMCR1) = 1 << (int_no - 32);
	else
		REG32(ICMCR0) = 1 << int_no;
	return 0;
}
int intc_get_no(void)
{
	unsigned int sr0 = REG32(ICPR0);
	unsigned int sr1 = REG32(ICPR1);

	if (sr0)
		return fls(sr0) -1;
	if (sr1) 
		return fls(sr1) -1 + 32;
	return -1;
}

/* for tst_irq() */
int intc_tst_no(unsigned int sr1,unsigned int sr0)
{
	if (sr0)
		return _ffs(sr0) -1;
	if (sr1) 
		return _ffs(sr1) -1 + 32;
	return 0;
}


int intc_init(void)
{
	intc_base = ioremap(0x10001000, 0x200);
	return 0;
}
