
#ifndef SMP_IO_H
#define SMP_IO_H

void smp_puts(char *str);
void smp_printf(char *fmt, ...) __attribute__ ((__format__ (__printf__, 1, 2)));


#define PRINTF(args...) smp_printf(args)
#ifndef printk
#define printk(args...) smp_printf(args)
#endif

#if 1				/* always exist */
void ncsim_error_addr(void *a0,void *a1,void *a2,void *a3);
#else
static inline 
void ncsim_error_addr(void *a0,void *a1,void *a2,void *a3)
{ return; }
#endif

#endif
