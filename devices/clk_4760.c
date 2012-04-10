#include <stdio.h>

#include <base.h>
#include <clock.h>

struct soc_clk_t {
	unsigned int m,n,no,pll,pll1;
	unsigned int cclk,sclk,h2clk,mclk,pclk,hclk;
} soc_clk;

typedef unsigned int u32;

#define JZ_EXTAL 12000000  // 12MHz
static int get_clk_info(void)
{
	unsigned long m, n, no, pll, cdiv, sdiv, h2div, mdiv, pdiv, hdiv;
	unsigned long od[4] = {1, 2, 4, 8};
	unsigned int div[10] = {1, 2, 3, 4, 6, 8, 12, 16, 24, 32};
	volatile u32 *cpccr = (u32*)0xB0000000;
	volatile u32 *cppcr = (u32*)0xB0000010;

	soc_clk.m = m = ((*cppcr) >> 24) & 0x7f;
	soc_clk.n = n = ((*cppcr) >> 18) & 0xf;
	soc_clk.no = no = ((*cppcr) >> 16) & 0x3;
	soc_clk.pll = pll = JZ_EXTAL * m*2 / (n * od[no]);

	cdiv = *cpccr & 0xf;
	hdiv = (*cpccr >> 4) & 0xf;
	pdiv = (*cpccr >> 8) & 0xf;
	mdiv = (*cpccr >> 12) & 0xf;
	h2div = (*cpccr >> 16) & 0xf;
	sdiv = (*cpccr >> 24) & 0xf;
	
#define X(name) soc_clk.name##clk = pll / div[(name##div)]
	X(c); X(s); X(h2); X(m); X(p); X(h);
	return 0;
}
unsigned int get_cpu_fre(void)
{
	if(soc_clk.pll == 0) get_clk_info();
	return soc_clk.cclk;
}
unsigned int get_pclk_fre(void)
{
	if(soc_clk.pll == 0) get_clk_info();
	return soc_clk.pclk;
}
void print_cpu_fre(void)
{
	if(soc_clk.pll == 0) get_clk_info();
	printk("print cpu frequency:\n");
	printk("m: %d, n: %d, no: %d\n",soc_clk.m,soc_clk.n,soc_clk.no);
	printk("extal: %dMHz, PLL out: %dMHz\n"
	       ,JZ_EXTAL/1000000,soc_clk.pll/1000000);
	printk("CPU    CLK: %dMHz\n",soc_clk.cclk/1000000);
	printk("AHB0   CLK: %dMHz\n",soc_clk.hclk/1000000);
	printk("Per    CLK: %dMHz\n",soc_clk.pclk/1000000);
	printk("Memory CLK: %dMHz\n",soc_clk.mclk/1000000);
	printk("AHB2   CLK: %dMHz\n",soc_clk.h2clk/1000000);
	printk("SRAM   CLK: %dMHz\n",soc_clk.sclk/1000000);
	return;
}
