
#ifndef __SPINLOCK_H__
#define __SPINLOCK_H__

#include <linux/spinlock.h>

typedef arch_spinlock_t spinlock_t;
#define spinlock_lock(lock) 	arch_spin_lock(&(lock))
#define spinlock_unlock(lock) 	arch_spin_unlock(&(lock))
#define spinlock_trylock(lock) 	arch_spin_trylock(&(lock))

#define spinlock_init(lock)	ARCH_SPIN_LOCK_INIT(lock)


typedef arch_rwlock_t rwlock_t;

#define rw_read_lock(lock) 	arch_read_lock(&(lock))
#define rw_read_unlock(lock) 	arch_read_unlock(&(lock))
#define rw_read_trylock(lock) 	arch_read_trylock(&(lock))

#define rw_write_lock(lock) 	arch_write_lock(&(lock))
#define rw_write_unlock(lock) 	arch_write_unlock(&(lock))
#define rw_write_trylock(lock) 	arch_write_trylock(&(lock))

#define rwlock_init(lock)	ARCH_RW_LOCK_INIT(lock)

#endif
