
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <base.h>
#include <iomap.h>
#include <mem.h>
#include <command.h>

static void do_reg_help(char *str)
{
	printk("reg rd/wt/and/or addr [~][val]\n");
	printk("reg map paddr\n");
	printk("reg unmap vaddr\n");
}
static int do_reg(int argc,char *argv[])
{
	unsigned int addr, val = 0;
	unsigned int old, new;

	if (argc < 3) {
		do_reg_help(NULL);
		return 0;
	}

	addr = strtoul(argv[2], NULL, 16);
	if (argc > 3) {
		char * str = argv[3];
		int not = str[0] == '~';
		if (not) str ++;
		val = strtoul(str, NULL, 16);
		if (not) val = ~val;
	}

	if (argc == 3 && strcmp(argv[1],"map") == 0) {
		void *vaddr = ioremap(addr, 0x4);
		if (!vaddr) {
			printk("ioremap %x failed.\n",addr);
			return -1;
		} else {
			printk("ioremap %x to %p.\n",addr,vaddr);
			return 0;
		}
	}
	else if (argc == 3 && strcmp(argv[1],"unmap") == 0) {
		iounmap((void*)addr);
	}

	addr &= ~0x3;
	if (!mem_get_phy((void*)addr)) {
		printk("access unmapped addr %x\n", addr);
		return -1;
	}

	old = *(unsigned long*)addr;
	if (strcmp(argv[1],"rd") == 0) {
		printk("%08x: %08x\n", addr, old);
		return 0;
	}
	else if (argc == 4 && strcmp(argv[1],"wt") == 0)
		new = val;
	else if (argc == 4 && strcmp(argv[1],"and") == 0)
		new = old & val;
	else if (argc == 4 && strcmp(argv[1],"or") == 0)
		new = old | val;
	else {
		do_reg_help(NULL);
		return -1;
	}
	*(unsigned long*)addr = new;
	printk("%08x: %08x => %08x\n", addr, old, new);
	return 0;
}
DEF_COMMAND("reg",do_reg,do_reg_help);
