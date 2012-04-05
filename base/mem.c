
#include <stdio.h>

#define MByte	(1024*1024)

static unsigned long mem_size = 32*MByte;
static void setup_mem_size(char *arg)
{
	char *p = arg;
	unsigned long size = 0;
	while ('0' <= *p && *p <= '9') {
		size *= 10;
		size += *p - '0';
		p ++;
	}
	if (*p != 'M' || *p != 'm') {
		printk("Bad mem argument: %s\n", arg);
		return;
	}
	p ++;
	size <<= 20;
	mem_size = size;
}
__setup("mem", setup_mem_size);

unsigned long max_low_pfn = 256*MByte/PAGE_SIZE;
struct page *page_table;

static int build_page_table(unsigned int max_pages);
static int add_mem_range(unsigned long start, unsigned size);
static int reserve_mem_range(unsigned long start, unsigned size);
static int mem_buddy_init(void);
int mm_init(void)
{
	unsigned long low, high, tab;
	unsigned long max_low, reserved;

	max_low = max_low_pfn * PAGE_SIZE;
	if (mem_size > max_low) {
		low = max_low;
		high = mem_size - max_low;
	} else {
		low = mem_size;
		high = 0;
	}

	printk("Memory size: %dMB, %dMB low, %dMB high.\n",
	       mem_size>>20, low>>20, high>>20);

	reserved = PCPU_BASE(CPU_NR);
	page_table = (struct page*)ALIGN_TO(reserved, MByte);
	build_page_table(1<<18); /* 256k pages, 1G memory */
	add_mem_range(0, low, MM_FREE);
	if (high) {
		add_mem_range(max_low, high, MM_FREE);
	}
	reserved = K0_TO_PHYS(reserved);
	reserve_mem_range(0, reserved);

	mem_buddy_init();
}

#define K0_PFN(addr)	(K0_TO_PHYS((unsigned)(addr))/PAGE_SIZE)
#define PPFN(paddr)	((paddr)/PAGE_SIZE)

static int page_table_max;
static int build_page_table(unsigned int max_pages)
{
	int i, n;

	page_table_max = max_pages;
	for (i = 0; i < max_pages; i++)
		page_table[i].flags = 0;
	n = K0_PFN(&page_table[max_pages]);
	i = K0_PFN(&page_table[0]);
	for (; i <= n; i++)
		page_table[i].flags |= PAGE_RSVD;
	return 0;
}
static int add_mem_range(unsigned long start, unsigned size)
{
	int i;

	for (i = PPFN(start); i < PPFN(start+size); i++)
		page_table[i].flags |= PAGE_VALID;
	return 0;
}
static int reserve_mem_range(unsigned long start, unsigned size)
{
	int i;

	for (i = PPFN(start); i < PPFN(start+size); i++)
		page_table[i].flags |= PAGE_RSVD;
	return 0;
}

struct mm_buddy {
#define MAX_ORDER	18	/* 256K pages max */
	struct page *area[MAX_ORDER];
	int nr_pages;
};
static struct mm_buddy *mbuddy;
static int buddy_put_pages(struct page *pages, int nr)
{
	
}
static struct page * buddy_get_pages(int nr, unsigned flag)
{
	struct page * pages;
	int i,order;

	if (nr > mbuddy->nr_free || !nr)
		return NULL;
	order = ffs(nr - 1);
	if (order >= MAX_ORDER) {
		printk("get big pages\n");
		return NULL;
	}
	if (!mbuddy->area[order]) {
		if (order + 1 >= MAX_ORDER)
			return NULL;
		
	}
	pages = mbuddy->area[order];
	mbuddy->area[order] = pages[1<<order].next;
	for (i = 0; i < nr; i++)
		page_table[i].flags |= PAGE_USED;
		
	buddy_put_pages(pages + nr, (1<<order) - nr);
}
static int mem_buddy_init(void)
{
	
}
