 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <smp_io.h>
#include <command.h>

#include <test/test-head.h>

extern struct test_entity __test_entity_start[];
extern struct test_entity __test_entity_stop[];

static struct test_entity *tests;
static int tests_num;

static int run_one_test(struct test_entity *t)
{
	int ret;
	printk("[TEST] run %s() ...\n",t->funname);
	ret = t->fun();
	if(ret) {
		printk("[ERR] %s() failed! %d\n",t->funname,ret);
		printk("[ERR] defined in %s:%d.\n",t->filename,t->line);
	}
	return ret;
}
static void show_all_test(void)
{
	int i;
	printk("test entity: %d @ %p\n",tests_num,tests);
	for (i = 0; i < tests_num; i++) {
		struct test_entity *p = &tests[i];
		printk("%2d %3d %8x %s %s\n", i, p->priority,
		       p->flags, p->funname, p->filename);
	}
}
static int main_test_all(void)
{
	int i,ret,err = 0;

	show_all_test();
	for (i = 0; i < tests_num; i++) {
		struct test_entity *p = &tests[i];

		if (p->priority > 0) {
			if (!(p->flags & TESTF_REPEAT))
				p->priority = -1;
		} else {
			continue;
		}

		ret = run_one_test(p);
		if(ret) 
			err ++;
	}
	if (err) {
		printk("[ERR] %d tests failed!\n",err);
	}
	return 0;
}

static inline unsigned int get_str_hash(const char *str)
{
	unsigned int seed = 1313131; // 31 131 1313 13131 131313 etc..
	unsigned int hash = 0;
	while (*str)
		hash = hash * seed + (*str++);
	return (hash & 0x7FFFFFFF);
}
static struct test_entity *get_test_by_name(const char *name)
{
	static int get_hash = 0;
	unsigned int i, hash;

	if (!get_hash) {
		for (i = 0; i < tests_num; i++)
			tests[i].hash = get_str_hash(tests[i].funname);
		get_hash = 1;
	}
	
	hash = get_str_hash(name);
	for (i = 0; i < tests_num; i++) {
		if (hash == tests[i].hash && 
		    strcmp(name, tests[i].funname) == 0)
			return &tests[i];
	}
	return NULL;
}

static int test_special(const char *names[], int num)
{
	struct test_entity *t;
	int i;

	for (i = 0; i < num; i++) {
		t = get_test_by_name(names[i]);
		if (!t) {
			printk("bad test name %s\n",names[i]);
			continue;
		}
		run_one_test(t);
	}
	return 0;
}
static int test_loop(const char *name, int loop)
{
	int ret = 0, i;
	struct test_entity *t = get_test_by_name(name);

	if (!t)
		return -1;

	printk("[TEST] run test loop %d\n", loop);
	for (i = 0; i < loop; i++) {
		ret += run_one_test(t) != 0;
	}
	return ret;
}

static const char *tfuns[] = {
	"test_atomic0",
	"test_atomic1",
	"test_atomic2",
	"test_atomic3",
	"test_spinlock0",
	"test_spinlock1",
	"test_rwlock0",
	"test_rwlock1",
	"test_cache0",
	"test_cache1",
	"test_cache2",
	"test_cache3",
	"test_cache4s",
	"test_cache4l",
	"test_cache4",
	"test_spinlock3_uc7w",
	"test_spinlock3_uc7w2",
	"test_spinlock3_uc7w3",
	"test_spinlock3_7w",
	"test_spinlock2_15w",
	"test_spinlock2_31w",
	"test_spinlock2_5w",
	"test_spinlock2",
//	"test_mixed",
};
int run_tests(void)
{
	tests = __test_entity_start;
	tests_num = __test_entity_stop - __test_entity_start;

	test_loop("test_cache4s", 2);
	test_loop("test_cache4l", 2);
	test_loop("test_cache4", 2);
	test_loop("test_spinlock3_uc7w", 1);
	test_loop("test_spinlock3_uc7w2", 1);
	test_loop("test_spinlock3_uc7w", 1);
	test_loop("test_spinlock3_uc7w3", 1);
	test_loop("test_spinlock3_uc7w2", 1);
	test_loop("test_spinlock3_uc7w", 1);
	test_loop("test_spinlock3_uc7w2", 1);
	test_loop("test_spinlock3_uc7w3", 1);
	test_loop("test_spinlock3_7w", 5);
	test_loop("test_spinlock2_15w", 5);
	test_loop("test_spinlock2_31w", 5);

	test_loop("test_spinlock2", 5);
	test_loop("test_spinlock2_5w", 5);
#if 0
	test_loop("test_llsc", 5);
	test_loop("test_smp_msg", 5);
	test_loop("test_spinlock0", 10);
	test_loop("test_rwlock0", 10);
	test_loop("test_cache0", 10);
	test_loop("test_cache2", 10);
	test_loop("test_cache3", 10);
	test_loop("test_smp_msg", 5);
	test_loop("test_llsc", 5);
#endif
	if (0) {
		main_test_all();
		test_special(tfuns, sizeof(tfuns)/sizeof(tfuns[0]));
	}
	return 0;
}


static void do_test_help(char *str)
{
	printk("test all\ntest special\ntest show\n");
	printk("test cache\n");
	printk("test testname [times]\n");
}
extern int cache_probe(void);
static int do_test(int argc,char *argv[])
{
	if (argc < 2) {
		do_test_help(NULL);
		return 0;
	}

	if (! tests) {
		tests = __test_entity_start;
		tests_num = __test_entity_stop - __test_entity_start;
	}

	if (argc >= 2 && strcmp(argv[1],"all") == 0)
		main_test_all();
	else if (argc >= 2 && strcmp(argv[1],"special") == 0)
		test_special(tfuns, sizeof(tfuns)/sizeof(tfuns[0]));
	else if (argc >= 2 && strcmp(argv[1],"show") == 0)
		show_all_test();
	else if (argc >= 2 && strcmp(argv[1],"cache") == 0)
		cache_probe();
	else if (argc >= 3) {
		int loop;
		struct test_entity *t;
		t = get_test_by_name(argv[1]);
		if (! t) {
			printk("No such test %s\n",argv[1]);
			return -1;
		}
		loop = strtoul(argv[2],NULL,10);
		test_loop(argv[1],loop);
	}
	return 0;
}
DEF_COMMAND("test",do_test,do_test_help);
