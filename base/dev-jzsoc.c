
#include <mipsop.h>
#include <jzsoc.h>

struct jzsoc_device * jzsoc_devs;

static struct jzsoc_device jz4770_devs[JZSOC_DEV_NR] = {
	[JZSOC_INTC] = { 0x10001000, 2 },
	[JZSOC_DDRC] = { 0x13020000, 0 },
	[JZSOC_OST]  = { 0x10002000, 27+8 },
	[JZSOC_UART0] = { 0x10030000, 13 },
	[JZSOC_UART1] = { 0x10031000, 12 },
	[JZSOC_UART2] = { 0x10032000, 11 },
	[JZSOC_UART3] = { 0x10033000, 10 },
};

static struct jzsoc_device jz4780_devs[JZSOC_DEV_NR] = {
	[JZSOC_INTC] = { 0x10001000, 2 },
	[JZSOC_DDRC] = { 0x13010000, 20+32+8 },
	[JZSOC_OST]  = { 0x12000000, 27+8 },
	[JZSOC_UART0] = { 0x10030000, 19+32+8 },
	[JZSOC_UART1] = { 0x10031000, 18+32+8 },
	[JZSOC_UART2] = { 0x10032000, 17+32+8 },
	[JZSOC_UART3] = { 0x10033000, 16+32+8 },
};

enum jzsoc_version jzsoc_ver;

int jzsoc_probe(void)
{
	unsigned long prid;

	prid = read_c0_prid();
	switch (prid) {
	case 0x2ed0024f:
		/* 4760b has a same prid */
		/* we use 4770 now, change it in feature */
		jzsoc_devs = jz4770_devs;
		jzsoc_ver = JZSOC_JZ4760;
		break;
	case 0x2ed1024f:
		jzsoc_devs = jz4770_devs;
		jzsoc_ver = JZSOC_JZ4770;
		break;
	case 0x3ee9024f:
		jzsoc_devs = jz4780_devs;
		jzsoc_ver = JZSOC_JZ4780;
		break;
	default:
		jzsoc_devs = jz4770_devs;
		jzsoc_ver = JZSOC_JZ4770;
		break;
	}
	return 0;
}
