#include <cache.h>
#include <smp_io.h>

struct cache_info L1_icache,L1_dcache,L2_cache;

void flush_dcache_range(unsigned int start,unsigned int end)
{
	start &= ~(CFG_CACHELINE_SIZE-1);
	while (start <= end) {
		cache_unroll(start,Hit_Writeback_Inv_D);
		start += L1_dcache.linesize;
	}
	__asm__ volatile ("sync");
}
void invalid_dcache_range(unsigned int start,unsigned int end)
{
	start &= ~(CFG_CACHELINE_SIZE-1);
	while (start <= end) {
		cache_unroll(start,Hit_Invalidate_D);
		start += L1_dcache.linesize;
	}
	//__asm__ volatile ("sync");
}
void writeb_dcache_range(unsigned int start,unsigned int end)
{
	start &= ~(CFG_CACHELINE_SIZE-1);
	while (start <= end) {
		cache_unroll(start,Hit_Writeback_D);
		start += L1_dcache.linesize;
	}
	__asm__ volatile ("sync");
}

void flush_icache_range(unsigned int start,unsigned int end)
{
	start &= ~(CFG_CACHELINE_SIZE-1);
	while(start <= end) {
		cache_unroll(start,Hit_Invalidate_I);
		start += L1_icache.linesize;
	}
}

void blast_icache(void)
{
	unsigned int start,end;
	start = 0x80000000;
	end = start + L1_dcache.size;
	while(start < end) {
		cache_unroll(start,Index_Invalidate_I);
		start += L1_dcache.linesize;
	}
}
void blast_dcache(void)
{
	unsigned int start,end;
	start = 0x80000000;
	end = start + L1_dcache.size;
	while(start < end) {
		cache_unroll(start,Index_Writeback_Inv_D);
		start += L1_dcache.linesize;
	}
	__asm__ volatile ("sync");
}
void blast_cache_all(void)
{
	blast_dcache();
	blast_icache();
}

#include <mipsop.h>
static void get_L1_cache(struct cache_info *cache, unsigned int conf)
{
	int ways,sets,linesize;

	ways = ((conf >> 0) & 7) + 1;
	linesize = 2 << ((conf >> 3) & 7);
	sets = 64 << ((conf >> 6) & 7);

	if (linesize == 2) {
		cache->linesize = 0;
		cache->size = 0;
		cache->ways = 0;
	} else {
		cache->linesize = linesize;
		cache->size = linesize * ways * sets;
		cache->ways = ways;
	}
}
int cache_init(void)
{
	unsigned long config;

	config = __read_32bit_c0_register($16, 1);
	get_L1_cache(&L1_dcache, config >> 7);
	get_L1_cache(&L1_icache, config >> 16);

	printk("L1 I-cache: size %dKB, %d-ways, linesize %dB\n",
	       L1_icache.size/1024,L1_icache.ways,L1_icache.linesize);
	printk("L1 D-cache: size %dKB, %d-ways, linesize %dB\n",
	       L1_dcache.size/1024,L1_dcache.ways,L1_dcache.linesize);

	if (CFG_ICACHE_SIZE != L1_icache.size || 
	    CFG_DCACHE_SIZE != L1_dcache.size) {
		printk("Cache config Macro defined wrong!!!\n");
		return -1;
	}
	return 0;
}
