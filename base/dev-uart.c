#include <iomap.h>
#include <base.h>

#define	UART0_BASE	0xB0030000
#define	UART1_BASE	0xB0031000
#define	UART2_BASE	0xB0032000
#define	UART3_BASE	0xB0033000
#define UART_BASE	UART0_BASE
#define UART_OFF	0x1000

#define OFF_RDR	(0x00)
#define OFF_TDR	(0x00)
#define OFF_LSR	(0x14)
#define OFF_LCR	(0x0C)

#define UART_LSR_DR	(1 << 0)
#define UART_LSR_TDRQ	(1 << 5)
#define UART_LSR_TEMT	(1 << 6)


#define DEV_REG_BASE	uart_base

static volatile u8 *uart_base;

/* check which port used. */
static void uart_chk(void)
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
static int uart_tstc (void)
{
	return REG8(OFF_LSR) & UART_LSR_DR;
}
static void uart_putc (const char c)
{
	/* Wait for fifo to shift out some bytes */
	while ((REG8(OFF_LSR) & (UART_LSR_TDRQ | UART_LSR_TEMT))
	       != 0x60)
		;
	REG8(OFF_TDR) = (u8)c;
}
static void uart_puts (const char *str)
{
	while (*str) {
		if (*str == '\n') 
			uart_putc('\r');
		uart_putc(*str);
		str ++;
	}
}
static int uart_getc (void)
{
	while (!uart_tstc());
	return REG8(OFF_RDR);
}

void uart_early_init(void)
{
	uart_chk();
}
void dev_uart_remap(void)
{
	uart_base = ioremap(K1_TO_PHYS(uart_base), 0x200);
}

#include <console.h>
static struct console uart_console = {
	.getchar = uart_getc,
	.putchar = uart_putc,
	.putstring = uart_puts,
};

int uart_init (void)
{
	/* change UART_PORT with chk value. */
	register_console(&uart_console);

	printk("Init UART serial.\n");
	return 0;
}


/* compatiable with old code */
void serial_put_string(const char *str)
{
	uart_puts(str);
}
void serial_out_byte(char c)
{
	uart_putc(c);
}
char serial_in_byte(void)
{
	return uart_getc();
}
