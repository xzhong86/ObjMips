
#ifndef __MEM_H__
#define __MEM_H__

#define PAGE_SHIFT (12)
#define PAGE_SIZE  (1<<PAGE_SHIFT)
#define PAGE_MASK  (~(PAGE_SIZE -1))

#define CCA_THROUGH	0
#define CCA_UCA		1
#define CCA_UNCACHE	2
#define CCA_CACHE	3

#define PG_Write	0x4
#define PG_Valid	0x2
#define PG_Global	0x1

/* page attribute */
#define PG_NORMAL	((CCA_CACHE << 3) | PG_Write | PG_Valid)
#define PG_READONLY	((CCA_CACHE << 3) | PG_Valid)
#define PG_GLOBAL	((CCA_CACHE << 3) | PG_Write | PG_Valid | PG_Global)
#define PG_UNCACHE	((CCA_UNCACHE << 3) | PG_Write | PG_Valid)
#define PG_THROUGH	((CCA_THROUGH << 3) | PG_Write | PG_Valid)
#define PG_UCA		((CCA_UCA << 3) | PG_Write | PG_Valid)

int add_mem_range(unsigned long addr, int len, 
		  unsigned long uaddr, unsigned pg_attr);
int add_mem_high(unsigned long phy, int len, 
		 unsigned long uaddr, unsigned pg_attr);

int remove_mem_range(unsigned long uaddr);

void mmu_dump_tlb(void);

#endif
