#include "test-head.h"
#include <stdlib.h>
#include <cache.h>
#include <linux/barrier.h>

static volatile int tmp;
static void rush_dcache(unsigned addr,int len, int ways)
{
	int i;
	while (ways--) {
		int *p;
		addr += CFG_DCACHEWAY_SIZE;
		p = (int*)addr;
		for (i = 0; i < len/4; i++)
			tmp = p[i];
	}
}

static int test_case1(char *buf)
{
	char *p0,*p1;
	blast_dcache();	
	p0 = (char*)(((unsigned)buf+4096*4) & (~(CFG_CACHELINE_SIZE-1)));
	p1 = K0_TO_K1(p0);
	p1[0] = 0;
	p1[1] = 0;
	barrier();

	p0[0] = 1;
	barrier();
	p1[1] = 2;
	barrier();

	flush_dcache_range((unsigned)p0,(unsigned)p0 + CFG_CACHELINE_SIZE);
	//rush_dcache((unsigned)p0, 16, 160);
	printk("%d %d  %d %d\n",p0[0],p0[1],p1[0],p1[1]);

	if (p0[0] != 1 || p0[1] != 2)
		ncsim_error_addr(p0,p1,(void*)(unsigned)p0[0],
				 (void*)(unsigned)p0[1]);
	return p0[0] != 1 || p0[1] != 2;
}

static int test_one_int(int *p)
{
	int ret,*p1;
	//blast_dcache();
	flush_dcache_range((unsigned)(p-1),(unsigned)(p+1));
	//cache_unroll(p-1, Hit_Writeback_Inv_D);
	//cache_unroll(p+1, Hit_Writeback_Inv_D);
	//__asm__ volatile ("sync");
	p1 = K0_TO_K1(p);
	p1[-1] = 0;
	p1[0] = 0;
	p1[1] = 0;
	barrier();
	p[0] = 2;
	barrier();
	p1[-1] = 1;
	p1[0] = 1;
	p1[1] = 1;
	//blast_dcache();	
	flush_dcache_range((unsigned)(p-1),(unsigned)(p+1));
	//cache_unroll(p, Hit_Writeback_Inv_D);
	//__asm__ volatile ("sync");
	ret = p[-1] != 1 || p[0] != 2 || p[1] != 1 ;
	if (ret)
		printk("%p: %d %d %d\n",p,p[-1],p[0],p[1]);
	return ret;
}
#define L2SIZE (512*1024)
static int test_case2(char *buf)
{
	int ret,*p;
	p = (int*)(((unsigned)buf + L2SIZE -1) & ~(L2SIZE - 1));
	ret  = test_one_int(p-1);
	ret += test_one_int(p);
	ret += test_one_int(p+7);
	ret += test_one_int(p+8);
	ret += test_one_int(p+1023);
	ret += test_one_int(p+1024);
	ret += test_one_int(p+8193);
	ret += test_one_int(p+8192);
	return ret;
}

int test_cache4s(void)
{
	int ret = 0;
	char *buf;

	buf = (char*)malloc(2*1024*1024);
	ret += test_case1(buf);
	ret += test_case2(buf);
	free(buf);
	return ret;
}

DEF_TEST(test_cache4s, 10, (TESTF_REPEAT));
