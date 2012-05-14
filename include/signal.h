
#ifndef __SIGNAL_H__
#define __SIGNAL_H__

#define SIGEXIT		4
#define SIGTERM		15  /* Termination (ANSI).  */

#define SIG_MAX		32

typedef void (*signal_fun_t)(int);

struct signal_action {
	signal_fun_t fun;
};

#include <thread.h>

int signal_setup(thread_t *thread);
int signal_release(thread_t *thread);

int signal_mask(int sig);
int signal_unmask(int sig);
int signal_set_action(int sig, signal_fun_t fun);

int signal_send(thread_t *thread,int sig);
int do_signal(void);

#endif
