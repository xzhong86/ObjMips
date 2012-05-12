#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <base.h>
#include <mips.h>
#include <irq.h>
#include <mem.h>
#include <spinlock.h>
#include <thread.h>
#include <sem.h>

struct thread_task *__current_thread[CPU_MAX];

struct task {
	struct thread_task task;
	struct page *page;
	int nr_page;
	sem_t exit;
	struct task *next;
};
struct task_list {
	struct task *running, *sleep, *dead;
	struct task *idle[CPU_MAX];
	spinlock_t list_lock;
} task_list;

static unsigned int thread_id = CPU_MAX;
struct thread_task * 
__thread_create(thread_fun_t fun, void *data, const char *name,
		unsigned cpumask, int stack_size)
{
	int flags, nrpg;
	unsigned long vaddr;
	struct thread_head *head;
	struct task *t;

	if (stack_size) {
		if (stack_size < PAGE_SIZE || stack_size % PAGE_SIZE)
			return NULL;
	} else {
		stack_size = PAGE_SIZE * 2;
	}
	nrpg = stack_size / PAGE_SIZE;

	t = malloc(sizeof(*t));
	if (!t)
		return NULL;
	memset(t, 0, sizeof(*t));

	t->page = mem_get_pages(nrpg, MEM_LOW);
	if (!t->page) {
		free(t);
		return NULL;
	}
	t->nr_page = nrpg;
	sem_init(&t->exit, 0);

	vaddr = PHYS_TO_K0(mem_page2phy(t->page));
	head = (struct thread_head*)vaddr;

	t->task.head = head;
	head->task = &t->task;
	head->pgd  = current_thread_head()->pgd;
	head->flags = _TIF_CREATE;
	head->saved_sp = vaddr + stack_size - 16;

	if (!name)
		name = "noname";
	t->task.cp0_status = SR_CU0;
	strncpy(t->task.name, name, THREAD_NAME_LEN);
	t->task.name[THREAD_NAME_LEN - 1] = 0;
	t->task.tid = thread_id++;
	t->task.stack_size = stack_size;
	t->task.stack_top = vaddr + stack_size;
	t->task.cpumask = cpumask;
	t->task.fun = fun;
	t->task.data = data;
	t->task.flags = 0;
	t->task.state = THREAD_STATE_RUNNING;
	t->next = NULL;

	local_irq_save(&flags);
	spinlock_lock(task_list.list_lock);

	t->next = task_list.running;
	task_list.running = t;

	spinlock_unlock(task_list.list_lock);
	local_irq_restore(&flags);

	return &t->task;
}

static void __clean_dead(struct task_list *tl, thread_t *thd)
{
	struct task **pp;
	for (pp = &tl->dead; *pp; pp = &(*pp)->next) {
		struct task *t;
		if (&(*pp)->task != thd)
			continue;
		t = *pp;
		*pp = t->next;
		mem_free_pages(t->page, t->nr_page);
		free(t);
		break;
	}
}
static void clean_thread(thread_t *thd)
{
	int flags;

	local_irq_save(&flags);
	spinlock_lock(task_list.list_lock);

	__clean_dead(&task_list, thd);

	spinlock_unlock(task_list.list_lock);
	local_irq_restore(&flags);
}

extern void thread_switch(struct thread_head *cur,struct thread_head *next);
void thread_yield(void)
{
	int flags, cpu;
	struct task *cur, *next;
	struct task **pp;

	local_irq_save(&flags);
	spinlock_lock(task_list.list_lock);

	cpu = smp_cpu_id();
	cur = container_of(current_thread(), struct task, task);

	next = task_list.running;
	if (!next) {
		next = task_list.idle[cpu];
		task_list.idle[cpu] = NULL;
	} else {
		task_list.running = next->next;
	}
	if (!next) {
		printk("No thread to switch!\n");
		goto out;
	}

	switch (cur->task.state) {
	case THREAD_STATE_RUNNING: pp = &task_list.running; break;
	case THREAD_STATE_SLEEP:   pp = &task_list.sleep; break;
	case THREAD_STATE_IDLE:    pp = &task_list.idle[cpu]; break;
	case THREAD_STATE_EXIT:
	default:  pp = &task_list.dead; break;
	}
	for ( ; *pp; pp = &(*pp)->next) ;
	*pp = cur;
	cur->next = NULL;

	cur->task.cp0_status = read_c0_status();
	cur->task.cp0_cause  = read_c0_cause();
	cur->task.cp0_epc    = read_c0_epc();

	__current_thread[cpu] = &next->task;
	thread_switch(cur->task.head, next->task.head);

	cpu = smp_cpu_id();
	write_c0_status(current_thread()->cp0_status);
	write_c0_cause(current_thread()->cp0_cause);
	write_c0_epc(current_thread()->cp0_epc);

out:
	spinlock_unlock(task_list.list_lock);
	local_irq_restore(&flags);
}
void thread_exit(int err)
{
	struct thread_task *cur;
	struct task *t;

	cur = current_thread();
	cur->exit_code = err;
	cur->state = THREAD_STATE_EXIT;
	if (err)
		printk("thread %s exit %d.\n", cur->name, err);
	if (cur->tid < CPU_MAX) {
		printk("thread %s(%d) should NOT exit.\n", 
		       cur->name, cur->tid);
		return;
	}
	t = container_of(cur, struct task, task);
	sem_up(&t->exit, 0);
	sem_free(&t->exit);
	thread_yield();

	*(volatile int*)1;		/* BUG */
}
void new_thread_init(void)
{
	struct thread_task *cur;
	int ret;

	cur = current_thread();
	thread_clear_cur(TIF_CREATE);

	write_c0_status(cur->cp0_status);
	write_c0_cause(cur->cp0_cause);

	spinlock_unlock(task_list.list_lock);
	irq_enable();

	/* run thread ! */
	ret = cur->fun(cur->data);

	thread_exit(ret);
}

int thread_wait(struct thread_task *thd)
{
	int ret = -1;
	struct task *waitt;

	waitt = container_of(thd, struct task, task);
	sem_down(&waitt->exit, 1);

	ret = thd->exit_code;
	clean_thread(thd);
	return ret;
}
int thread_wakeup(struct thread_task *thd)
{
	int flags, ret = -1;
	struct task *t;
	struct task **pp;

	local_irq_save(&flags);
	spinlock_lock(task_list.list_lock);

	for (pp = &task_list.sleep; *pp; pp = &(*pp)->next)
		if (&(*pp)->task == thd)
			break;
	if (*pp) {
		t = *pp;
		*pp = t->next;
		t->next = task_list.running;
		task_list.running = t;
		t->task.state = THREAD_STATE_RUNNING;
		ret = 0;
	} else {
		if (thd && thd->state != THREAD_STATE_EXIT) {
			thd->state = THREAD_STATE_RUNNING;
			ret = 0;
		}
	}

	spinlock_unlock(task_list.list_lock);
	local_irq_restore(&flags);
	return ret;
}

void cpu_idle_loop(void)
{
	struct thread_task *cur;

	cur = current_thread();
	do {
		if (!task_list.running)
			cpu_wait();
		else {
			cur->state = THREAD_STATE_IDLE;
			thread_yield();
		}
	} while(1);
}

#include <pcpu.h>
extern struct pgd *current_pgd[CPU_MAX];
int thread_init(void)
{
	struct task *t;
	struct thread_head *head;
	int cpu = smp_cpu_id();

	t = malloc(sizeof(*t));
	head = malloc(sizeof(*head));

	memset(t, 0, sizeof(*t));
	memset(head, 0, sizeof(*head));

	t->task.head = head;
	head->task = &t->task;

	head->pgd = current_pgd[cpu];

	strcat(t->task.name, "idle0");
	t->task.name[4] += cpu;
	t->task.tid = cpu;
	t->task.cpumask = 1 << cpu;
	t->task.state = THREAD_STATE_RUNNING;
	t->task.stack_size = __SMP_SIZE;
	t->task.stack_top = PCPU_BASE(cpu) + __SMP_SIZE;

	__current_thread[cpu] = &t->task;
	__current_thread_head = head;
	return 0;
}
