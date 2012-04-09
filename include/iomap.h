
#ifndef __IOMAP_H__
#define __IOMAP_H__

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

/* you should define DEV_REG_BASE in your C file. */
#define REG32(off) (*(volatile u32*)((void*)(DEV_REG_BASE) + (off)))
#define REG16(off) (*(volatile u16*)((void*)(DEV_REG_BASE) + (off)))
#define REG8(off) (*(volatile u8*)((void*)(DEV_REG_BASE) + (off)))

#ifndef __TYPE_phy_t
#define __TYPE_phy_t
typedef unsigned long phy_t;
#endif

/* memory/IO map/remap api */
extern int jzsoc_mem_remapped;

#include <mips.h>
/* IO addr remap, the phy addr in default mode */
void *__ioremap(phy_t addr, unsigned size);
static inline
void *ioremap(phy_t paddr, unsigned size)
{
	if (__builtin_constant_p(paddr)) {
		if (!jzsoc_mem_remapped && ((paddr)>>28) == 1)
			return (void*)PHYS_TO_K1(paddr);
	}
	return __ioremap(paddr, size);
}
void iounmap(void *vaddr);

#endif
