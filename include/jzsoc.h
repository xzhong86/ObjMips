
#ifndef __JZSOC_H__
#define __JZSOC_H__

enum jzsoc_devices {
	JZSOC_INTC,
	JZSOC_DDRC,
	JZSOC_OST,
	JZSOC_UART0,
	JZSOC_UART1,
	JZSOC_UART2,
	JZSOC_UART3,

	JZSOC_DEV_NR,
};

struct jzsoc_device {
	/* old phys address like 0x10001000 */
	unsigned long base;
	int irq;
};
/* a pointer to struct jzsoc_device jzsoc_devs[JZSOC_DEV_NR]; */
extern struct jzsoc_device * jzsoc_devs;


enum jzsoc_version {
	JZSOC_JZ4740, JZSOC_JZ4750, JZSOC_JZ4760,
	JZSOC_JZ4760b,JZSOC_JZ4770, JZSOC_JZ4780, 

	JZSOC_VER_NR,
};
extern enum jzsoc_version jzsoc_ver;
extern const char *jzsoc_name[JZSOC_VER_NR];

#endif
