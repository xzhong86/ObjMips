
#ifndef __SPINLOCK_H__
#define __SPINLOCK_H__

#include <linux/spinlock.h>

typedef arch_spinlock_t spinlock_t;
#define spinlock_lock(lock) 	arch_spin_lock(&(lock))
#define spinlock_unlock(lock) 	arch_spin_unlock(&(lock))
#define spinlock_trylock(lock) 	arch_spin_trylock(&(lock))

#define spinlock_init(lock)	ARCH_SPIN_LOCK_INIT(lock)

#endif
