
#include <jzsoc.h>

enum jzsoc_ver jzsoc_ver;
const char *jzsoc_name[JZSOC_VER_NR] = {
	[JZSOC_4740] = "Jz4740",
	[JZSOC_4750] = "Jz4750",
	[JZSOC_4760] = "Jz4760",
	[JZSOC_4760b] = "Jz4760b",
	[JZSOC_4770] = "Jz4770",
	[JZSOC_4780] = "Jz4780",
};

static struct jzsoc_device jz4770_devs[JZSOC_DEV_NR] = {
	[JZSOC_INTC] = { 0x10001000, 2 },
	[JZSOC_DDRC] = { 0x13020000, 0 },
	[JZSOC_OST]  = { 0x10002000, 27+8 },
	[JZSOC_UART0] = { 0x10030000, 5+8 },
	[JZSOC_UART1] = { 0x10031000, 4+8 },
	[JZSOC_UART2] = { 0x10032000, 3+8 },
	[JZSOC_UART3] = { 0x10033000, 2+8 },
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

struct jzsoc_device * jzsoc_devs = jz4770_devs;

int jzsoc_probe(unsigned long prid)
{
	switch (prid) {
	case 0x2ed0024f:
		/* 4760b has a same prid */
		/* we use 4770 now, change it in feature */
		jzsoc_devs = jz4770_devs;
		jzsoc_ver = JZSOC_4760;
		break;
	case 0x2ed1024f:
		jzsoc_devs = jz4770_devs;
		jzsoc_ver = JZSOC_4770;
		break;
	case 0x3ee9024f:
		jzsoc_devs = jz4780_devs;
		jzsoc_ver = JZSOC_4780;
		break;
	default:
		jzsoc_devs = jz4770_devs;
		jzsoc_ver = JZSOC_4770;
		break;
	}
	return 0;
}
