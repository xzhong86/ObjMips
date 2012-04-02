#include <test/test-head.h>
#include <string.h>
#include <linux/spinlock.h>

static struct lock_cnt {
	arch_rwlock_t rwlock;
	int counter;
} lcnt;

#define MSG_READ   1
#define MSG_WRITE  2
#define MSG_uREAD  3
#define MSG_uWRITE 4

#define TRY_TIMES 100
static void fun_slave(void)
{
	int exit,msg,i;
	exit = 0;
	do {
		smp_msg_receive(&msg);
		switch(msg) {
		case 0: exit = 1;
			break;
		case MSG_READ: 
			msg = 0;
			for(i=0; !msg && i<TRY_TIMES; i++)
				msg = arch_read_trylock(&lcnt.rwlock);
			smp_msg_send_other(msg);
			break;
		case MSG_WRITE:
			msg = 0;
			for(i=0; !msg && i<TRY_TIMES; i++)
				msg = arch_write_trylock(&lcnt.rwlock);
			smp_msg_send_other(msg);
			break;
		case MSG_uREAD: 
			arch_read_unlock(&lcnt.rwlock);
			//smp_msg_send_other(0);
			break;
		case MSG_uWRITE: 
			arch_write_unlock(&lcnt.rwlock);
			//smp_msg_send_other(0);
			break;
		default: break;
		}
	} while(!exit);
}
static int main_test(void)
{
	int step=0,msg;
#define FAIL_AT(exp)   step=__LINE__; if(exp) goto failed
    
	arch_read_lock(&lcnt.rwlock);    // read lock = 1
	arch_read_lock(&lcnt.rwlock);    // read lock = 2
	smp_msg_send_other(MSG_READ);
	smp_msg_receive(&msg);
	FAIL_AT(msg!=1);
	smp_msg_send_other(MSG_uREAD);
	smp_msg_send_other(MSG_WRITE);
	smp_msg_receive(&msg);
	FAIL_AT(msg!=0);
	arch_read_unlock(&lcnt.rwlock);    // read lock = 1
	smp_msg_send_other(MSG_READ);
	smp_msg_receive(&msg);
	FAIL_AT(msg!=1);
	smp_msg_send_other(MSG_uREAD);
	
	smp_msg_send_other(MSG_WRITE);
	smp_msg_receive(&msg);
	FAIL_AT(msg!=0);
	arch_read_unlock(&lcnt.rwlock);    // read lock = 0
	smp_msg_send_other(MSG_READ);
	smp_msg_receive(&msg);
	FAIL_AT(msg!=1);
	smp_msg_send_other(MSG_uREAD);

	//smp_msg_send_other(MSG_uREAD);
	smp_msg_send_other(MSG_WRITE);
	smp_msg_receive(&msg);
	FAIL_AT(msg!=1);
	smp_msg_send_other(MSG_uWRITE);


	arch_write_lock(&lcnt.rwlock);    // write lock = 1
	smp_msg_send_other(MSG_READ);
	smp_msg_receive(&msg);
	FAIL_AT(msg!=0);
	
	smp_msg_send_other(MSG_WRITE);
	smp_msg_receive(&msg);
	FAIL_AT(msg!=0);

	arch_write_unlock(&lcnt.rwlock);    // write lock = 0
	printk("rwlock0 test passed.\n");
	smp_msg_send_other(0); //exit
	return 0;

failed:
	printk("rwlock0 test failed. msg=%d at line %d\n",msg,step);
	smp_msg_send_other(0); //exit
	return -1;
}

static void push_slave(void)
{
	smp_fun_push(0,fun_slave);
}

int test_rwlock0(void)
{
	int ret;

	memset(&lcnt, 0, sizeof(lcnt));
	smp_ipi_func(0xff,smp_msg_clear);
	smp_ipi_func(0x2,push_slave);
	ret = main_test();
	smp_fun_wait(0x2);
	return ret;
}

DEF_TEST(test_rwlock0, 20, TESTFM_2ONCE);
