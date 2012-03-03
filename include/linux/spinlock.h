
#ifndef SPINLOCK_H
#define SPINLOCK_H

#undef __BIG_ENDIAN
typedef union {
	/*
	 * bits  0..15 : serving_now
	 * bits 16..31 : ticket
	 */
	unsigned int lock;
	struct {
#ifdef __BIG_ENDIAN
		unsigned short ticket;
		unsigned short serving_now;
#else
		unsigned short serving_now;
		unsigned short ticket;
#endif
	} h;
} arch_spinlock_t;
#define ARCH_SPIN_LOCK_INIT(lk)	   do { (lk).lock = 0; } while(0)

typedef struct {
	volatile unsigned int lock;
} arch_rwlock_t;
#define ARCH_RW_LOCK_INIT(rwlk)	   do { (rwlk).lock = 0; } while(0)


#include "linux/barrier.h"

/*
 * Your basic SMP spinlocks, allowing only a single CPU anywhere
 *
 * Simple spin lock operations.  There are two variants, one clears IRQ's
 * on the local processor, one does not.
 *
 * These are fair FIFO ticket locks
 *
 * (the type definitions are in asm/spinlock_types.h)
 */

static inline void arch_spin_lock(arch_spinlock_t *lock)
{
	int my_ticket;
	int tmp;
	int inc = 0x10000;

	__asm__ __volatile__ (
		"	.set push		# arch_spin_lock	\n"
		"	.set noreorder					\n"
		"							\n"
		"1:	ll	%[ticket], %[ticket_ptr]		\n"
		"	addu	%[my_ticket], %[ticket], %[inc]		\n"
		"	sc	%[my_ticket], %[ticket_ptr]		\n"
		"	beqz	%[my_ticket], 1b			\n"
		"	 srl	%[my_ticket], %[ticket], 16		\n"
		"	andi	%[ticket], %[ticket], 0xffff		\n"
		"	andi	%[my_ticket], %[my_ticket], 0xffff	\n"
		"	bne	%[ticket], %[my_ticket], 4f		\n"
		"	 subu	%[ticket], %[my_ticket], %[ticket]	\n"
		"2:							\n"
		"	.subsection 2					\n"
		"4:	andi	%[ticket], %[ticket], 0x1fff		\n"
		"	sll	%[ticket], 5				\n"
		"							\n"
		"6:	bnez	%[ticket], 6b				\n"
		"	 subu	%[ticket], 1				\n"
		"							\n"
		"	lhu	%[ticket], %[serving_now_ptr]		\n"
		"	beq	%[ticket], %[my_ticket], 2b		\n"
		"	 subu	%[ticket], %[my_ticket], %[ticket]	\n"
		"	b	4b					\n"
		"	 subu	%[ticket], %[ticket], 1			\n"
		"	.previous					\n"
		"	.set pop					\n"
		: [ticket_ptr] "+m" (lock->lock),
		  [serving_now_ptr] "+m" (lock->h.serving_now),
		  [ticket] "=&r" (tmp),
		  [my_ticket] "=&r" (my_ticket)
		: [inc] "r" (inc)
		);
	
	smp_llsc_mb();
}

static inline void arch_spin_unlock(arch_spinlock_t *lock)
{
	unsigned int serving_now = lock->h.serving_now + 1;
	wmb();
	lock->h.serving_now = (unsigned short)serving_now;
	nudge_writes();
}

/* return 1 when successed and 0 when failed. */
static inline unsigned int arch_spin_trylock(arch_spinlock_t *lock)
{
	int tmp, tmp2, tmp3;
	int inc = 0x10000;

	__asm__ __volatile__ (
		"	.set push		# arch_spin_trylock	\n"
		"	.set noreorder					\n"
		"							\n"
		"1:	ll	%[ticket], %[ticket_ptr]		\n"
		"	srl	%[my_ticket], %[ticket], 16		\n"
		"	andi	%[my_ticket], %[my_ticket], 0xffff	\n"
		"	andi	%[now_serving], %[ticket], 0xffff	\n"
		"	bne	%[my_ticket], %[now_serving], 3f	\n"
		"	 addu	%[ticket], %[ticket], %[inc]		\n"
		"	sc	%[ticket], %[ticket_ptr]		\n"
		"	beqz	%[ticket], 1b				\n"
		"	 li	%[ticket], 1				\n"
		"2:							\n"
		"	.subsection 2					\n"
		"3:	b	2b					\n"
		"	 li	%[ticket], 0				\n"
		"	.previous					\n"
		"	.set pop					\n"
		: [ticket_ptr] "+m" (lock->lock),
		  [ticket] "=&r" (tmp),
		  [my_ticket] "=&r" (tmp2),
		  [now_serving] "=&r" (tmp3)
		: [inc] "r" (inc)
		);
	
	smp_llsc_mb();

	return tmp;
}



/*
 * Read-write spinlocks, allowing multiple readers but only one writer.
 *
 * NOTE! it is quite common to have readers in interrupts but no interrupt
 * writers. For those circumstances we can "mix" irq-safe locks - any writer
 * needs to get a irq-safe write-lock, but readers can get non-irqsafe
 * read-locks.
 */


static inline void arch_read_lock(arch_rwlock_t *rw)
{
	unsigned int tmp;

	__asm__ __volatile__(
		"	.set	noreorder	# arch_read_lock	\n"
		"1:	ll	%1, %2					\n"
		"	bltz	%1, 2f					\n"
		"	 addu	%1, 1					\n"
		"	sc	%1, %0					\n"
		"	beqz	%1, 1b					\n"
		"	 nop						\n"
		"	.subsection 2					\n"
		"2:	ll	%1, %2					\n"
		"	bltz	%1, 2b					\n"
		"	 addu	%1, 1					\n"
		"	b	1b					\n"
		"	 nop						\n"
		"	.previous					\n"
		"	.set	reorder					\n"
		: "=m" (rw->lock), "=&r" (tmp)
		: "m" (rw->lock)
		: "memory"
		);

	smp_llsc_mb();
}

/* Note the use of sub, not subu which will make the kernel die with an
   overflow exception if we ever try to unlock an rwlock that is already
   unlocked or is being held by a writer.  */
static inline void arch_read_unlock(arch_rwlock_t *rw)
{
	unsigned int tmp;

	smp_mb__before_llsc();

	__asm__ __volatile__(
		"	.set	noreorder	# arch_read_unlock	\n"
		"1:	ll	%1, %2					\n"
		"	sub	%1, 1					\n"
		"	sc	%1, %0					\n"
		"	beqz	%1, 2f					\n"
		"	 nop						\n"
		"	.subsection 2					\n"
		"2:	b	1b					\n"
		"	 nop						\n"
		"	.previous					\n"
		"	.set	reorder					\n"
		: "=m" (rw->lock), "=&r" (tmp)
		: "m" (rw->lock)
		: "memory"
		);
}

static inline void arch_write_lock(arch_rwlock_t *rw)
{
	unsigned int tmp;

	__asm__ __volatile__(
		"	.set	noreorder	# arch_write_lock	\n"
		"1:	ll	%1, %2					\n"
		"	bnez	%1, 2f					\n"
		"	 lui	%1, 0x8000				\n"
		"	sc	%1, %0					\n"
		"	beqz	%1, 2f					\n"
		"	 nop						\n"
		"	.subsection 2					\n"
		"2:	ll	%1, %2					\n"
		"	bnez	%1, 2b					\n"
		"	 lui	%1, 0x8000				\n"
		"	b	1b					\n"
		"	 nop						\n"
		"	.previous					\n"
		"	.set	reorder					\n"
		: "=m" (rw->lock), "=&r" (tmp)
		: "m" (rw->lock)
		: "memory"
		);

	smp_llsc_mb();
}

static inline void arch_write_unlock(arch_rwlock_t *rw)
{
	smp_mb();

	__asm__ __volatile__(
	"				# arch_write_unlock	\n"
	"	sw	$0, %0					\n"
	: "=m" (rw->lock)
	: "m" (rw->lock)
	: "memory");
}

static inline int arch_read_trylock(arch_rwlock_t *rw)
{
	unsigned int tmp;
	int ret;

	__asm__ __volatile__(
		"	.set	noreorder	# arch_read_trylock	\n"
		"	li	%2, 0					\n"
		"1:	ll	%1, %3					\n"
		"	bltz	%1, 2f					\n"
		"	 addu	%1, 1					\n"
		"	sc	%1, %0					\n"
		"	beqz	%1, 1b					\n"
		"	 nop						\n"
		"	.set	reorder					\n"
		__WEAK_LLSC_MB
		"	li	%2, 1					\n"
		"2:							\n"
		: "=m" (rw->lock), "=&r" (tmp), "=&r" (ret)
		: "m" (rw->lock)
		: "memory"
		);

	return ret;
}

static inline int arch_write_trylock(arch_rwlock_t *rw)
{
	unsigned int tmp;
	int ret;

	__asm__ __volatile__(
		"	.set	noreorder	# arch_write_trylock	\n"
		"	li	%2, 0					\n"
		"1:	ll	%1, %3					\n"
		"	bnez	%1, 2f					\n"
		"	lui	%1, 0x8000				\n"
		"	sc	%1, %0					\n"
		"	beqz	%1, 3f					\n"
		"	 li	%2, 1					\n"
		"2:							\n"
		__WEAK_LLSC_MB
		"	.subsection 2					\n"
		"3:	b	1b					\n"
		"	 li	%2, 0					\n"
		"	.previous					\n"
		"	.set	reorder					\n"
		: "=m" (rw->lock), "=&r" (tmp), "=&r" (ret)
		: "m" (rw->lock)
		: "memory"
		);

	return ret;
}

#endif
