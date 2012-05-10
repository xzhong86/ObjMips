
#ifndef __COMMAND_H__
#define __COMMAND_H__

#ifndef __used
# define __used	__attribute__((__used__))
#endif
#ifndef __section
# define __section(S) __attribute__ ((__section__(#S)))
#endif
#define __command_section __section(.data.command_entity)

typedef int (*command_fun_t)(int,char**);
typedef void (*command_help_t)(char *);

struct __command_entity {
	const char *name;
	command_fun_t fun;
	command_help_t help;
	unsigned int hash;
};

#include <config.h>

#ifdef CONFIG_CUI
int register_command(char *name,command_fun_t fun);
int unregister_command(char *name);
#else
static inline int register_command(char *name,command_fun_t fun)
{
	return -1;
}
static inline int unregister_command(char *name)
{
	return -1;
}
#endif

int do_command(int argc,char **argv);
int complete_command(char *str);

#define DEF_COMMAND(NAME,FUN,HELP)					\
	static struct __command_entity __e_##fun __used __command_section = { \
		.name = NAME,						\
		.fun = FUN,						\
		.help = HELP,						\
	}


#endif
