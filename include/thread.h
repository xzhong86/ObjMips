
#ifndef __THREAD_H__
#define __THREAD_H__


#ifndef __ASSEMBLY__

typedef struct thread_task thread_t;

#include <signal.h>

typedef int (*thread_fun_t)(void *);
struct thread_head;
struct thread_task {
#define THREAD_NAME_LEN	16
	char name[THREAD_NAME_LEN];
	unsigned int tid;

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

	struct thread_head *head;
	unsigned long cp0_epc, cp0_badvaddr;
	unsigned long cp0_status, cp0_cause;

	/* signal */
	struct signal {
		unsigned long mask, pending;
		struct signal_action actions[SIG_MAX];
	} signal;

	/* relationship */
	struct thread_task *father,*child,*brother;
	struct {
		struct thread_task *leader, *next;
	} group;

	thread_fun_t fun;
	void *data;
};

struct pgd;
/* this struction may be accessed in assembler */
struct thread_head {
	unsigned long pad[2];	/* used in asm */
	unsigned long flags;
	unsigned long saved_sp;
	struct pgd *pgd;		/* used in tlb miss */
	struct thread_task	*task;	/* thread structure */
};
#else
/* Only used in assembler */
#define HEAD_REG	k1
#define THREAD_PAD0	0
#define THREAD_PAD1	4
#define THREAD_FLAGS	8
#define THREAD_SP	12
#define THREAD_PGD	16

#endif

#define TIF_CREATE	0
#define TIF_PENDING	1
#define TIF_RESCHED	2

#define _TIF_CREATE	(1 << TIF_CREATE)
#define _TIF_PENDING	(1 << TIF_PENDING)
#define _TIF_RESCHED	(1 << TIF_RESCHED)



#ifndef __ASSEMBLY__

#include <smp.h>
extern struct thread_task *__current_thread[CPU_MAX];

/* we used k1 here, change it must take care. */
register struct thread_head * __current_thread_head __asm__("$27");
#define current_thread_head() __current_thread_head
#define current_thread()  (__current_thread_head->task)

#include <bitops.h>
#define thread_test_flag(T,flg)	 test_bit((flg), &(T)->head->flags)
#define thread_set_flag(T,flg)	 set_bit((flg), &(T)->head->flags)
#define thread_clear_flag(T,flg) clear_bit((flg), &(T)->head->flags)

#define thread_test_cur(flg)	 test_bit((flg), &__current_thread_head->flags)
#define thread_set_cur(flg)	 set_bit((flg), &__current_thread_head->flags)
#define thread_clear_cur(flg)	 clear_bit((flg), &__current_thread_head->flags)


struct thread_task * 
__thread_create(thread_fun_t fun, void *data, const char *name, 
		unsigned cpumask, int stack_size);
/* default: allow all cpu, use default stack size */
#define thread_create(F, D)				\
	__thread_create((F), (D), #F, (unsigned)-1, 0);
#define thread_create_name(F, D, N)			\
	__thread_create((F), (D), (N), (unsigned)-1, 0);

/* called before 'thread' have a child */
void thread_new_group(thread_t *thd);

void thread_exit_group(int err);

/* release cpu for other thread */
void thread_yield(void);

/* wait for thread exit, return exit code */
int thread_wait(struct thread_task *);
/* wakeup thread, return 0 if success */
int thread_wakeup(struct thread_task *);

/* special exit for thread */
void thread_exit(int);

#endif

#endif
