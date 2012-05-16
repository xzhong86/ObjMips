
#include <base.h>
#include <iomap.h>
#include <jzsoc.h>

#define DEV_REG_BASE	ddrc_base

#define DDRC_CFG	0x4
#define DDRC_MMAP0	0x24
#define DDRC_MMAP1	0x28

#define CFG_ROW1_SHF	27
#define CFG_ROW1_MASK	0x7
#define CFG_COL1_SHF	24
#define CFG_COL1_MASK	0x7
#define CFG_BA1		(1 << 23)
#define CFG_ROW_SHF	11
#define CFG_ROW_MASK	0x7
#define CFG_COL_SHF	8
#define CFG_COL_MASK	0x7
#define CFG_CS1EN	(1 << 7)
#define CFG_CS0EN	(1 << 6)
#define CFG_BA		(1 << 1)
#define CFG_DW		(1 << 0)

#define MMAP_MASK_MASK	(0xff)

static void *ddrc_base;
	
static long int initdram1(int board_type)
{
	u32 ddr_cfg;
	u32 rows1, cols1, dw, banks1, cs1;
	u32 size1;

	ddr_cfg = REG32(DDRC_CFG);
	dw  = (ddr_cfg & CFG_DW) ? 4 : 2;
	cs1 = (ddr_cfg & CFG_CS1EN) ? 1 : 0;

	if (cs1) {
		rows1 = 12 + ((ddr_cfg >> CFG_ROW1_SHF) & CFG_ROW1_MASK);
		cols1 = 8  + ((ddr_cfg >> CFG_COL1_SHF) & CFG_COL1_MASK);
		banks1 = (ddr_cfg & CFG_BA1) ? 8 : 4;
		size1 = (1 << (rows1 + cols1)) * dw * banks1;
		if (size1 > 0x40000000)
			size1 = 0x40000000;
		return size1;
	} else {
		return 0;
	}
}

static long int initdram(int board_type)
{
	u32 ddr_cfg;
	u32 rows, cols, dw, banks, cs0;
	u32 size = 0;

	ddr_cfg = REG32(DDRC_CFG);
	rows = 12 + ((ddr_cfg >> CFG_ROW_SHF) & CFG_ROW_MASK);
	cols = 8  + ((ddr_cfg >> CFG_COL_SHF) & CFG_COL_MASK);

	dw    = (ddr_cfg & CFG_DW) ? 4 : 2;
	banks = (ddr_cfg & CFG_BA) ? 8 : 4;
	cs0   = (ddr_cfg & CFG_CS0EN) ? 1 : 0;

	if (cs0) 
		size = (1 << (rows + cols)) * dw * banks;

	if (size > 0x20000000)
		size = 0x20000000;

	size += initdram1(0);

	return size;
}

static int ddrc_remap(void)
{
	unsigned int base0, size0, mask0, mmap0_reg;
	unsigned int base1, size1, mask1, mmap1_reg;
	unsigned int size, ahb0, ahb1, ahb2;

	size = initdram(0);
	size1 = initdram1(0);
	size0 = size - size1;

	base0 = 0;		/* remap to 0 */
	base1 = base0 + (size0 >> 24);
	mask0 = (~((size0 >> 24) - 1) & 0xff);
	mask1 = size1? ~((size1 >> 24) - 1): 0;

	mmap0_reg = (base0 << 8) | mask0;
	mmap1_reg = (base1 << 8) | mask1;

#define REG(addr)	(*(volatile unsigned *)(addr))
	ahb0 = REG(0xB3000000) | (1 << 31);
	ahb1 = REG(0xB3200000) | (1 << 15);
	ahb2 = REG(0xB3400000) | (1 << 31);

	/* .align X = 2^X */
	asm volatile (".align 5\n\tsync");
	REG32(DDRC_MMAP0) = mmap0_reg;
	REG32(DDRC_MMAP1) = mmap1_reg;
	REG(0xB3400000) = ahb2;
	REG(0xB3200000) = ahb1;
	REG(0xB3000000) = ahb0;

	return 0;
}

static int ioremapped;
static void setup_ioremap(char *arg)
{
	if (arg[0] == '1')
		ioremapped = 1;
}
__setup("ioremap", setup_ioremap);

unsigned int ddrc_ddr_size;
void dev_uart_remap(void);
int ddrc_init(void)
{
	unsigned int base, size;

	base = jzsoc_devs[JZSOC_INTC].base;
	ddrc_base = ioremap(base, 0x1000);
	if (ioremapped) {
		jzsoc_mem_remapped = 1;
		ddrc_remap();
		ddrc_base = ioremap(base, 0x1000);
		dev_uart_remap();
	}
	size = initdram(0);
	printk("ddr mem size %dMB %p\n", size>>20, ddrc_base);
	ddrc_ddr_size = size;
	return 0;
}
