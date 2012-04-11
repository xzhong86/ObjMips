
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

#ifndef __TYPE_phy_t
#define __TYPE_phy_t
typedef unsigned long phy_t;
#endif

/* MMU system */
int map_mem_range(unsigned long k0addr, int len, 
		  unsigned long vaddr, unsigned pg_attr);
int map_mem_phy(phy_t addr, int len, 
		unsigned long vaddr, unsigned pg_attr);

int unmap_mem_range(unsigned long vaddr);

void mmu_dump_tlb(void);


/* memory page manger api */
extern unsigned long max_low_pfn;
struct page;

/* flag in mem_get_pages,mem_alloc */
#define MEM_LOW	0x1

struct page * mem_get_pages(int page_nr, unsigned flag);
int mem_free_pages(struct page *pages, int nr);
/* get phy addr of page */
phy_t mem_page2phy(struct page *pages);
/* get page from phy addr */
struct page * mem_phy2page(phy_t addr);

void mem_dump_buddy(void);


/* memory alloc base on page system, if alloc
 * size < PAGE_SIZE/2, mem_alloc return NULL.
 */
void * mem_alloc(unsigned size);
void mem_free(void *);

/* get phy addr from any pointer */
phy_t mem_get_phy(void*);


#endif
