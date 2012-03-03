
#ifndef BARRIER_H
#define BARRIER_H

#define __sync()				\
	__asm__ __volatile__(			\
		".set	push\n\t"		\
		".set	noreorder\n\t"		\
		".set	mips2\n\t"		\
		"sync\n\t"			\
		".set	pop"			\
		: /* no output */		\
		: /* no input */		\
		: "memory")


#define __fast_iob()				\
	__asm__ __volatile__(			\
		".set	push\n\t"		\
		".set	noreorder\n\t"		\
		"lw	$0,%0\n\t"		\
		"nop\n\t"			\
		".set	pop"			\
		: /* no output */		\
		: "m" (*(int *)CKSEG1)		\
		: "memory")

# define fast_wmb()	__sync()
# define fast_rmb()	__sync()
# define fast_mb()	__sync()
# define fast_iob()				\
	do {					\
		__sync();			\
		__fast_iob();			\
	} while (0)



#define wmb()		fast_wmb()
#define rmb()		fast_rmb()
#define mb()		fast_mb()
#define iob()		fast_iob()


#define barrier()	__asm__ __volatile__("\n" : : :"memory")

#include "config.h"

#if defined(CONFIG_WEAK_ORDERING) && defined(CONFIG_SMP)
#  define smp_mb()	__asm__ __volatile__("sync" : : :"memory")
#  define smp_rmb()	__asm__ __volatile__("sync" : : :"memory")
#  define smp_wmb()	__asm__ __volatile__("sync" : : :"memory")
#else
#define smp_mb()	barrier()
#define smp_rmb()	barrier()
#define smp_wmb()	barrier()
#endif

#if defined(CONFIG_WEAK_REORDERING_BEYOND_LLSC) && defined(CONFIG_SMP)
#define __WEAK_LLSC_MB		"       sync	\n"
#else
#define __WEAK_LLSC_MB		"		\n"
#endif

#define set_mb(var, value) \
	do { var = value; smp_mb(); } while (0)

#define smp_llsc_mb()	__asm__ __volatile__(__WEAK_LLSC_MB : : :"memory")


#define smp_mb__before_llsc() smp_llsc_mb()
#define nudge_writes() mb()

#endif /* BARRIER_H */
