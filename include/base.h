
#ifndef __BASE_H__
#define __BASE_H__

int cache_init(void);
int mmu_init(void);

extern void cpu_wait(void);

#undef offsetof
#ifdef __compiler_offsetof
#define offsetof(TYPE,MEMBER) __compiler_offsetof(TYPE,MEMBER)
#else
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})

#define ALIGN_TO(var, align) (((var)+(align)-1)&~((align)-1))

#ifndef __used
# define __used	__attribute__((__used__))
#endif
#ifndef __section
# define __section(S) __attribute__ ((__section__(#S)))
#endif
#define __setup_section __section(.data.setup_entity)

typedef void (*setup_fun_t)(char*);
struct __setup_entity {
	const char *name;
	setup_fun_t fun;
	unsigned int hash;
};

#define __setup(NAME,FUN)						\
	static struct __setup_entity __e_##FUN __used __setup_section = { \
		.name = NAME,						\
		.fun = FUN,						\
	}


#endif
