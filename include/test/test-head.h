
#include <stdio.h>

#include <base.h>
#include <pcpu.h>
#include <smp.h>
#include <smp_msg.h>
#include <smp_fun.h>


/* TEST_SCALE could control test scale */
#define TEST_SCALE	128
#define TEST_SCALE_MAX	128
/* CALC_TEST can get test scale from expect value */
#define CALC_TEST(exp)  ((exp) * TEST_SCALE / TEST_SCALE_MAX)

#define TIMES CALC_TEST(1000)
#define LOOPS CALC_TEST(1024)

#define CPUS  RUNNING_CPUS
//#define CPUS  (2)


#ifndef __used
# define __used	__attribute__((__used__))
#endif
#ifndef __section
# define __section(S) __attribute__ ((__section__(#S)))
#endif
#define __test_entity __section(.data.test_entity)

typedef int (*test_fun_t)(void);
struct test_entity {
	char *filename;
	int line;
	char *funname;
	test_fun_t fun;
	int priority;
	unsigned int flags;
	unsigned int hash;
};
#define TESTF_REPEAT	0x0001	/* test could repeat */
#define TESTF_SCALE	0x0002	/* could control test scale */
#define TESTF_BIG	0x0004	/* it's a big test(loop) */
#define TESTF_2BIG	0x0008	/* it's a very big test, run less */
#define TESTF_DEVICE	0x0010	/* device test */
#define TESTF_2CORE	0x0100	/* only use 2-core */
#define TESTF_MCORE	0x0200	/* can use multi-core */

/* multi flag macro, offten used */
#define TESTFM_2ONCE (TESTF_REPEAT | TESTF_2CORE)
#define TESTFM_MONCE (TESTF_REPEAT | TESTF_MCORE)
#define TESTFM_2LOOP (TESTF_REPEAT | TESTF_SCALE | TESTF_BIG | TESTF_2CORE)
#define TESTFM_MLOOP (TESTF_REPEAT | TESTF_SCALE | TESTF_BIG | TESTF_MCORE)

#define __DEF_TEST_ENT(fn,pri,flg,F,L)					\
	static struct test_entity __e_##fn __used __test_entity = {	\
		.filename = F,						\
		.line = L,						\
		.funname = #fn,						\
		.fun = fn,						\
		.priority = pri,					\
		.flags = flg,						\
	}

#define DEF_TEST(fn,priority,flags)				\
	__DEF_TEST_ENT(fn,priority,flags,__FILE__,__LINE__)


