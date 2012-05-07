#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <irq.h>
#include <thread.h>
#include <sem.h>

int sem_init(sem_t *sem, int init)
{
	if (!sem)
		return -1;
	memset(sem, 0, sizeof(*sem));
	sem->val = init;
	return 0;
}
int sem_up(sem_t *sem, int up)
{
	int flags;

	if (up <= 0)
		up = 0;
	local_irq_save(&flags);
	spinlock_lock(sem->lock);

	sem->val += up;
	while (sem->wait) {
		struct waitlist *w = sem->wait;
		if (sem->val >= w->down || !up) {
			sem->val -= w->down;
			sem->wait = w->next;
			thread_wakeup(w->thread);
			free(w);
		} else 
			break;
	}

	spinlock_unlock(sem->lock);
	local_irq_restore(&flags);
	return 0;
}
int sem_down(sem_t *sem, int down)
{
	int flags, sleep = 0;
	struct thread_task *cur;

	if (down <= 0)
		return -1;

	local_irq_save(&flags);
	spinlock_lock(sem->lock);

	cur = current_thread(smp_cpu_id());
	if (sem->val < down) {
		struct waitlist **wp;
		for (wp = &sem->wait; *wp; wp = &(*wp)->next) ;
		*wp = malloc(sizeof(struct waitlist));
		if (*wp) {
			cur->state = THREAD_STATE_SLEEP;
			(*wp)->down = down;
			(*wp)->thread = cur;
			(*wp)->next = NULL;
			sleep = 1;
		}
	} else {
		sem->val -= down;
	}

	spinlock_unlock(sem->lock);

	if (sleep) {
		cur->state = THREAD_STATE_SLEEP;
		thread_yield();
	}

	local_irq_restore(&flags);
	return 0;
}

int sem_free(sem_t *sem)
{
	if (sem->wait)
		return -1;
	return 0;
}
