#include <smp_io.h>
#include <mem.h>
#include <mips.h>
#include <stdlib.h>

#define USR_BASE 0x10000000
#define TO_USR(addr) ((typeof(addr))(K0_TO_PHYS(addr) + USR_BASE))
int usrmap_test(void)
{
	int *p,*up;
	int i,e,size = 4*4096;

	p = (int*)malloc(size);
	up = TO_USR(p);

	e = add_mem_range((unsigned long)p, size, (unsigned)up, PG_NORMAL);
	printk("User map test %p %d @ %p %d\n",p,size,up,e);
	
	printk("set 11111111 in k0\n");
	for (i = 0; i < size/4; i++)
		p[i] = 11111111 + i;

	printk("check in user ...");
	for (i = 0,e = 0; i < size/4; i++)
		e += up[i] != 11111111 + i;
	printk(" %d error\n",e);

	//mmu_dump_tlb();

	printk("set 22222222 in usr\n");
	for (i = 0; i < size/4; i++)
		up[i] = 22222222 + i;

	printk("check in k0 ...");
	for (i = 0,e = 0; i < size/4; i++)
		e += p[i] != 22222222 + i;
	printk(" %d error\n",e);

	remove_mem_range((unsigned)up);
	printk("free %p\n",p);
	free(p);
	return 0;
}
