 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <base.h>
#include <command.h>

static void write_words(unsigned int addr, int wds, unsigned int val)
{
	unsigned int *p = (unsigned int*)addr;

	while (wds --)
		*p ++ = val;
}
static void read_check(unsigned int addr, int wds, unsigned int val)
{
	unsigned int *p = (unsigned int*)addr;

	while (wds --) {
		if (*p != val)
			printk("* %p != %x\n", p, val);
		p ++;
	}
}

static void do_mem_help(char *str)
{
	printk("mem read [addr] [words] [check val]\n");
	printk("mem write [addr] [words] [val]\n");
}
static int do_mem(int argc,char *argv[])
{
	if (argc < 2) {
		do_mem_help(NULL);
		return 0;
	}

	if (argc == 5) {
		int rd = strcmp(argv[1],"read") == 0;
		int wt = strcmp(argv[1],"write") == 0;
		if (rd || wt) {
			unsigned int addr = strtoul(argv[2], NULL, 16);
			unsigned int wds = strtoul(argv[3], NULL, 16);
			unsigned int val = strtoul(argv[4], NULL, 16);
			
			printk("mem %s %x %x %x\n",argv[1],addr,wds,val);
			if (wt)
				write_words(addr, wds, val);
			else
				read_check(addr, wds, val);
		}
	}
	return 0;
}
DEF_COMMAND("mem",do_mem,do_mem_help);

#include <mem.h>
#include <pmon.h>
#include <test/test-head.h>
static void *t_mem;
static int t_size;
static int t_tmp;
static int mread(void)
{
	int i,tmp = 0,*p = t_mem;

	for (i = 0; i < t_size/4; i += 8, p += 8) {
		tmp ^= p[0]; tmp ^= p[1]; tmp ^= p[2]; tmp ^= p[3];
		tmp ^= p[4]; tmp ^= p[5]; tmp ^= p[6]; tmp ^= p[7];
	}
	t_tmp = tmp;
	return t_size;
}
static int mwrite(void)
{
	int i,*p = t_mem;

	for (i = 0; i < t_size/4; i += 8, p += 8) {
		p[0] = i; p[1] = i; p[2] = i; p[3] = i;
		p[4] = i; p[5] = i; p[6] = i; p[7] = i;
	}
	return t_size;
}
static int jread(void)
{
	int i,tmp = 0,*p = t_mem;

	for (i = 0; i < t_size/4; i += 16, p += 16) {
		tmp ^= p[0]; tmp ^= p[4]; tmp ^= p[8]; tmp ^= p[12];
	}
	t_tmp = tmp;
	return t_size;
}
static int jwrite(void)
{
	int i,*p = t_mem;

	for (i = 0; i < t_size/4; i += 16, p += 16) {
		p[0] = i; p[4] = i; p[8] = i; p[12] = i;
	}
	return t_size;
}
static int mset(void)
{
	memset(t_mem, 0x5a, t_size);
	return t_size;
}
static int mcopy(void)
{
	memcpy(t_mem, t_mem + t_size/2, t_size/2);
	return t_size/2;
}
static void test_case(int (*fun)(void), char *desc)
{
	unsigned long tmp,cyc,cycle = 0;
	int times = 0;
	unsigned long size = 0;

	pmon_prepare(PMON_EVENT_CYCLE);
	do {
		pmon_clear_cnt();
		pmon_start();

		size += fun();

		pmon_stop();
		pmon_get_cnt32(tmp,cyc);
		cycle += cyc;
		times ++;
		if (cycle > 400*1000000)
			break;
	} while(1);

	tmp = size / (cycle / 1000000);
	printk("%10s %5ld kB in 1 Mcycle. (total %ld kB, %ld cycle)\n",
	       desc, tmp/1024, size/1024, cycle);
}
static int memory(void)
{
	t_size = 8*1024*1024;
	t_mem = mem_alloc(t_size);
	test_case(mread, "seq read");
	test_case(mwrite, "seq write");
	test_case(jread, "j4w read");
	test_case(jwrite, "j4w write");
	test_case(mset, "std write");
	test_case(mcopy, "std copy");

	mem_free(t_mem);
	return 0;
}
DEF_TEST(memory, 30, TESTF_REPEAT);
