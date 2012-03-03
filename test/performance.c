#include <stdio.h>
#include <smp_io.h>
#include <time.h>
#include <pmon.h>
#include <cache.h>

int cache_pref(void)
{
	volatile unsigned int x;
	unsigned int *base0 = (unsigned int*)0x80000000;
	unsigned int *base1 = base0 + CFG_DCACHE_SIZE / 4;
	volatile unsigned int *hreg = (unsigned int*)0xB00020e8;
	unsigned int cnt[4],icc[4],dcc[4];
	int num = CFG_DCACHE_SIZE / 4;
	int inc = CFG_CACHELINE_SIZE / 4;
	int i;

	pmon_prepare(PMON_EVENT_CACHE);
	pmon_start();

	for(i=0;i<num;i+=inc) x = base0[i];

	cnt[0] = ost_read_counter();
	pmon_get_cnt32(icc[0],dcc[0]);

	for(i=0;i<num;i+=inc) x = base0[i];

	cnt[1] = ost_read_counter();
	pmon_get_cnt32(icc[1],dcc[1]);

	for(i=0;i<num;i+=inc) x = base1[i];

	cnt[2] = ost_read_counter();
	pmon_get_cnt32(icc[2],dcc[2]);

	for(i=0;i<num;i+=inc) x = hreg[0];

	cnt[3] = ost_read_counter();
	pmon_stop();
	pmon_get_cnt32(icc[3],dcc[3]);

	PRINTF("cache preformance: cache_pref()\n");
	PRINTF("loop times: %d, insts: %d, cpu cyc: %d\n",num/inc,5,num*5/inc);
	PRINTF("stage \ticc m\tdcc m\tcpu cyc\n");
	PRINTF("stage 1\t%d\t%d\t%d\n",icc[1]-icc[0],dcc[1]-dcc[0]
	       ,(cnt[1]-cnt[0])*4);
	PRINTF("stage 2\t%d\t%d\t%d\n",icc[2]-icc[1],dcc[2]-dcc[1]
	       ,(cnt[2]-cnt[1])*4);
	PRINTF("stage 3\t%d\t%d\t%d\n",icc[3]-icc[2],dcc[3]-dcc[2]
	       ,(cnt[3]-cnt[2])*4);
	int inc_cyc = ((cnt[2]-cnt[1]) - (cnt[1]-cnt[0]))*4;
	int inc_ccm = (icc[2]-icc[1]+dcc[2]-dcc[1]) - (icc[1]-icc[0]+dcc[1]-dcc[0]);
	float rate = (float)inc_cyc / (float)inc_ccm;
	PRINTF("cost cyc per cc miss: %.3f\n",rate);

	inc_cyc = ((cnt[3]-cnt[2]) - (cnt[1]-cnt[0]))*4;
	inc_ccm = 512;
	rate = (float)inc_cyc / (float)inc_ccm;
	PRINTF("cost cyc per access hard reg: %.3f\n",rate);

	return 0;
}

#include <mipsop.h>
#define PAGE_SIZE (16 * 1024)
#define PAGE_MASK (~(PAGE_SIZE-1))
static void my_memcpy(void *dst, void *src, int size)
{
	unsigned char *pdst = dst;
	unsigned char *psrc = src;

	while(size--) {
		*pdst++ = *psrc++;
	}
}
static unsigned int test_mem_copy(unsigned long addr, int size)
{
	unsigned int i,loop;

	loop = 768 * 32 * 16 * 1024 / size;
	PRINTF("test mem copy %lx %dKB loop:%d\n",addr,size/1024,loop);
	pmon_prepare(PMON_EVENT_CYCLE);
	//pmon_prepare(PMON_EVENT_CACHE);
	//pmon_prepare(PMON_EVENT_INST);

	pmon_start();
	for (i = 0; i < loop; i++)
		my_memcpy((void*)addr,(void*)(addr+size/2),size/2);
	pmon_stop();

	pmon_report();
	return 0;
}

int map_mem_tlb(unsigned long vaddr, unsigned long paddr,
		       unsigned page_size, unsigned mem_size);
static void flush_tlb_entry(void);
static void show_tlb_entry(void);

#define  PADDR 0x02000000
#define KVADDR 0x82000000
#define UVADDR 0x12000000
int tlb_pref(void)
{
	unsigned long kvaddr = KVADDR;
	unsigned long  paddr = PADDR;
	unsigned long uvaddr = UVADDR;
	int size = PAGE_SIZE * 32;
	int i;

	show_tlb_entry();
	test_mem_copy(kvaddr, size);

	__write_32bit_c0_register($5 ,4, 0xa9000000);
	
	test_mem_copy(kvaddr, size);
	flush_tlb_entry();
	test_mem_copy(kvaddr, size);

	PRINTF("id pagemask   entryhi   entrylo0  entrylo1\n");
	for (i = 0; i < size/(PAGE_SIZE*2); i++) {
		unsigned long pagemask = (PAGE_SIZE*2) - 1;
		unsigned long entryhi, entrylo0, entrylo1;

		entryhi = uvaddr & PAGE_MASK;
		
		entrylo0 = (((paddr/PAGE_SIZE)<<6) | 07 | (3 << 3));
		paddr += PAGE_SIZE;
 		entrylo1 = (((paddr/PAGE_SIZE)<<6) | 07 | (3 << 3));
		paddr += PAGE_SIZE;
		
		uvaddr += (2*PAGE_SIZE);

		PRINTF("%2d 0x%08lX 0x%08lX 0x%08lX 0x%08lX\n",
		       i, pagemask, entryhi, entrylo0, entrylo1);
		
		write_c0_index(i);
		write_c0_pagemask(pagemask);
		write_c0_entryhi(entryhi);
		write_c0_entrylo0(entrylo0);
		write_c0_entrylo1(entrylo1);
		/* barrier */
		__asm__ __volatile__("nop;nop;nop;nop;");
		/* write indexed tlb entry */
		__asm__ __volatile__("tlbwi");
	}

	uvaddr = UVADDR;
	PRINTF("memory copy test. page size %dKB\n",PAGE_SIZE/1024);
	test_mem_copy(kvaddr, size);
	test_mem_copy(uvaddr, size);
	return 0;
}
int tlb_pref1(void)
{
	unsigned long kvaddr = KVADDR;
	unsigned long  paddr = PADDR;
	unsigned long uvaddr = UVADDR;
	int i;

	__write_32bit_c0_register($5 ,4, 0xa9000000);
	PRINTF("\n###### New TLB memory test ######\n");

	int page_arr[8] = { 1,4,1,4,4,1,1,4};
	for (i = 0; i < 8; i++) {
		unsigned page_size = 4 * 1024 * page_arr[i];
		//unsigned mem_size = 64 * page_size;
		unsigned mem_size = 256 * 1024;

		PRINTF("\n------ New test period %dKB ------\n\n",
		       page_size/1024);

		PRINTF("---memory copy test before flush tlb\n");
		test_mem_copy(kvaddr, mem_size);
		flush_tlb_entry();

		PRINTF("---memory copy test before map tlb\n");
		test_mem_copy(kvaddr, mem_size);
		if (map_mem_tlb(uvaddr, paddr, page_size, mem_size))
			break;

		PRINTF("\nmemory copy test. page size %dKB\n",
		       page_size/1024);
		test_mem_copy(kvaddr, mem_size);
		test_mem_copy(uvaddr, mem_size);

		PRINTF("---memory copy test at last\n");
		test_mem_copy(kvaddr, mem_size);
	}
	return 0;
}


int map_mem_tlb(unsigned long vaddr, unsigned long paddr,
		       unsigned page_size, unsigned mem_size)
{
	unsigned int page_mask;
	int i,n;

	page_mask = ~ (page_size - 1);
	n = mem_size / (page_size * 2);
	if (n > 32)
		return -1;

	for (i = 0; i < n; i++) {
		unsigned long pagemask = (page_size*2) - 1;
		unsigned long entryhi, entrylo0, entrylo1;

		entryhi = vaddr & page_mask;
		
		entrylo0 = (((paddr/(4*1024))<<6) | 07 | (3 << 3));
		paddr += page_size;
 		entrylo1 = (((paddr/(4*1024))<<6) | 07 | (3 << 3));
		paddr += page_size;
		
		vaddr += (2*page_size);

		write_c0_index(i);
		write_c0_pagemask(pagemask);
		write_c0_entryhi(entryhi);
		write_c0_entrylo0(entrylo0);
		write_c0_entrylo1(entrylo1);
		/* barrier */
		__asm__ __volatile__("nop;nop;nop;nop;");
		/* write indexed tlb entry */
		__asm__ __volatile__("tlbwi");
	}
	return 0;
}
static void flush_tlb_entry(void)
{
	unsigned long paddr = 0;
	unsigned long uvaddr = 0;
	int i;
	
	/* flush TLB */
	for (i = 0; i < 32; i++) {
		unsigned long entryhi, entrylo0, entrylo1;

		entryhi = uvaddr & PAGE_MASK;
		
		entrylo0 = ((paddr/PAGE_SIZE)<<6);
		paddr += PAGE_SIZE;
 		entrylo1 = ((paddr/PAGE_SIZE)<<6);
		paddr += PAGE_SIZE;
		
		uvaddr += (2*PAGE_SIZE);

		write_c0_index(i);
		write_c0_pagemask(0);
		write_c0_entryhi(entryhi);
		write_c0_entrylo0(entrylo0);
		write_c0_entrylo1(entrylo1);
		/* barrier */
		__asm__ __volatile__("nop;nop;nop;nop;");
		/* write indexed tlb entry */
		__asm__ __volatile__("tlbwi");
	}
}
static void show_tlb_entry(void)
{
	int i;

	/* check TLB */
	for (i = 0; i < 32; i++) {
		write_c0_index(i);
		__asm__ __volatile__("nop;nop;nop;nop;");
		__asm__ __volatile__("tlbr");
		__asm__ __volatile__("nop;nop;nop;nop;");

		PRINTF("%2d %x %x %x %x\n",i,
		       read_c0_pagemask(),
		       read_c0_entryhi(),
		       read_c0_entrylo0(),
		       read_c0_entrylo1());
	}
}
