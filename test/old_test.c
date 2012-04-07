/* move out from main.c */
#include <smp_io.h>

int busy_loop(int loop)
{
	int i,j,sum=0;
	for(i=0;i<loop;i++)
		for(j=0;j<loop;j++)
			sum += j-i;
	return sum;
}
int div(int a,int b)
{
	printk("in fun div.\n");
	//print_c0_regs();
	asm("break 10");
	asm("break 11");
	asm("mfc0 $2,$13\nori $2,$2,0x0200\nmtc0 $2,$13\n");
	return a / b;
}
static int val;
static void jz_mem_test_one(
	unsigned int start,long len,int step)
{
	//unsigned int addr = 8400a2c0;
	unsigned int addr = start;
	unsigned int end  = addr + len;
	char *p = (char *)addr;
	printk("test addr: %p\n",p);
	while(p < (char*)end) {
		val += (*p) % 16;
		p += step / sizeof(*p);
	}
	printk("test end: %p\n",p);
}
static void jz_mem_test(void) 
{
	jz_mem_test_one(0x83000000,0x00800000,8);
	jz_mem_test_one(0x83800000,0x00800000,8);
	jz_mem_test_one(0x84000000,0x00800000,8);
	jz_mem_test_one(0x84800000,0x00800000,8);

	jz_mem_test_one(0xa3000000,0x00800000,8);
	jz_mem_test_one(0xa3800000,0x00800000,8);
	jz_mem_test_one(0xa4000000,0x00800000,8);
	jz_mem_test_one(0xa4800000,0x00800000,8);
}

int old_test(void)
{
	
	//div(5,0);
	//jz_mem_test();
	//cache_pref();
	//cache_probe();
	//tlb_pref();
	//tlb_pref1();
	//tlb_pref1();
	//usrmap_test();
	return 0;
}
