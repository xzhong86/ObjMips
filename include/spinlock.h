
#ifndef __SPINLOCK_H__
#define __SPINLOCK_H__

#include <linux/spinlock.h>

typedef arch_spinlock_t spinlock_t;
#define spin_lock(lock) 	arch_spin_lock(lock)
#define spin_unlock(lock) 	arch_spin_unlock(lock)

#endif
