
#ifndef CACHECFG_H
#define CACHECFG_H


#define CFG_CACHELINE_SIZE 32

#define CFG_ICACHE_SIZE (32 * 1024)
#define CFG_DCACHE_SIZE (32 * 1024)

#define CFG_ICACHELINE_SIZE CFG_CACHELINE_SIZE
#define CFG_DCACHELINE_SIZE CFG_CACHELINE_SIZE

#define CFG_ICACHE_LINES (CFG_ICACHE_SIZE/CFG_ICACHELINE_SIZE)
#define CFG_DCACHE_LINES (CFG_DCACHE_SIZE/CFG_DCACHELINE_SIZE)

#define CFG_ICACHEWAY_SIZE (4*1024)
#define CFG_DCACHEWAY_SIZE (4*1024)

#define CFG_ICACHE_WAYS (CFG_ICACHE_SIZE/CFG_ICACHEWAY_SIZE)
#define CFG_DCACHE_WAYS (CFG_DCACHE_SIZE/CFG_DCACHEWAY_SIZE)

/*
 * Cache Operations
 */
#define Index_Invalidate_I      0x00
#define Index_Writeback_Inv_D   0x01
#define Index_Invalidate_SI     0x02
#define Index_Writeback_Inv_SD  0x03
#define Index_Load_Tag_I	0x04
#define Index_Load_Tag_D	0x05
#define Index_Load_Tag_SI	0x06
#define Index_Load_Tag_SD	0x07
#define Index_Store_Tag_I	0x08
#define Index_Store_Tag_D	0x09
#define Index_Store_Tag_SI	0x0A
#define Index_Store_Tag_SD	0x0B
#define Create_Dirty_Excl_D	0x0d
#define Create_Dirty_Excl_SD	0x0f
#define Hit_Invalidate_I	0x10
#define Hit_Invalidate_D	0x11
#define Hit_Invalidate_SI	0x12
#define Hit_Invalidate_SD	0x13
#define Fill			0x14
#define Hit_Writeback_Inv_D	0x15
					/* 0x16 is unused */
#define Hit_Writeback_Inv_SD	0x17
#define Hit_Writeback_I		0x18
#define Hit_Writeback_D		0x19
					/* 0x1a is unused */
#define Hit_Writeback_SD	0x1b
					/* 0x1c is unused */
					/* 0x1e is unused */
#define Hit_Set_Virtual_SI	0x1e
#define Hit_Set_Virtual_SD	0x1f

#endif
