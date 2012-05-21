/*
 *  Copyright (C) 2008 Ingenic Semiconductor Inc.
 *
 *  Author: <zpzhong@ingenic.cn>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ASM_MACH_XBURST_JZSOC_H__
#define __ASM_MACH_XBURST_JZSOC_H__

enum jzsoc_ver {
	JZSOC_4740, JZSOC_4750, JZSOC_4760,
	JZSOC_4760b,JZSOC_4770, JZSOC_4780,

	JZSOC_VER_NR,
};
extern enum jzsoc_ver jzsoc_ver;
extern const char *jzsoc_name[JZSOC_VER_NR];

enum jzsoc_base_dev {
	JZSOC_INTC, JZSOC_OST,  JZSOC_DDRC,

	/* the uart order is important */
	JZSOC_UART0,JZSOC_UART1,JZSOC_UART2,
	JZSOC_UART3,JZSOC_UART4,

	JZSOC_DEV_NR,
};
#define JZSOC_UART_MAX	(JZSOC_UART4 + 1)

struct jzsoc_device {
	/* phys address like 0x10010000 */
	unsigned long base;
	int irq;
};
extern struct jzsoc_device * jzsoc_devs;

int jzsoc_probe(unsigned long prid);

#endif
