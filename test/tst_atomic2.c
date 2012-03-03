#include "test-head.h"
#include <linux/atomic.h>

static __inline__ void atomic_add_r(int i, atomic_t * v)
{
	int temp;

	__asm__ __volatile__(
		"	.set	mips3					\n"
		"1:	ll	%0, %1		# atomic_add		\n"
		"	addu	%0, %2					\n"
		"	sc	%0, %1					\n"
		"	beqz	%0, 1b					\n"
		: "=&r" (temp), "=m" (v->counter)
		: "Ir" (i), "m" (v->counter)
		);
	
}

static atomic_t counter;

#undef  TIMES
#define TIMES CALC_TEST(100)
#define GEAR_TIMES 10
static inline void gear_add0(atomic_t *cntp)
{
	atomic_add(1,cntp);	/* 1 */
/* 2 */ asm("nop;nop;");
	atomic_add(1,cntp);	/* 2 */
/* 2 */ asm("nop;nop;");
	atomic_add(1,cntp);	/* 3 */
/* 9 */ asm("nop;nop;nop;nop;nop;nop;nop;nop;nop;");
	atomic_add(1,cntp);	/* 4 */
/* 3 */ asm("nop;nop;nop;");
	atomic_add(1,cntp);	/* 5 */
/* 2 */ asm("nop;nop;");
	atomic_add(1,cntp);	/* 6 */
/* 9 */ asm("nop;nop;nop;nop;nop;nop;nop;nop;nop;");
	atomic_add(1,cntp);	/* 7 */
/* 2 */ asm("nop;nop;");
	atomic_add(1,cntp);	/* 8 */
/* 10*/ asm("nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;");
	atomic_add(1,cntp);	/* 9 */
/* 2 */ asm("nop;nop;");
	atomic_add(1,cntp);	/* 10 */
/* 2 */ asm("nop;nop;");
	return ;
}
static inline void gear_add1(atomic_t *cntp)
{
	atomic_add(1,cntp);	/* 1 */
/* 2 */ asm("nop;nop;");
	atomic_add(1,cntp);	/* 2 */
/* 2 */ asm("nop;nop;");
	atomic_add(1,cntp);	/* 3 */
/* 2 */ asm("nop;nop;");
	atomic_add(1,cntp);	/* 4 */
/* 2 */ asm("nop;nop;");
	atomic_add(1,cntp);	/* 5 */
/* 2 */ asm("nop;nop;");
	atomic_add(1,cntp);	/* 6 */
/* 10*/ asm("nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;");
	atomic_add(1,cntp);	/* 7 */
/* 2 */ asm("nop;nop;");
	atomic_add(1,cntp);	/* 8 */
/* 2 */ asm("nop;nop;");
	atomic_add(1,cntp);	/* 9 */
/* 2 */ asm("nop;nop;");
	atomic_add(1,cntp);	/* 10 */
/* 2 */ asm("nop;nop;");
	return ;
}
static void loop_fun0(int *arr)
{
	int i,j;
	for(i=0; i<TIMES; i++) {
		for(j=0; j<LOOPS; j++) 
			gear_add0(&counter);
		arr[i] = counter.counter;
	}
}
static void loop_fun1(int *arr)
{
	int i,j;
	for(i=0; i<TIMES; i++) {
		for(j=0; j<LOOPS; j++) 
			gear_add1(&counter);
		arr[i] = counter.counter;
	}
}
static void fun_slave(void)
{
	int msg;
	int *arr;
	arr = (int*)pcpu_malloc(TIMES*sizeof(int));
	smp_msg_send_other(1);
	smp_msg_receive(&msg);
	loop_fun1(arr);
	smp_msg_send_other(1);	
	pcpu_free(arr);
}
static void push_slave(void)
{
	smp_fun_push(0,fun_slave);
}

static int fun_main(void)
{
	int msg,ret = 0;
	int *arr;
	arr = (int*)pcpu_malloc(TIMES*sizeof(int));
	smp_msg_receive(&msg);
	smp_msg_send(0xff,1);
	smp_msg_receive(&msg);
	loop_fun0(arr);
	smp_msg_receive(&msg);
	PRINTF("atomic add %d times, counter=%d\n"
	       ,2*TIMES*LOOPS*GEAR_TIMES,counter.counter);
	if(2*TIMES*LOOPS*GEAR_TIMES != counter.counter)
		ret = -1;
	
	pcpu_free(arr);
	return ret;
}

int test_atomic2(void)
{
	int ret;

	counter.counter = 0;
	smp_ipi_func(0xff,smp_msg_clear);
	smp_ipi_func(0x2,push_slave);

	ret = fun_main();
	smp_fun_wait(0x2);
	return ret;
}

DEF_TEST(test_atomic2, 10, TESTFM_2LOOP);
