
typedef unsigned int u32;
int pll_init(void)
{
	volatile u32 *cpccr = (u32*)0xB0000000;
	unsigned int cr,cdiv,hdiv;

	/* div[10] = {1, 2, 3, 4, 6, 8, 12, 16, 24, 32} */
	cdiv = 1;
	hdiv = 1;

	return 0;

	cr = (hdiv<<4) & cdiv;
	*cpccr = *cpccr | 0x400000;  // set change enable bit: CPCCR.CE

	*cpccr = (*cpccr & ~0xf) | (cdiv & 0xf);
	*cpccr = (*cpccr & ~0xf0) | (hdiv<<4 & 0xf0);

	*cpccr = *cpccr & ~0x400000;  // clear change enable bit: CPCCR.CE
	return 0;
}
