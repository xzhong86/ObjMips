
#include <stdio.h>
#include <stdarg.h>
#include <spinlock.h>
#include <irq.h>

static spinlock_t lock;

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
	int flag;

	local_irq_save(&flag);
	spinlock_lock(lock);

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


