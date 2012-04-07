#include <test/test-head.h>
#include <cache.h>
 
#define ARR_NUM (256)
static struct share_data {
	char char_arr[ARR_NUM];
	short sht_arr[ARR_NUM];
	int int_arr[2];
} sh;

static void fun_slave(void)
{
	int i,j,msg;
	smp_msg_send_other(1);
	smp_msg_receive(&msg);
	for(j=0;j<100;j++)
		for(i=0;i<ARR_NUM;i++) {
			if(i%2 == 1) {
				sh.char_arr[i]--;
				sh.sht_arr[i]--;
			}
			sh.int_arr[1] --;
		}
	smp_msg_send_other(1);
	smp_msg_receive(&msg);
	unsigned int start = (unsigned int)&sh;
	unsigned int end = start + sizeof(sh);
	flush_dcache_range(start,end);
	smp_msg_send_other(1);
}
static void push_slave(void)
{
	smp_fun_push(0,fun_slave);
}

static int main_test(void)
{
	int i,j,msg,ret = 0;
	smp_msg_receive(&msg);
	smp_msg_send(0xff,1);  // send to all
	smp_msg_receive(&msg);
	for(j=0;j<100;j++)
		for(i=0;i<ARR_NUM;i++) {
			if(i%2 == 0) {
				sh.char_arr[i]++;
				sh.sht_arr[i]++;
			}
			sh.int_arr[0] ++;
		}
	smp_msg_receive(&msg);
	smp_msg_send_other(1);
	unsigned int start = (unsigned int)&sh;
	unsigned int end = start + sizeof(sh);
	flush_dcache_range(start,end);
	smp_msg_receive(&msg);
	ret = 0;
	for(i=0;i<ARR_NUM;i++) {
		if(i%2 == 0) {
			if(sh.char_arr[i] != 100){
				printk("sh.char_arr[%d]:%d.\n",i,sh.char_arr[i]);
				ret++;
			}
			if(sh.sht_arr[i] != 100){
				printk("sh.sht_arr[%d]:%d.\n",i,sh.sht_arr[i]);
				ret++;
			}
			//	ret += sh.char_arr[i] != 100;
			//	ret += sh.sht_arr[i] != 100;
		} else {
			if(sh.char_arr[i] != -100){
				printk("sh.char_arr[%d]:%d.\n",i,sh.char_arr[i]);
				ret++;
			}
			if(sh.sht_arr[i] != -100){
				printk("sh.sht_arr[%d]:%d,address: %p.\n",i,sh.sht_arr[i],(void *)&sh.sht_arr[i]);
				ret++;
			}
			//	ret += sh.char_arr[i] != -100;
			//	ret += sh.sht_arr[i] != -100;
		}
	}
	if(sh.int_arr[0] != 100*ARR_NUM ){
		printk("sh.int_arr[0]:%d.\n",sh.int_arr[0]);
		ret++;
	}
	if(sh.int_arr[1] != -100*ARR_NUM ){
		printk("sh.int_arr[1]:%d.\n",sh.int_arr[1]);
		ret++;
	}
//	ret += sh.int_arr[0] != 100*ARR_NUM;
//	ret += sh.int_arr[1] != -100*ARR_NUM;
	if(ret) {
		printk("cache0 test failed, ret:%d\n",ret);
		return -1;
	}
	
	printk("cache0 test passed!\n");
	return 0;
}

static void flush_cache(void)
{
	unsigned int start = (unsigned int)&sh;
	unsigned int end = start + sizeof(sh);
	flush_dcache_range(start,end);
	return;
}

int test_cache0(void)
{
	int ret,i;

	for(i=0;i<ARR_NUM;i++){
		sh.char_arr[i]=0;
		sh.sht_arr[i]=0;
	}
	sh.int_arr[0]=0;
	sh.int_arr[1]=0;

	
//	smp_ipi_func(0xff,flush_cache);
	//flush_cache();
	smp_ipi_func(0x3,flush_cache);
	smp_ipi_func(0x3,smp_msg_clear);
	
       
	smp_ipi_func(0x2,push_slave);
	ret = main_test();
	smp_fun_wait(0x2);
	return ret;
}

DEF_TEST(test_cache0, 10, (TESTF_REPEAT | TESTF_BIG | TESTF_2CORE));
