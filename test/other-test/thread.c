
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
static int bad_addr(void *d)
{
	thread_t *cur = current_thread();
	printk("tid %d %s acc 0x4\n", cur->tid, cur->name);
	*(volatile int *)0x4;
	return 0;
}
static int test_thread(void)
{
	thread_t *thd;
	int i;
	for (i = 0; i < 5; i++)
		thread_create(thread, (void*)(1000+i));
	nr = i;
	while (nr) {
		printk("main yield %p\n", &i);
		thread_yield();
	}
	thread_yield();
	thd = thread_create(bad_addr, NULL);
	i = thread_wait(thd);
	printk("bad_addr thread exit with %d\n",i);
	return 0;
}
DEF_TEST(test_thread, 30, TESTF_REPEAT);
