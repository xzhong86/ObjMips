/*
 * Jz47xx UART support
 * copy form uboot-1.1.6
 *
 * Hardcoded to UART 0 for now
 * Options also hardcoded to 8N1
 *
 *  Copyright (c) 2005
 *  Ingenic Semiconductor, <jlwei@ingenic.cn>
 * 
 * common serial code, use uart which inited by uboot.
 */

#include <base.h>
#include "jz_serial.h"
#include "serial.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#ifndef UART_OFF
#define UART_OFF 0x1000
#endif

static volatile u8 *uart_base;

static void serial_putc (const char c);
static void serial_puts (const char *str);

/* check which port used. */
static void serial_chk(void)
{
	volatile u8 *uart;
	int p;
	
	for(p = 0; p < 4; p++) {
		uart = (u8*)(UART_BASE + p * UART_OFF);
		uart[OFF_TDR] = "0123"[p];
	}
	for(p = 0; p < 4; p++) {
		uart = (u8*)(UART_BASE + p * UART_OFF);
		if (uart[OFF_LCR]) 
			break;
	}
	if (p == 4)
		uart_base = (u8*)(UART2_BASE);
	else
		uart_base = uart;
}

static int serial_tstc (void)
{
	return REG8(OFF_LSR) & UART_LSR_DR;
}
static void serial_putc (const char c)
{
	/* Wait for fifo to shift out some bytes */
	while (!((REG8(OFF_LSR) & (UART_LSR_TDRQ | UART_LSR_TEMT)) == 0x60))
		;
	REG8(OFF_TDR) = (u8)c;
}
static void serial_puts (const char *str)
{
}

static int serial_getc (void)
{
	volatile u8 *uart_rdr = uart_base + OFF_RDR;

	while (!serial_tstc());

	return *uart_rdr;
}

static int serial_init (void)
{
	/* change UART_PORT with chk value. */
	serial_chk();
	serial_puts("Init common serial.\n");
	printk("printk in serial init.\n");
	return 0;
}

struct serial_ops serial_common = {
	.init = serial_init,
	.putc = serial_putc,
	.puts = serial_puts,
	.getc = serial_getc,
};
