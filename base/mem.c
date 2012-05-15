
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <base.h>
#include <mem.h>
#include <iomap.h>
#include <pcpu.h>
#include <mips.h>
#include <bitops.h>
#include <smp.h>

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
	if (*p != 'M' && *p != 'm') {
		printk("Bad mem argument: %s\n", arg);
		return;
	}
	p ++;
	size <<= 20;
	mem_size = size;
}
__setup("mem", setup_mem_size);

unsigned long max_low_pfn = 256*MByte/PAGE_SIZE;

struct page {
	unsigned long flags;
#define PAGE_VALID	0x1
#define PAGE_RSVD	0x2
#define PAGE_USED	0x4
#define PAGE_HIGH	0x8
	union {
		struct page *next;
	};
};
struct page *page_table;

static int build_page_table(unsigned int max_pages);
static int add_mem_range(unsigned long start, unsigned size);
static int reserve_mem_range(unsigned long start, unsigned size);
static int mem_buddy_init(void);
int mem_init(void)
{
	unsigned long low, high;
	unsigned long high_addr, max_low, reserved;

	high_addr = 768*MByte;
	if (jzsoc_mem_remapped) {
		max_low_pfn = 512*MByte/PAGE_SIZE;
		high_addr = 512*MByte;
	}
	max_low = max_low_pfn * PAGE_SIZE;
	if (mem_size > max_low) {
		low = max_low;
		high = mem_size - max_low;
	} else {
		low = mem_size;
		high = 0;
	}

	printk("Memory size: %ldMB, %ldMB low, %ldMB high.\n",
	       mem_size>>20, low>>20, high>>20);

	reserved = PCPU_BASE(CPU_MAX);
	page_table = (struct page*)ALIGN_TO(reserved, MByte);
	if (jzsoc_mem_remapped)
		/* 1M pages, 4G memory space in remap mode */
		build_page_table(1<<20);
	else
		/* 256k pages, 1G memory space with memory 512M max */
		build_page_table(1<<18);

	add_mem_range(0, low);
	if (high) {
		add_mem_range(high_addr, high);
	}
	reserved = K0_TO_PHYS(reserved);
	reserve_mem_range(0, reserved);

	return mem_buddy_init();
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

	if (PPFN(start) > page_table_max || 
	    PPFN(start+size) > page_table_max)
		return -1;

	for (i = PPFN(start); i < PPFN(start+size); i++) {
		page_table[i].flags |= PAGE_VALID;
		if (i >= max_low_pfn)
			page_table[i].flags |= PAGE_HIGH;
	}
	return 0;
}
static int reserve_mem_range(unsigned long start, unsigned size)
{
	int i;

	if (PPFN(start) > page_table_max || 
	    PPFN(start+size) > page_table_max)
		return -1;

	for (i = PPFN(start); i < PPFN(start+size); i++)
		page_table[i].flags |= PAGE_RSVD;
	return 0;
}

struct mm_buddy {
#define MAX_ORDER	18	/* 256K pages max */
	struct page *area[MAX_ORDER];
	int nr_free;
};
static struct mm_buddy *mbuddy;
#define PFN(p) 	((p) - page_table)
static int buddy_put(struct page *pages, int order)
{
	int self, buddy, max;

	self = pages - page_table;
	if (self & ((1<<order) -1))
		return -1;

	mbuddy->nr_free += 1 << order;
	do {
		struct page **pp;

		self = pages - page_table;
		if (order + 1 == MAX_ORDER)
			buddy = -1; /* no buddy at top */
		else
			buddy = self ^ (1 << order);
		max = self > buddy? self: buddy;

		for (pp = &mbuddy->area[order]; *pp; pp = &(*pp)->next) {
			int pfn = *pp - page_table;
			if (pfn > max || pfn == buddy)
				break;
		}
		if (*pp && PFN(*pp) == buddy) {
			struct page *pg = *pp;
			*pp = pg->next;
			if (max == self)
				pages = pg;
			order += 1;
		} else {
			pages->next = *pp;
			*pp = pages;
			pages = NULL;
		}
	} while(pages);

	return 0;
}
static struct page * buddy_get(int order, unsigned flag)
{
	struct page *pages;
	int s_order;

	for (s_order = order; s_order < MAX_ORDER; s_order ++)
		if (mbuddy->area[s_order])
			break;
	if (s_order == MAX_ORDER)
		return NULL;

	mbuddy->nr_free -= 1 << order;
	pages = mbuddy->area[s_order];
	mbuddy->area[s_order] = pages->next;
	while (s_order > order) {
		struct page **pp,*p = pages + (1<<(s_order-1));
		for (pp = &mbuddy->area[s_order-1]; *pp; pp = &(*pp)->next)
			if (PFN(p) < PFN(*pp))
				break;
		p->next = *pp;
		*pp = p;
		s_order --;
	}
	return pages;
}
static int buddy_put_pages(struct page *pages, int nr)
{
	int pfn, order;

	if (nr <= 0)
		return -1;

	do {
		int od;
		pfn = pages - page_table;
		order = _ffs(pfn) - 1;
		od = fls(nr) - 1;
		order = order < od? order: od;
		buddy_put(pages, order);
		nr -= 1 << order;
		pages += 1 << order;
	} while (nr > 0);

	return 0;
}
static struct page * buddy_get_pages(int nr, unsigned flag)
{
	struct page * pages;
	int order;

	if (nr > mbuddy->nr_free || nr <= 0)
		return NULL;
	order = fls(nr - 1);
	pages = buddy_get(order, flag);
	if (!pages)
		return NULL;
	if ((1<<order) - nr > 0)
		buddy_put_pages(pages + nr, (1<<order) - nr);
	return pages;
}
static int mem_buddy_init(void)
{
	int i, start;

	mbuddy = malloc(sizeof(*mbuddy));
	memset(mbuddy, 0, sizeof(*mbuddy));
	for (start = 0,i = 0; i < page_table_max; i++) {
		unsigned long flag = page_table[i].flags;
		flag &= (PAGE_USED | PAGE_RSVD | PAGE_VALID);
		if (flag == PAGE_VALID && start == 0)
			start = i;
		else if (flag != PAGE_VALID && start) {
			printk("add pfn %d - %d\n",start,i);
			buddy_put_pages(page_table + start, i - start);
			start = 0;
		}
	}
	return 0;
}

int mem_free_pages(struct page *pages, int nr)
{
	if (pages)
		return buddy_put_pages(pages, nr);
	return -1;
}
struct page * mem_get_pages(int nr, unsigned flag)
{
	return buddy_get_pages(nr, flag);
}
unsigned long mem_page2phy(struct page *pages)
{
	return PFN(pages) * PAGE_SIZE;
}
struct page * mem_phy2page(phy_t addr)
{
	return page_table + PPFN(addr);
}

void mem_dump_buddy(void)
{
	int i;

	printk("Dump buddy system:  free %d\n",mbuddy->nr_free);
	for (i = MAX_ORDER-1; i >= 0; i--) {
		struct page **pp;
		pp = &mbuddy->area[i];
		if (!*pp)
			continue;
		printk("buddy[%02d]:",i);
		for (; *pp; pp = &(*pp)->next)
			printk("%6d ",*pp - page_table);
		printk("\n");
	}
}

struct mem_unit {
	void * vaddr;
	unsigned nr_pages, flag;
	struct page *pages;
	struct mem_unit *next;
};
static struct mem_unit *mem_list;
void * mem_alloc(unsigned size)
{
	struct mem_unit *mu;
	struct page *pages;
	unsigned long addr;

	if (size < PAGE_SIZE/2)
		return NULL;
	mu = malloc(sizeof(*mu));
	if (!mu)
		return NULL;
	size = ((size - 1)/PAGE_SIZE) +1;
	pages = buddy_get_pages(size, MEM_LOW);
	if (!pages) {
		free(mu);
		return NULL;
	}
	addr = PFN(pages)*PAGE_SIZE + 0x80000000;
	mu->nr_pages = size;
	mu->flag = MEM_LOW;
	mu->vaddr = (void*)addr;
	mu->pages = pages;

	mu->next = mem_list;
	mem_list = mu;
	return (void*)addr;
}
void mem_free(void *addr)
{
	struct mem_unit *mu,**mup;

	for (mup = &mem_list; *mup; mup = &(*mup)->next)
		if ((*mup)->vaddr == addr)
			break;
	if (!*mup)
		return;

	mu = *mup;
	*mup = mu->next;

	buddy_put_pages(mu->pages, mu->nr_pages);
	free(mu);
}

extern struct page * find_page_addr(unsigned long);
unsigned long mem_get_phy(void * p)
{
	unsigned long vaddr = (unsigned long)p;
	unsigned long area, off;
	struct page *page;

	area = vaddr >> 28;
	off  = vaddr & 0xFFFFFFF;
	if (area == 8 || area == 0xa)
		return off;
	if (area == 9 || area == 0xb)
		return off + 0x10000000;

	page = find_page_addr(vaddr);
	if (!page)
		return 0;

	vaddr = PFN(page) * PAGE_SIZE;
	vaddr += off & (PAGE_SIZE - 1);
	return vaddr;
}


/* JZSoC remap */
int jzsoc_mem_remapped;

#if 0
/* FIXME: more complex remap system */
static unsigned long iomap_addr = 0xE0000000;
void *__ioremap(phy_t addr, unsigned size)
{
	void *p;
	unsigned pgoff;

	if (((addr) >> 28) != 1)
		return NULL;
	if (!jzsoc_mem_remapped)
		return (void*)PHYS_TO_K1(addr);

	addr += 0xE0000000;
	pgoff = addr & (PAGE_SIZE -1);
	addr -= pgoff;
	size += pgoff;
	if (map_mem_phy(addr, size, iomap_addr, PG_UNCACHE))
		return NULL;

	p = (void*)(iomap_addr + pgoff);
	iomap_addr += ALIGN_TO(size, PAGE_SIZE);
	return p;
}
void iounmap(void *vaddr)
{
	return;
}

#else
/* a simple remap system, linely remap 0xF0000000~0xFFFFFFFF
 * to virtual address 0xE0000000~0xEFFFFFFF
 */
void *__ioremap(phy_t phy, unsigned size)
{
	unsigned long addr;
	phy_t start, end;

	if (((phy) >> 28) != 1)
		return NULL;
	if (!jzsoc_mem_remapped)
		return (void*)PHYS_TO_K1(phy);

	addr = phy + 0xD0000000;
	start = mem_get_phy((void*)(addr));
	end = mem_get_phy((void*)(addr + size -1));

	if (start && end)
		return (void*)addr;
	if (end) {
		while (end && size > PAGE_SIZE) {
			size -= PAGE_SIZE;
			end = mem_get_phy((void*)(addr + size -1));
		}
	} else {
		while (start) {
			addr += PAGE_SIZE;
			start = mem_get_phy((void*)(addr));
		}
	}
	if (start || end)
		return NULL;

	phy = addr - 0xD0000000;
	if (map_mem_phy(phy+0xE0000000, size, addr, PG_UNCACHE))
		return NULL;
	return (void*)(phy + 0xD0000000);
}
void iounmap(void *vaddr)
{
	return;
}
#endif
