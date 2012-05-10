
#include <stdio.h>
#include <stdlib.h>

#include <spinlock.h>
#include <sem.h>
#include <mem.h>
#include <console.h>

static struct console *fifo_console;

#define RX_BUF_LEN	64
#define TX_BUF_LEN	4096
static struct con_buf {
	struct qbuf {
		char *buf;
		int st,ed;
	} rx,tx;
	spinlock_t tx_lock;
	int rx_empty,tx_busy;
	sem_t rx_sem, tx_sem;
} buffer;
static void rx_char(struct console *con, char ch)
{
	struct qbuf *qb = &buffer.rx;
	int ed = qb->ed + 1;
	if (ed == RX_BUF_LEN)
		ed = 0;
	if (ed != qb->st) {
		qb->buf[qb->ed] = ch;
		qb->ed = ed;
	}
	if (buffer.rx_empty) {
		sem_up(&buffer.rx_sem, 1);
		buffer.rx_empty = 0;
	}
}
static int con_getchar(void)
{
	struct qbuf *qb = &buffer.rx;
	char ch;
	do {
		if (qb->st != qb->ed) {
			ch = qb->buf[qb->st];
			if (++qb->st == RX_BUF_LEN)
				qb->st = 0;
			return ch;
		}
		buffer.rx_empty = 1;
		sem_down(&buffer.rx_sem, 1);
	} while (1);
}
static void fifo_out(struct qbuf *qb)
{
	static spinlock_t lock;
	if (!spinlock_trylock(lock))
		return;
	while (qb->st != qb->ed) {
		char ch = qb->buf[qb->st];
		if ((fifo_console->putchar)(ch))
			break;
		if (++qb->st == TX_BUF_LEN)
			qb->st = 0;
	}
	spinlock_unlock(lock);
}
static void tx_end(struct console *con)
{
	fifo_out(&buffer.tx);
	if (buffer.tx_busy == 1) {
		sem_up(&buffer.tx_sem, 1);
		buffer.tx_busy = 2;
	}
}
static int put_char(struct qbuf *qb, char ch, int cansleep)
{
	do {
		int ed = qb->ed + 1;
		if (ed == TX_BUF_LEN)
			ed = 0;
		if (ed != qb->st) {
			qb->buf[qb->ed] = ch;
			qb->ed = ed;
			break;
		} else if (cansleep) {
			buffer.tx_busy = 1;

			spinlock_unlock(buffer.tx_lock);
			fifo_out(qb);
			sem_down(&buffer.tx_sem, 1);
			spinlock_lock(buffer.tx_lock);

			buffer.tx_busy = 0;
		} else 
			return -1;
	} while (1);
	return 0;
}
static int con_putchar(char ch)
{
	spinlock_lock(buffer.tx_lock);
	put_char(&buffer.tx, ch, 1);
	spinlock_unlock(buffer.tx_lock);
	fifo_out(&buffer.tx);
	return 0;
}
static void con_putstring(const char *str)
{
	spinlock_lock(buffer.tx_lock);
	if (buffer.tx_busy == 0) {
		while (*str) {
			if (*str == '\n') 
				put_char(&buffer.tx, '\r', 1);
			put_char(&buffer.tx, *str, 1);
			str ++;
		}
	}
	spinlock_unlock(buffer.tx_lock);
	fifo_out(&buffer.tx);
}
static int con_init(struct console *con)
{
	fifo_console = con;
	buffer.rx.buf = malloc(RX_BUF_LEN);
	if (!buffer.rx.buf)
		return -1;
	buffer.tx.buf = mem_alloc(TX_BUF_LEN);
	if (!buffer.tx.buf) {
		free(buffer.rx.buf);
		return -1;
	}
	sem_init(&buffer.rx_sem, 0);
	sem_init(&buffer.tx_sem, 0);
	con->rx_char = rx_char;
	con->tx_end = tx_end;
	return 0;
}
struct console console = {
	.getchar	= con_getchar,
	.putstring	= con_putstring,
	.putchar	= con_putchar,
};

static void puts_nosleep(const char *str)
{
	spinlock_lock(buffer.tx_lock);
	if (buffer.tx_busy == 0) {
		int ret = 0;
		while (*str && ret == 0) {
			if (*str == '\n') 
				put_char(&buffer.tx, '\r', 0);
			ret = put_char(&buffer.tx, *str, 0);
			str ++;
		}
	}
	spinlock_unlock(buffer.tx_lock);
	fifo_out(&buffer.tx);
}

static void (*printk_puts)(const char *);
static struct console *def_console;
int register_console(struct console *con, enum console_type type)
{
	if (type == CONSOLE_TYPE_DUMMY)
		def_console = con;
	else {
		if (con_init(con) == 0) {
			def_console = &console;
			def_console->putstring("fifo console works!.\n");
			printk_puts = puts_nosleep;
		}
	}
	return 0;
}

struct console *get_defconsole(void)
{
	return def_console;
}



#include <irq.h>
#include <stdarg.h>

#if 1 // CONFIG_NCSIM
void ncsim_error_addr(void *a0,void *a1,void *a2,void *a3)
{
	asm volatile ("nop;nop;nop;nop;nop;nop;ssnop;nop;nop;");
}
#endif

extern void serial_put_string(const char *str);
#if !defined CONFIG_NCSIM
static void smp_puts(char *str)
{
	static spinlock_t lock;
	int flag;

	local_irq_save(&flag);
	spinlock_lock(lock);

	if (printk_puts)
		printk_puts(str);
	else
		serial_put_string(str);

	spinlock_unlock(lock);
	local_irq_restore(&flag);
}
#endif

void printk(char *fmt, ...)
{
#if !defined CONFIG_NCSIM
	char buf[128];
	va_list ap;
	
	va_start(ap,fmt);
	vsnprintf(buf,128,fmt,ap);
	va_end(ap);

	smp_puts(buf);
#endif
}
