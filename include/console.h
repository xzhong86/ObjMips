
#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <config.h>

struct console {
	int (*getchar)(void);
	void (*putchar)(const char);
	void (*putstring)(const char *);
};

#ifdef CONFIG_CUI
int register_console(struct console *con);
struct console *get_defconsole(void);
#else
static inline int register_console(struct console *con)
{
	return 0;
}
static inline struct console *get_defconsole(void)
{
	return 0;
}
#endif


#endif
