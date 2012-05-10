#include <iomap.h>
#include <irq.h>
#include <base.h>

#include <console.h>

#define	UART0_BASE	0xB0030000
#define	UART1_BASE	0xB0031000
#define	UART2_BASE	0xB0032000
#define	UART3_BASE	0xB0033000
#define UART_BASE	UART0_BASE
#define UART_OFF	0x1000

#define OFF_RDR	(0x00)
#define OFF_TDR	(0x00)
#define OFF_IER	(0x04)
#define OFF_DLLR	(0x00)
#define OFF_DLHR	(0x04)
#define OFF_FCR	(0x08)
#define OFF_LSR	(0x14)
#define OFF_LCR	(0x0C)
#define OFF_ISR	(0x20)
#define OFF_UMR	(0x24)
#define OFF_UACR	(0x28)

#define IER_RDR	(1 << 0)
#define IER_TDR	(1 << 1)

#define LSR_DR  	(1 << 0)
#define LSR_TDRQ	(1 << 5)
#define LSR_TEMT	(1 << 6)


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
	return REG8(OFF_LSR) & LSR_DR;
}
static int uart_putc (const char c)
{
	/* Wait for fifo to shift out some bytes */
	while ((REG8(OFF_LSR) & (LSR_TDRQ | LSR_TEMT))
	       != 0x60)
		;
	REG8(OFF_TDR) = (u8)c;
	return 0;
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

static struct console fifo_console;
static void fifo_interrput(int irq, void *data)
{
	struct console *con = data;

	unsigned lsr = REG8(OFF_LSR) & (LSR_TDRQ | LSR_TEMT | LSR_DR);
	if (lsr & LSR_DR) {
		char ch = REG8(OFF_RDR);
		(con->rx_char)(con, ch);
	}
	if (lsr & LSR_TDRQ) {
		REG8(OFF_IER) &= ~IER_TDR;
		(con->tx_end)(con);
	}
	return ;
}
static int fifo_init(void)
{
	unsigned dlhr, dllr, umr, uacr;

	umr  = REG32(OFF_UMR);
	uacr = REG32(OFF_UACR);
	REG8(OFF_LCR) |= 0x80;
	dlhr = REG32(OFF_DLHR);
	dllr = REG32(OFF_DLLR);
	REG8(OFF_LCR) &= ~0x80;
	if (!umr && !uacr && !dlhr && !dllr)
		return -1;

	REG8(OFF_IER) = 0;
	REG8(OFF_FCR) = ~0x10;
	REG8(OFF_ISR) = ~0x3;
	REG8(OFF_LCR) = 0x3;	/* 8n1 */

	REG32(OFF_UMR) = umr;
	REG32(OFF_UACR) = uacr;

	REG8(OFF_LCR) |= 0x80;
	REG32(OFF_DLHR) = dlhr;
	REG32(OFF_DLLR) = dllr;
	REG8(OFF_LCR) &= ~0x80;

	REG8(OFF_IER) = IER_RDR;
	REG8(OFF_FCR) = 0x17;

	register_irqfun(3+8, fifo_interrput,"UART fifo",&fifo_console);
	return 0;
}
static int fifo_getc(void)
{
	if (uart_tstc())
		return REG8(OFF_RDR);
	return -1;
}
static int fifo_putc(const char ch)
{
	static int space;
	unsigned int lsr;

	if (space) {
		REG8(OFF_TDR) = ch;
		space --;
		return 0;
	} else {
		lsr = REG8(OFF_LSR) & (LSR_TDRQ | LSR_TEMT);
		space = lsr == (LSR_TDRQ | LSR_TEMT)? 32 : 
			lsr == LSR_TDRQ? 16 : 0;
		if (space) {
			REG8(OFF_TDR) = ch;
			space --;
			return 0;
		}
	}
	REG8(OFF_IER) |= IER_TDR;
	return -1;
}

static struct console uart_console = {
	.getchar = uart_getc,
	.putchar = uart_putc,
	.putstring = uart_puts,
};
static struct console fifo_console = {
	.getchar = fifo_getc,
	.putchar = fifo_putc,
};
int uart_init (void)
{
	int ret;

	printk("serial to fifo.\n");
	ret = fifo_init();
	if (ret)
		ret = register_console(&uart_console, CONSOLE_TYPE_DUMMY);
	else {
		ret = register_console(&fifo_console, CONSOLE_TYPE_FIFO);
		if (ret || !fifo_console.rx_char || !fifo_console.tx_end) {
			printk("fifo uart register failed.\n");
			return -1;
		}
	}

	printk("Init UART serial\n");
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
