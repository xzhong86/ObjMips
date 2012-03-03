
#include <stdio.h>
#include <stdarg.h>
#include <linux/spinlock.h>
#include <irq.h>

static arch_spinlock_t lock = { .lock = 0 };

#if 1 // CONFIG_NCSIM
void ncsim_error_addr(void *a0,void *a1,void *a2,void *a3)
{
	asm volatile ("nop;nop;nop;nop;nop;nop;ssnop;nop;nop;");
}
#endif

extern void serial_put_string(const char *str);
void smp_puts(char *str)
{
	int flag;
	local_irq_save(&flag);
	arch_spin_lock(&lock);
#if !defined CONFIG_NCSIM
	serial_put_string(str);
#endif
	arch_spin_unlock(&lock);
	local_irq_restore(&flag);
}

void smp_printf(char *fmt, ...)
{
	char buf[128];
	va_list ap;
	
	va_start(ap,fmt);
#if !defined CONFIG_NCSIM
	vsnprintf(buf,128,fmt,ap);
#endif
	va_end(ap);

	smp_puts(buf);
}


