

#include <thread.h>

static void kill_group(int sig)
{
	thread_exit_group(-1);
}
static void exit_self(int sig)
{
	thread_exit(-sig);
}
int signal_setup(thread_t *thread)
{
	struct signal *sigp = &thread->signal;

	sigp->actions[SIGTERM].fun = kill_group;
	sigp->actions[SIGEXIT].fun = exit_self;
	return 0;
}
int signal_release(thread_t *thread)
{
	thread->signal.pending = 0;
	thread_clear_flag(thread, TIF_PENDING);
	return 0;
}

int signal_mask(int sig)
{
	if (sig < 0 || sig >= SIG_MAX)
		return -1;
	current_thread()->signal.mask |= 1 << sig;
	return 0;
}
int signal_unmask(int sig)
{
	if (sig < 0 || sig >= SIG_MAX)
		return -1;
	current_thread()->signal.mask &= ~(1 << sig);
	return 0;
}
int signal_set_action(int sig, signal_fun_t fun)
{
	if (sig < 0 || sig >= SIG_MAX || !fun)
		return -1;

	current_thread()->signal.mask &= ~(1 << sig);
	current_thread()->signal.actions[sig].fun = fun;
	return 0;
}

int signal_send(thread_t *thread, int sig)
{
	struct signal *sigp = &thread->signal;

	if (sig < 0 || sig >= SIG_MAX)
		return -1;
	sigp->pending |= 1 << sig;
	if (sigp->pending & ~sigp->mask)
		thread_set_flag(thread, TIF_PENDING);
	return 0;
}

int do_signal(void)
{
	struct signal *sigp = &current_thread()->signal;
	unsigned long work;

	work = sigp->pending & ~sigp->mask;
	while (work) {
		int sig = fls(work) - 1;
		if (sigp->actions[sig].fun)
			sigp->actions[sig].fun(sig);
		sigp->pending &= ~(1 << sig);
		work = sigp->pending & ~sigp->mask;
	}
	thread_clear_cur(TIF_PENDING);
	return 0;
}
