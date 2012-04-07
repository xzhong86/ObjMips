 
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
