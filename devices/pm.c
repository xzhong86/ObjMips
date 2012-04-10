
#include <stdio.h>
#include <base.h>

typedef unsigned int u32;
int pm_init(void)
{
	volatile u32 *clkgr0 = (u32*)0xB0000020;
	volatile u32 *clkgr1 = (u32*)0xB0000028;

	/* support AHB,DDR,UART2 */
	*clkgr0 = 0x3ffdffff;
	/* close all 16bits */
	*clkgr1 = 0x0000ffff;
	
	return 0;
}

const char *clkgr0strs[] = {
	"NEMC","BCH","OTG","MSC0","SSI0", /**/ "I2C0","I2C1","SCC","AIC","TSSI",
	"OWI","MSC1","MSC2","KBC","SDAC", /**/ "UART0","UART1","UART2","UART3","SSI1",
	"SSI2","DMAC","GPS","MAC","UHC",  /**/ "MDMA","CIM","TVE","LCD","IPU",
	"DDR","AHB_MON",""
};
const char *clkgr1strs[] = {
	"BDMA","MC","DBLK","ME","DCT", /**/ "SDRAM","CABAC","VPU","PCM0","GPU",
	"PCM1","UART4","OSD","I2S2CH","AUX", /**/ "I2C2","","","","",
};
void pm_show_clkgr(void)
{
	volatile u32 *clkgr0 = (u32*)0xB0000020;
	volatile u32 *clkgr1 = (u32*)0xB0000028;
	int i,f;
	
	printk("CLKGR0\t: %08x\n",*clkgr0);
	printk("CLKGR1\t: %08x\n",*clkgr1);

	printk("working modules: ");
	for(i=0,f=1; i<32; i++,f<<=1)
		if(!(*clkgr0 & f))
			printk("%s,",clkgr0strs[i]);
	
	for(i=0,f=1; i<16; i++,f<<=1)
		if(!(*clkgr1 & f))
			printk("%s,",clkgr1strs[i]);
	
	printk("\n");
}

