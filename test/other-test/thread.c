
#include <base.h>
#include <thread.h>
#include <test/test-head.h>

static volatile int nr;
static int thread(void *d)
{
	int i;
	for (i = 0; i < 10; i++) {
		printk("thread %d %p\n",(int)d,&i);
		thread_yield();
	}
	nr --;
	return (int)d;
}

static int test_thread(void)
{
	int i;
	for (i = 0; i < 5; i++)
		thread_create(thread, (void*)(1000+i));
	nr = i;
	while (nr) {
		printk("main yield %p\n", &i);
		thread_yield();
	}
	thread_yield();
	return 0;
}
DEF_TEST(test_thread, 30, TESTF_REPEAT);
