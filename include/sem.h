
#ifndef __SEM_H__
#define __SEM_H__

#include <thread.h>
#include <spinlock.h>
struct waitlist {
	int down;
	thread_t *thread;
	struct waitlist *next;
};
struct sem {
	int val;
	spinlock_t lock;
	struct waitlist *wait;
};
typedef struct sem sem_t;

int sem_init(sem_t *, int init);
/* if up <= 0; up all ! */
int sem_up(sem_t *, int up);
/* if down <= 0; error ! */
int sem_down(sem_t *, int down);
int sem_free(sem_t *);

#endif
