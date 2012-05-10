
#ifndef __THREAD_H__
#define __THREAD_H__

typedef int (*thread_fun_t)(void *);
struct thread_head;
struct thread_task {
#define THREAD_NAME_LEN	16
	char name[THREAD_NAME_LEN];
	unsigned int tid;

#define THREAD_CREATE	0x1
	unsigned long flags;
	enum {
		THREAD_STATE_RUNNING,THREAD_STATE_SLEEP,
		THREAD_STATE_EXIT,
		THREAD_STATE_IDLE, /* special state for idle thread */
	} state;
	unsigned int cpumask;
	unsigned int stack_size;
	unsigned int stack_top;
	int exit_code;

	struct thread_head *thread;
	unsigned long cp0_epc, cp0_badvaddr;
	unsigned long cp0_status, cp0_cause;

	thread_fun_t fun;
	void *data;
};
typedef struct thread_task thread_t;

struct thread_head {
	struct thread_task	*task;	/* thread structure */
	struct pt_regs		*regs;
	unsigned long flags;
	unsigned long saved_sp;
};

#include <smp.h>
extern struct thread_task *__current_thread[CPU_NR];
#define current_thread(cpu)  (__current_thread[(cpu)])
#define current_thread_head()  (current_thread(smp_cpu_id())->thread)

struct thread_task * 
__thread_create(thread_fun_t fun, void *data, const char *name, 
		unsigned cpumask, int stack_size);
/* default: allow all cpu, use default stack size */
#define thread_create(F, D)				\
	__thread_create((F), (D), #F, (unsigned)-1, 0);
#define thread_create_name(F, D, N)			\
	__thread_create((F), (D), (N), (unsigned)-1, 0);

/* release cpu for other thread */
void thread_yield(void);

/* wait for thread exit, return exit code */
int thread_wait(struct thread_task *);
/* wakeup thread, return 0 if success */
int thread_wakeup(struct thread_task *);

/* special exit for thread */
void thread_exit(int);


#endif
