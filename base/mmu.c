#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <smp.h>
#include <base.h>
#include <spinlock.h>
#include <mem.h>
#include <mips.h>
#include <mipsop.h>

//#define DEBUG
#if PAGE_SIZE != (4*1024)
#error "PAGE_SIZE must be 4k!"
#endif

#define PGD_SHIFT	22
#define PTE_SHIFT	12
#define PTRS_PER_PTE	(1024)
#define PTRS_PER_PGD	(1024)

#define get_pgd(addr) (((unsigned)(addr)) >> PGD_SHIFT)
#define get_pte(addr) ((((unsigned)(addr)) >> PTE_SHIFT) & (PTRS_PER_PTE-1))

struct pte {
	struct pg_pair {
		unsigned int lo0,lo1;
	} pairs[PTRS_PER_PTE/2];
};
struct pgd {
	struct pte *ptes[PTRS_PER_PGD];
};
static struct pgd *kernel_pgd;
struct pgd *current_pgd[CPU_NR];

static int tlb_entry_num;

#ifdef DEBUG
#define pr_debug  printk
static void dump_mem_pgd(struct pgd *pgd)
{
	int i, j;

	for (i = 0; i < PTRS_PER_PGD; i++) {
		struct pte *pte = pgd->ptes[i];
		if (!pte)
			continue;
		printk("pte %d %p:\n",i,pte);
		for (j = 0; j < PTRS_PER_PTE/2; j++) {
			struct pg_pair *pair = &pte->pairs[j];
			if (!pair->lo0 && !pair->lo1)
				continue;
			printk("  %4d: %08x %08x\n",j,pair->lo0,pair->lo1);
		}
	}
}
#else
#define pr_debug(arg...)  do { } while(0)
#define dump_mem_pgd(r) do { } while(0)
#endif

static void flush_tlb_entry(void);

static struct pg_pair *get_pair(struct pgd *pgd, unsigned long addr)
{
	int pgdi = get_pgd(addr);
	int ptei = get_pte(addr)/2;
	if (pgd && pgd->ptes[pgdi])
		return &pgd->ptes[pgdi]->pairs[ptei];
	return NULL;
}
static unsigned int *get_entrylo(struct pgd *pgd, unsigned long addr)
{
	struct pg_pair *pair;

	pair = get_pair(pgd, addr);
	if (pair) {
		if (addr/PAGE_SIZE %2)
			return &pair->lo1;
		else
			return &pair->lo0;
	}
	return NULL;
}
static phy_t get_phy_addr(unsigned long addr)
{
	int cpu = smp_cpu_id();
	unsigned int *lop;

	lop = get_entrylo(current_pgd[cpu],addr);
	if (lop)
		return (*lop >> 6) * PAGE_SIZE;
	return 0;
}

int mm_fault(unsigned long addr, int read)
{
	pr_debug("mm_fault: %lx %s\n",addr,read?"read":"write");
	if (get_phy_addr(addr))
		return 0;
	return -1;
}

static int insert_pte(struct pgd *pgd, unsigned long uaddr)
{
	struct page *page;
	int pgdi = get_pgd(uaddr);
	unsigned long addr;

	page = mem_get_pages(1, MEM_LOW);
	if (!page)
		return -1;
	addr = mem_page2phy(page);
	addr = PHYS_TO_K0(addr);
	pgd->ptes[pgdi] = (struct pte*)addr;
	memset((void*)addr, 0, sizeof(struct pte));
	return 0;
}
static void unmap_range(unsigned long start, unsigned long end)
{
	struct pgd *pgd;
	unsigned int *lop;

	pgd = current_pgd[smp_cpu_id()];
	while (start < end) {
		lop = get_entrylo(pgd, start);
		if (lop) {
			/* invalid page */
			*lop = 0;
		}
		start += PAGE_SIZE;
	}
}
static int map_mem_pfn(unsigned pfn, int nrpage, 
		       unsigned long uaddr, unsigned pg_attr)
{
	struct pgd *pgd;
	unsigned long addr;

	if (uaddr == 0)
		return -1;

	addr = uaddr;
	pg_attr &= 0x3f;
	pgd = current_pgd[smp_cpu_id()];
	while (nrpage > 0) {
		unsigned int *lop;
		lop = get_entrylo(pgd, addr);
		if (!lop) {
			if (insert_pte(pgd, addr))
				unmap_range(uaddr, addr - PAGE_SIZE);
			else
				continue;
		}
		*lop = (pfn << 6) | pg_attr;
		pfn += 1;
		addr += PAGE_SIZE;
		nrpage --;
	}

	return 0;
}

struct range_unit {
	unsigned long uaddr, end;
	int nr_page;
	struct range_unit *next;
};
static struct range_unit *range_list;
static spinlock_t range_lock;
static int insert_range(unsigned long uaddr, int nr_page)
{
	struct range_unit *ru, **rup;

	ru = malloc(sizeof(*ru));
	if (!ru)
		return -1;
	ru->uaddr = uaddr;
	ru->nr_page = nr_page;
	ru->end = uaddr + nr_page * PAGE_SIZE;

	spinlock_lock(range_lock);
	for (rup = &range_list; *rup; rup = &(*rup)->next)
		if ((*rup)->uaddr >= uaddr)
			break;
	if (*rup && (*rup)->uaddr < ru->end) {
		spinlock_unlock(range_lock);
		free(ru);
		return -1;
	}
	ru->next = *rup;
	*rup = ru;
	spinlock_unlock(range_lock);
	return 0;
}
static struct range_unit *deliver_range(unsigned long uaddr)
{
	struct range_unit *ru, **rup;

	ru = NULL;
	spinlock_lock(range_lock);
	for (rup = &range_list; *rup; rup = &(*rup)->next)
		if ((*rup)->uaddr == uaddr)
			break;
	if (*rup) {
		ru = *rup;
		*rup = ru->next;
	}
	spinlock_unlock(range_lock);
	return ru;
}
int unmap_mem_range(unsigned long uaddr)
{
	struct range_unit *ru;

	uaddr &= PAGE_MASK;
	ru = deliver_range(uaddr);
	if (!ru)
		return -1;

	unmap_range(ru->uaddr, ru->end);
	free(ru);
	smp_ipi_func(0xff, flush_tlb_entry);
	return 0;
}

int map_mem_range(unsigned long addr, int len, 
		  unsigned long uaddr, unsigned pg_attr)
{
	unsigned long start,end = addr + len;

	if ((addr >> 28) != 8 && (addr >> 28) != 9)
		return -1;
	if ((uaddr >> 28) == 8 || (uaddr >> 28) == 9 ||
	    (uaddr >> 28) == 0xa || (uaddr >> 28) == 0xb )
		return -1;

	start = K0_TO_PHYS(addr)/PAGE_SIZE;
	end = (K0_TO_PHYS(end) + PAGE_SIZE -1)/PAGE_SIZE;
	if (insert_range(uaddr & PAGE_MASK, end-start))
		return -1;
	return map_mem_pfn(start, end-start, uaddr & PAGE_MASK, pg_attr);
}
int map_mem_phy(unsigned long phy, int len, 
		unsigned long uaddr, unsigned pg_attr)
{
	unsigned long start,end = phy + len;

	if ((uaddr >> 28) == 8 || (uaddr >> 28) == 9 ||
	    (uaddr >> 28) == 0xa || (uaddr >> 28) == 0xb )
		return -1;

	end = (end + PAGE_SIZE -1)/PAGE_SIZE;
	start = phy/PAGE_SIZE;
	if (insert_range(uaddr & PAGE_MASK, end-start))
		return -1;
	return map_mem_pfn(start, end-start, uaddr & PAGE_MASK, pg_attr);
}

struct page * find_page_addr(unsigned long vaddr)
{
	return mem_phy2page(0);
}

int mmu_init(void)
{
	unsigned long config;

	kernel_pgd = malloc(sizeof(struct pgd));
	memset(kernel_pgd, 0, sizeof(struct pgd));
	current_pgd[0] = kernel_pgd;
	current_pgd[1] = kernel_pgd;
	config = __read_32bit_c0_register($16, 1);
	tlb_entry_num = (config >> 25) % 64 + 1;
	flush_tlb_entry();
	__write_32bit_c0_register($5, 4, 0xa9000000);
	printk("MMU: TLB Entries %d\n",tlb_entry_num);
	return 0;
}

void mmu_dump_tlb(void)
{
	int i;
	printk("Dump MMU tlb entries:\n");
	for (i = 0; i < 32; i++) {
		write_c0_index(i);
		__asm__ __volatile__("nop;nop;nop;nop;");
		__asm__ __volatile__("tlbr");
		__asm__ __volatile__("nop;nop;nop;nop;");

		printk("%2d %x %x %x %x\n",i,
		       read_c0_pagemask(),
		       read_c0_entryhi(),
		       read_c0_entrylo0(),
		       read_c0_entrylo1());
	}
}

static void flush_tlb_entry(void)
{
	int i;
	
	/* flush TLB */
	for (i = 0; i < tlb_entry_num; i++) {
		write_c0_index(i);
		write_c0_pagemask(0);
		write_c0_entryhi(i << 13);
		write_c0_entrylo0((2) << 6 | 0x0);
		write_c0_entrylo1((3) << 6 | 0x1f);
		/* barrier */
		__asm__ __volatile__("nop;nop;nop;nop;");
		/* write indexed tlb entry */
		__asm__ __volatile__("tlbwi");
	}
}
