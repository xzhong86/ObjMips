/*
 * Cache operations for the cache instruction.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * (C) Copyright 1996, 1997 by Ralf Baechle
 */
#ifndef	__ASM_MIPS_CACHE_H__
#define	__ASM_MIPS_CACHE_H__

#include <cachecfg.h>

struct cache_info {
	unsigned int size,ways,linesize;
};

extern struct cache_info L1_icache,L1_dcache,L2_cache;

#define cache_unroll(base,op)	        	\
	__asm__ __volatile__("	         	\
		.set noreorder;		        \
		.set mips3;		        \
		cache %1, (%0);	                \
		.set mips0;			\
		.set reorder"			\
		:				\
		: "r" (base),			\
		  "i" (op));

void flush_icache_range(unsigned int start,unsigned int end);

void flush_dcache_range(unsigned int start,unsigned int end);
void invalid_dcache_range(unsigned int start,unsigned int end);
void writeb_dcache_range(unsigned int start,unsigned int end);

void blast_icache(void);
void blast_dcache(void);
void blast_cache_all(void);

/* cpu pipeline/write buffer flush */
static inline void jz_sync(void)
{
	__asm__ volatile ("sync");
}

#endif	/* __ASM_MIPS_CACHEOPS_H */
