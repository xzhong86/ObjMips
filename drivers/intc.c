#include <bitops.h>

/* interrupt controller */
typedef unsigned int u32;
int intc_init(void)
{
//	volatile u32 *icmr0 = (u32*)0xB0001004;
//	volatile u32 *icmr1 = (u32*)0xB0001024;

//	PRINTF("Interrupt Controller:\n");
//	PRINTF("  ICMR0: %08x, ICMR1: %08x\n",*icmr0,*icmr1);
	return 0;
}
int intc_mask(unsigned int sr1,unsigned int sr0)
{
	volatile u32 *icmsr0 = (u32*)0xB0001008;
	volatile u32 *icmsr1 = (u32*)0xB0001028;
	
	if(sr1) *icmsr1 = sr1;
	if(sr0) *icmsr0 = sr0;

	return 0;
}
int intc_unmask(unsigned int sr1,unsigned int sr0)
{
	volatile u32 *icmcr0 = (u32*)0xB000100c;
	volatile u32 *icmcr1 = (u32*)0xB000102c;

	if(sr1) *icmcr1 = sr1;
	if(sr0) *icmcr0 = sr0;

	return 0;
}


int intc_get_no(void)
{
	volatile u32 *icpr0 = (u32*)0xB0001010;
	volatile u32 *icpr1 = (u32*)0xB0001030;
	
	unsigned int sr0 = *icpr0;
	unsigned int sr1 = *icpr1 & 0x7ff;

	if(sr0)
		return fls(sr0) -1;
	if(sr1) 
		return fls(sr1) -1 + 32;
	return 0;
}

/* for tst_irq() */
int intc_tst_no(unsigned int sr1,unsigned int sr0)
{
	if(sr0)
		return ffs(sr0) -1;
	if(sr1) 
		return ffs(sr1) -1 + 32;
	return 0;
}
