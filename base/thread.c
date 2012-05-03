#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <base.h>
#include <mips.h>
#include <irq.h>
#include <mem.h>
#include <spinlock.h>
#include <thread.h>

struct thread_task *__current_thread[CPU_NR];

struct task {
	struct thread_task task;
	struct page *page;
	int nr_page;
	struct task *next;
};
struct task_list {
	struct task *running, *sleep, *dead;
	rwlock_t list_lock;
} task_list;

int __thread_create(thread_fun_t fun, void *data, const char *name,
		    unsigned cpumask, int stack_size)
{
	int flags, nrpg;
	unsigned long vaddr;
	struct thread_head *thread;
	struct task *t;

	if (stack_size) {
		if (stack_size < PAGE_SIZE || stack_size % PAGE_SIZE)
			return -1;
	} else {
		stack_size = PAGE_SIZE * 2;
	}
	nrpg = stack_size / PAGE_SIZE;

	t = malloc(sizeof(*t));
	if (!t)
		return -1;
	memset(t, 0, sizeof(*t));

	t->page = mem_get_pages(nrpg, MEM_LOW);
	if (!t->page) {
		free(t);
		return -1;
	}
	t->nr_page = nrpg;
	vaddr = PHYS_TO_K0(mem_page2phy(t->page));
	thread = (struct thread_head*)vaddr;

	t->task.thread = thread;
	thread->task = &t->task;
	thread->regs = NULL;
	thread->flags = 0;
	thread->saved_sp = vaddr + stack_size - 16;

	if (!name)
		name = "noname";
	t->task.cp0_status = SR_CU0;
	strncpy(t->task.name, name, THREAD_NAME_LEN);
	t->task.name[THREAD_NAME_LEN - 1] = 0;
	t->task.cpumask = cpumask;
	t->task.fun = fun;
	t->task.data = data;
	t->task.flags |= THREAD_CREATE;
	t->task.state = THREAD_STATE_RUNNING;
	t->next = NULL;

	local_irq_save(&flags);
	rw_write_lock(task_list.list_lock);

	t->next = task_list.running;
	task_list.running = t;

	rw_write_unlock(task_list.list_lock);
	local_irq_restore(&flags);

	return 0;
}

extern void thread_switch(struct thread_head *cur,struct thread_head *next,
			  int create);
void thread_yield(void)
{
	int flags, cpu;
	struct task *cur, *next;
	struct task **pp;

	local_irq_save(&flags);
	rw_write_lock(task_list.list_lock);

	cpu = smp_cpu_id();
	cur = container_of(current_thread(cpu), struct task, task);

	next = task_list.running;
	if (!next) {
		printk("No running thread to switch!\n");
		goto out;
	}
	task_list.running = next->next;

	switch (cur->task.state) {
	case THREAD_STATE_RUNNING: pp = &task_list.running; break;
	case THREAD_STATE_SLEEP:   pp = &task_list.sleep; break;
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
	thread_switch(cur->task.thread, next->task.thread,
		      next->task.flags & THREAD_CREATE);

	cpu = smp_cpu_id();
	write_c0_status(current_thread(cpu)->cp0_status);
	write_c0_cause(current_thread(cpu)->cp0_cause);
	write_c0_epc(current_thread(cpu)->cp0_epc);

out:
	rw_write_unlock(task_list.list_lock);
	local_irq_restore(&flags);
}

void new_thread_init(void)
{
	struct thread_task *cur;
	int ret;

	cur = current_thread(smp_cpu_id());
	cur->flags &= ~THREAD_CREATE;

	write_c0_status(cur->cp0_status);
	write_c0_cause(cur->cp0_cause);

	rw_write_unlock(task_list.list_lock);
	irq_enable();

	/* run thread ! */
	ret = cur->fun(cur->data);
	if (ret)
		printk("thread %s exit %d.\n", cur->name, ret);

	cur->state = THREAD_STATE_EXIT;
	thread_yield();

	*(volatile int*)1;		/* BUG */
}

int thread_init(void)
{
	struct task *t;
	struct thread_head *head;

	t = malloc(sizeof(*t));
	head = malloc(sizeof(*head));

	memset(t, 0, sizeof(*t));
	memset(head, 0, sizeof(*head));

	t->task.thread = head;
	head->task = &t->task;

	strncpy(t->task.name, "idle0", THREAD_NAME_LEN);
	t->task.name[THREAD_NAME_LEN - 1] = 0;
	t->task.cpumask = 1;
	t->task.state = THREAD_STATE_RUNNING;

	__current_thread[smp_cpu_id()] = &t->task;
	return 0;
}
