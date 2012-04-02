
#include <stdio.h>
#include <stdlib.h>
#include <smp_io.h>
#include <test/test-head.h>

#include <mips_inst.h>
#include <cache.h>
static int build(void * addr)
{
	unsigned int i, *p = addr;

	for (i = 0; i < 62; i++) {
		p[i] = INST_ADDIU(4, 4, i+1);
		p[i+64] = INST_ADDIU(4, 4, i+1);
	}
	p[62] = INST_JR(31);
	p[63] = 0;
	p[62+64] = INST_JR(31);
	p[63+64] = 0;
	flush_dcache_range((unsigned)p, (unsigned)&p[128]);
	flush_icache_range((unsigned)p, (unsigned)&p[128]);
	return 0;
}
static int check(void * addr)
{
	unsigned int i, *p = addr;

	for (i = 0; i < 64; i++) {
		if (p[i] != p[i+64])
			printk("p[%d](%x) != %x\n",
			       i, p[i], p[i+64]);
	}
	return 0;
}
static int test_addr(void *addr)
{
	typedef int (*sum_fun_t)(int);
	int (*parr)[1024];
	sum_fun_t funs[256];
	int i, j, sum = 0;

	for (i = 0; i < 256; i++) {
		build(addr + 4096*i);
		funs[i] = addr + 4096*i;
	}

	parr = addr + 256*4096;
	for (i = 0; i < 256; i++) 
		for (j = 1; j < 64; j++)
			parr[i][j] = j;

	for (i = 0; i < 256*100; i++) {
		int r, tmp;
		sum_fun_t sum_fun;

		r = rand();
		sum_fun = funs[r%256];
		tmp = sum_fun(sum);
		r /= 256;
		if (tmp != sum + 31*63 ||
		    rand() % 16 == 0)
			check(sum_fun);
		sum = tmp;

		tmp = r % 256;
		for (j = 2; j < 64; j++) {
			if (parr[tmp][1]*j != parr[tmp][j])
				printk("parr[%d][%d](%d) != %d\n",
				       tmp, j, parr[tmp][j], parr[tmp][1]*j);
			parr[tmp][j] += j;
		}
		parr[tmp][1] += 1;
	}
	return 0;
}
static int test_case(void *addr, char arr[], 
		     void (*f_dcache)(unsigned,unsigned))
{
	unsigned int start = (unsigned)addr;
	char *p = addr;
	int i;

	for (i = 0; i < 256; i++)
		p[i] = 0;
	f_dcache(start, start+i);
	for (i = 1; i < 256; i+=2)
		if (arr[i])
			p[i] = arr[i];
	f_dcache(start, start+i);
	for (i = 0; i < 256; i++)
		if (p[i] != arr[i]) {
			printk("p[%d](%d) != %d\n",i,p[i],arr[i]);
			break;
		}
	return i != 256;
}

static int is_prime(int i)
{
	int j;
	if (i < 4)
		return i > 1;
	for (j = 2; j <= i/2; j++)
		if (i % j == 0)
			return 0;
	return 1;
}
static void rush_dcache(unsigned a,unsigned b)
{
	int i;
	blast_icache();
	for (i = 0; i < 512*1024; i += 32)
		*(volatile char*)(0x80000000+i);
}
static int test_strob(void *addr)
{
	void (*f_dcache)(unsigned,unsigned);
	char arr[256];
	int i;

	f_dcache = flush_dcache_range;
again:
	for (i = 0; i < 256; i++)
		arr[i] = i%2;
	test_case(addr, arr, f_dcache);
	for (i = 0; i < 256; i++)
		arr[i] = is_prime(i)? i/2: 0;
	test_case(addr, arr, f_dcache);
	for (i = 0; i < 256; ) {
		arr[i++] = 1; arr[i++] = 0; arr[i++] = 1; arr[i++] = 0;
		arr[i++] = 1; arr[i++] = 1; arr[i++] = 0; arr[i++] = 0;
	}
	test_case(addr, arr, f_dcache);
	for (i = 2; i < 256; i*=2) {
		int j;
		for (j = 0; j < 256; j++) 
			arr[j] = rand()%i == 0? 0xa: 0;
		test_case(addr, arr, f_dcache);
	}

	if (f_dcache == flush_dcache_range) {
		f_dcache = rush_dcache;
		goto again;
	}
	return 0;
}

int test_ddr(void)
{
	test_addr((void*)0x82000000);
	test_strob((void*)0x82000000);
	return 0;
}

DEF_TEST(test_ddr, 10, TESTF_REPEAT);
