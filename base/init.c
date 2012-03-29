
#include <stdio.h>
#include <smp_io.h>
#include <init.h>

extern struct __initcall_entity __initcall_entity_start[];
extern struct __initcall_entity __initcall_entity_stop[];

void init_initcalls(void)
{
	struct __initcall_entity *calls;
	int num;
	int i, cur, next, ret;

	calls = __initcall_entity_start;
	num = __initcall_entity_stop - __initcall_entity_start;

	next = 0;
	do {
		cur = next;
		for (i = 0; i < num; i ++) {
			if (calls[i].fun && calls[i].level == cur) {
				ret = calls[i].fun();
				calls[i].fun = NULL;
				if (ret)
					printk("%s in %s return err %d\n",
					       calls[i].funname, calls[i].file, ret);
			}
			else if (calls[i].level > cur) {
				if (cur == next || next > calls[i].level)
					next = calls[i].level;
			}
		}
		if (cur == next)
			break;
	} while(1);
}
