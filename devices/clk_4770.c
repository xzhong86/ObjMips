#include <stdio.h>

#include <base.h>

struct soc_clk_t {
	unsigned int m,n,no,pll,pll1;
	unsigned int cclk,h0clk,h2clk,c1clk,pclk,h1clk;
} soc_clk;

typedef unsigned int u32;

#define JZ_EXTAL 12000000  // 12MHz
static int get_clk_info(void)
{
	unsigned long m, n, no, pll, cdiv, h0div, h2div, c1div, pdiv, h1div;
	unsigned long od[4] = {1, 2, 4, 8};
	unsigned int div[10] = {1, 2, 3, 4, 6, 8, 12, 16, 24, 32};
	volatile u32 *cpccr = (u32*)0xB0000000;
	volatile u32 *cppcr = (u32*)0xB0000010;

	soc_clk.m = m = (((*cppcr) >> 24) & 0x7f) + 1;
	soc_clk.n = n = (((*cppcr) >> 18) & 0x1f) + 1;
	soc_clk.no = no = ((*cppcr) >> 16) & 0x3;
	soc_clk.pll = pll = JZ_EXTAL * m / (n * od[no]);

	cdiv = *cpccr & 0xf;
	h0div = (*cpccr >> 4) & 0xf;
	pdiv = (*cpccr >> 8) & 0xf;
	c1div = (*cpccr >> 12) & 0xf;
	h2div = (*cpccr >> 16) & 0xf;
	h1div = (*cpccr >> 24) & 0xf;
	
#define X(name) soc_clk.name##clk = pll / div[(name##div)]
	X(c); X(h0); X(h2); X(c1); X(p); X(h1);
	return 0;
}
static void print_cpu_fre(void)
{
	if(soc_clk.pll == 0) get_clk_info();
	printk("print cpu frequency:\n");
	printk("m: %d, n: %d, no: %d\n",soc_clk.m,soc_clk.n,soc_clk.no);
	printk("extal: %dMHz, PLL out: %dMHz\n"
	       ,JZ_EXTAL/1000000,soc_clk.pll/1000000);
	printk("CPU     CLK: %dMHz\n",soc_clk.cclk/1000000);
	printk("AHB0    CLK: %dMHz\n",soc_clk.h0clk/1000000);
	printk("Per     CLK: %dMHz\n",soc_clk.pclk/1000000);
	printk("AUX cpu CLK: %dMHz\n",soc_clk.c1clk/1000000);
	printk("AHB2    CLK: %dMHz\n",soc_clk.h2clk/1000000);
	printk("VPU Bus CLK: %dMHz\n",soc_clk.h1clk/1000000);
	return;
}

#include <init.h>
#include <jzsoc.h>
static int clk_init(void)
{
	if (jzsoc_ver == JZSOC_4770)
		print_cpu_fre();
	return 0;
}
dev_initcall(clk_init, 0);
