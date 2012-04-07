#include <test/test-head.h>

#define LL(var)  ({						\
			unsigned int res;			\
			__asm__ __volatile__(			\
				"ll %0,%1":"=r"(res):"m"(var)	\
				:"memory");			\
			res;					\
		})
#define SC(val,var)  ({						\
			unsigned int res;			\
			__asm__ __volatile__(			\
				"sc %2,%1\n\tmove %0,%2"	\
				:"=r"(res),"=m"(var):"r"(val)	\
				:"memory");			\
			res;					\
		})
#define ADD(var,i)  ({						\
			unsigned int res;			\
			__asm__ __volatile__(			\
				"ll %0,%1\n\taddiu %0,%0,%2\n"	\
				"\tsc %0,%1"			\
				:"=r"(res),"=m"(var):"i"(i)	\
				:"memory");			\
			res;					\
		})


static int base_test(void)
{
	int x,y,z,tmp;
	x=0; y=0; z=0;
	tmp = LL(x);
	tmp++;
	tmp = SC(tmp,x);
	printk("[LL/SC base 1] x:%d, tmp:%d\n",x,tmp);
	if(x==0 || tmp==0) return -1;

	int a,b;
	x=0;
	a = LL(x);
	a ++;
	b = LL(x);
	b += 2;
	b = SC(b,x);
	a = SC(a,x);
	printk("[LL/SC base 2] x:%d, a:%d, b:%d\n",x,a,b);
//	if(b==0 || a!=0 || x!=2) return -1;
	if(b==0 || a!=0 || x!=2) 
		printk("[LL/SC base 2] error\n");

	int t0,t1,t2;
	x=0;
	t0 = LL(x);
	t0 += 1;
	t1 = LL(y);
	t1 += 2;
	t2 = LL(z);
	t2 += 3;
	t2 = SC(t2,z);
	t1 = SC(t1,y);
	t0 = SC(t0,x);
	printk("[LL/SC base 3] x:%d, y:%d, z:%d\n",x,y,z);
	if(t2==0 || t1!=0 || t0!=0 || x!=0 || y!=0 || z!=3)
		return -1;

	printk("[LL/SC base] test passed!\n");
	return 0;
}


#ifdef CONFIG_SMP
static int cnt,cnt2;
static void fun_slave(void)
{
	int msg,tmp,i;
	smp_msg_send_other(1);  //  1>
	smp_msg_receive(&msg);  // <2

	tmp = 0;
	for(i=0;i<1000;i++)
		tmp += ADD(cnt,1);
	if(tmp>7) tmp = 7;
	smp_msg_send_other(tmp);//  3>
	smp_msg_receive(&msg);  // <4

	tmp = 0;
	for(i=0;i<1000;i++)
		tmp = ADD(cnt2,1);
	if(tmp>7) tmp = 7;
	smp_msg_send_other(tmp);//  5>
	//smp_msg_receive(&msg);  // <6
}
static void fun_main(void)
{
	int msg,ret;
	smp_msg_receive(&msg);  // <1
	ret = LL(cnt);
	smp_msg_send_other(1);  //  2>
	ret = SC(ret,cnt);
	smp_msg_receive(&msg);  // <3
	printk("same addr msg=%d,ret=%d\n",msg,ret);
	if(ret || !msg) goto fail;

	ret = LL(cnt);
	smp_msg_send_other(1);  //  4>
	ret = SC(ret,cnt);
	smp_msg_receive(&msg);  // <5
	printk("diff addr msg=%d,ret=%d\n",msg,ret);
	if(ret || !msg) goto fail;
	smp_msg_send(0xff,0);
	return;
fail:
	smp_msg_send(0xff,1);
	return;
}
static void push_slave(void)
{
	smp_fun_push(0,fun_slave);
}
static void push_main(void)
{
	smp_fun_push(0,fun_main);
}
static int smp_test(void)
{
	int msg;
	smp_ipi_func(0xff,smp_msg_clear);
	smp_ipi_func(0x2,push_slave);
	smp_ipi_func(0x1,push_main);
	while(smp_fun_run() == 0)
		;
	smp_msg_receive(&msg);
	return msg? -1: 0;
}
#endif

int test_llsc(void)
{
	int ret;
	ret = base_test();
	if(ret) return ret;
#ifdef CONFIG_SMP
	ret = smp_test();
	smp_fun_wait(0x2);
#endif
	return ret;
}

DEF_TEST(test_llsc, 10, TESTFM_2ONCE);
