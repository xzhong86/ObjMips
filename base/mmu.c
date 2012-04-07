#include <stdio.h>
#include <stdlib.h>
#include <smp.h>
#include <base.h>
#include <linux/spinlock.h>
#include <mem.h>
#include <mips.h>
#include <mipsop.h>

//#define DEBUG

struct mem_range {
	/* used in asm, the order is important */
	unsigned long start,end;
	struct EntryLo {
		unsigned int lo0,lo1;
	} pairs[0];
};
struct mem_manage {
	arch_spinlock_t lock;
#define MEM_RANGE_MAX 128
	struct mem_range *ranges[MEM_RANGE_MAX];
	int nr_ranges;
};
static struct mem_manage mem_manager;
static int tlb_entry_num;

struct mem_range *current_range[CPU_NR];

#ifdef DEBUG
#define pr_debug  printk
static void dump_mem_range(struct mem_range *r)
{
	int i,n;
	printk("mem: %lx - %lx\n",r->start,r->end);
	n = ((r->end - r->start)/PAGE_SIZE +1) /2;
	for (i = 0; i < n; i++)
		printk("\tlo0:%x(%x)\tlo1:%x(%x)\n",
		       r->pairs[i].lo0,r->pairs[i].lo0>>6,
		       r->pairs[i].lo1,r->pairs[i].lo1>>6);
}
#else
#define pr_debug(arg...)  do { } while(0)
#define dump_mem_range(r) do { } while(0)
#endif

static void flush_tlb_entry(void);

int mm_fault(unsigned long addr, int read)
{
	int cpu = smp_cpu_id();
	int i;

	pr_debug("mm_fault: %lx %s\n",addr,read?"read":"write");
	arch_spin_lock(&mem_manager.lock);
	for (i = 0; i < MEM_RANGE_MAX; i++) {
		struct mem_range *r = mem_manager.ranges[i];
		if (r && r->start <= addr && addr < r->end) {
			if (current_range[cpu] != r) {
				current_range[cpu] = r;
				flush_tlb_entry();
			} else {
				i = MEM_RANGE_MAX;
#ifdef DEBUG
				mmu_dump_tlb();
#endif
			}
			break;
		}
	}
	i = i == MEM_RANGE_MAX? -1: 0;
	arch_spin_unlock(&mem_manager.lock);
	return i;
}
static int map_mem_pfn(unsigned pfn, int nrpage, 
		       unsigned long uaddr, unsigned pg_attr)
{
	int i, j, n, pairs, err = -1;
	unsigned long uend;

	if (uaddr == 0)
		return -1;

	uend = uaddr + nrpage * PAGE_SIZE;
	uaddr &= ~(PAGE_SIZE*2 - 1);
	arch_spin_lock(&mem_manager.lock);
	if (mem_manager.nr_ranges >= MEM_RANGE_MAX)
		goto out;

	for (i = 0; i < MEM_RANGE_MAX; i++) {
		struct mem_range *r = mem_manager.ranges[i];
		if (r && r->start <= uend && uaddr < r->end) {
			printk("[MMU] add range err\n");
			goto out;
		}
	}
	n = pfn % 2;
	pfn -= n;
	pairs = ((nrpage +n +1) / 2);
	pg_attr &= 0x3f;
	for (i = 0; i < MEM_RANGE_MAX; i++) {
		struct mem_range *r = mem_manager.ranges[i];
		if (r)
			continue;
		r = (typeof(r))malloc(sizeof(struct mem_range) + 
				      sizeof(struct EntryLo) * pairs);
		r->start = uaddr;
		r->end = uend;
		for (j = 0; j < pairs; j++) {
			r->pairs[j].lo0 = ((pfn+0) << 6) | pg_attr;
			r->pairs[j].lo1 = ((pfn+1) << 6) | pg_attr;
			pfn += 2;
		}
		if (n)
			r->pairs[0].lo0 &= ~0x2;
		if (nrpage < pairs*2-n)
			r->pairs[pairs-1].lo1 &= ~0x2;
		dump_mem_range(r);
		mem_manager.ranges[i] = r;
		mem_manager.nr_ranges ++;
		break;
	}
	err = i == MEM_RANGE_MAX? -1: 0;

out:
	arch_spin_unlock(&mem_manager.lock);
	if (!err)
		smp_ipi_func(0xff, flush_tlb_entry);
	return err;
}
int unmap_mem_range(unsigned long uaddr)
{
	int i,j;
	uaddr = uaddr & PAGE_MASK;
	arch_spin_lock(&mem_manager.lock);
	for (i = 0; i < MEM_RANGE_MAX; i++) {
		struct mem_range *r = mem_manager.ranges[i];
		if (!r || r->start != uaddr) 
			continue;
		for (j = 0; j < CPU_NR; j++)
			if (current_range[j] == r)
				current_range[j] = NULL;
		free(r);
		mem_manager.ranges[i] = NULL;
		mem_manager.nr_ranges --;
		break;
	}
	i = i == MEM_RANGE_MAX? -1: 0;
	arch_spin_unlock(&mem_manager.lock);
	if (! i)
		smp_ipi_func(0xff, flush_tlb_entry);
	return i;
}

int map_mem_range(unsigned long addr, int len, 
		  unsigned long uaddr, unsigned pg_attr)
{
	unsigned long start,end = addr + len;

	if ((addr >> 28) != 8)
		return -1;
	start = K0_TO_PHYS(addr)/PAGE_SIZE;
	end = (K0_TO_PHYS(end) + PAGE_SIZE -1)/PAGE_SIZE;
	return map_mem_pfn(start, end-start, uaddr & PAGE_MASK, pg_attr);
}
int map_mem_high(unsigned long phy, int len, 
		 unsigned long uaddr, unsigned pg_attr)
{
	unsigned long start,end = phy + len;

	if (phy < 0x30000000)
		return -1;
	end = (end + PAGE_SIZE -1)/PAGE_SIZE;
	start = phy/PAGE_SIZE;
	return map_mem_pfn(start, end-start, uaddr & PAGE_MASK, pg_attr);
}

int mmu_init(void)
{
	unsigned long config;

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
