
#ifndef __INIT_H__
#define __INIT_H__


#ifndef __used
# define __used	__attribute__((__used__))
#endif
#ifndef __section
# define __section(S) __attribute__ ((__section__(#S)))
#endif
#define __initcall_section __section(.data.initcall_entity)


typedef int (*initcall_t)(void);
struct __initcall_entity {
	const char *file;
	const char *funname;
	unsigned int level;
	initcall_t fun;
	struct __initcall_entity *next;
};

#define initcall(FUN, LVL)						\
	static struct __initcall_entity __e_##FUN __used __initcall_section = { \
		.file = __FILE__,					\
		.funname = #FUN,					\
		.fun = FUN,						\
		.level = LVL,						\
	}

/* LVL followed can be -99 ~ 99 */
#define base_initcall(FUN, LVL)	initcall(FUN, (LVL) + 100)
#define soft_initcall(FUN, LVL)	initcall(FUN, (LVL) + 200)
#define dev_initcall(FUN, LVL)	initcall(FUN, (LVL) + 300)

#define test_initcall(FUN, LVL)	initcall(FUN, (LVL) + 1100)
#define fw_initcall(FUN, LVL)	initcall(FUN, (LVL) + 1300)

#endif
