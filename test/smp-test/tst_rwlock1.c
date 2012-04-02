#include <test/test-head.h>
#include <string.h>
#include <linux/spinlock.h>

static struct lock_cnt {
	arch_rwlock_t rwlock;
	int counter;
} lcnt;

static int loop_fun(int *arr,int seed)
{
	struct lock_cnt *lc = &lcnt;
	int i,j,r;
	volatile int cnt;
	pcpu_srand(seed);
	r = pcpu_rand();
	for(i=0; i<TIMES*LOOPS; i++) {
		arch_write_lock(&lc->rwlock);
		cnt = ++lc->counter;
		arch_write_unlock(&lc->rwlock);
		if(i%LOOPS == LOOPS-1) {
			r= pcpu_rand();
			arr[i/LOOPS] = cnt;
		}
		r ^= (r>>1) ^ i;
		for(j=0; j<r%4; j++) {
			arch_read_lock(&lc->rwlock);
			cnt = lc->counter;
			arch_read_unlock(&lc->rwlock);
		}
	}
	return 0;
}
static void fun_slave(void)
{
	int msg;
	int *arr;
	arr = (int*)pcpu_malloc(TIMES*sizeof(int));
	smp_msg_send_other(1);
	smp_msg_receive(&msg);
	loop_fun(arr,2);
	smp_msg_send_other(1);
	pcpu_free(arr);
}
static void push_slave(void)
{
	smp_fun_push(0,fun_slave);
}

static int main_test(void)
{
	int msg,ret = 0;
	int *arr;
	arr = (int*)pcpu_malloc(TIMES*sizeof(int));
	smp_msg_receive(&msg);
	smp_msg_send(0xff,1);
	smp_msg_receive(&msg);
	loop_fun(arr,1);
	smp_msg_receive(&msg);
	PRINTF("rwlock add %d times, counter=%d\n"
	       ,2*TIMES*LOOPS,lcnt.counter);
	if(2*TIMES*LOOPS != lcnt.counter)
		ret = -1;
	
	pcpu_free(arr);
	return ret;
}

int test_rwlock1(void)
{
	int ret;

	memset(&lcnt, 0, sizeof(lcnt));
	smp_ipi_func(0xff,smp_msg_clear);
	smp_ipi_func(0x2,push_slave);
	ret = main_test();
	smp_fun_wait(0x2);
	return ret;
}

DEF_TEST(test_rwlock1, 20, TESTFM_2LOOP);
