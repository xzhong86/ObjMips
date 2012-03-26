#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <smp_io.h>
#include <time.h>
#include <pmon.h>
//#include <cache.h>

struct global {
	struct cache_info {
		const char *name;
		int size,ways,line_size;
		int cost_min,cost_max;
		int hit10, miss10; /* 10 times */
	} L1, L2, L3;
#define INST_NUM	512
	unsigned long buf[INST_NUM+2];
} *glb;

#include <mips_inst.h>
void flush_icache_range(unsigned int start,unsigned int end);
void flush_dcache_range(unsigned int start,unsigned int end);
static int build_read(unsigned long *buf, int lsize)
{
	unsigned long *p = buf;
	int i;

	for (i = 0; i < INST_NUM; i++)
		*p++ = INST_LW(2, i*lsize, 4);

	*p++ = INST_JR(31);
	*p++ = INST_NOP;
	flush_dcache_range((unsigned int)buf, (unsigned int)p);
	flush_icache_range((unsigned int)buf, (unsigned int)p);

	return i;
}
static int get_read_cost(int lsize,int csize,int bytes,char *buf)
{
	unsigned int tmp,cyc;
	int i,n,size, bs;

	bs = build_read(glb->buf, lsize);
	size = bs * lsize;

	for (i = 0; i < csize; i+=lsize)
		buf[i] += 1;

	pmon_prepare(PMON_EVENT_CYCLE);
	pmon_start();
	for (i = 0, n = 0; i < bytes; i += bs) {
		((void (*)(char*))glb->buf)(&buf[n]);
		n += size;
		if (n >= csize) 
			n = 0;
	}
	pmon_stop();
	pmon_get_cnt32(tmp,cyc);
	tmp = cyc*10 / bytes;
	return tmp;
}
static int get_acc_cost(int lsize,int csize,int bytes,char *buf)
{
	unsigned int tmp,cyc;
	int i,n;

	pmon_prepare(PMON_EVENT_CYCLE);
	pmon_start();
	for (i = 0, n = 0; i < bytes; i++) {
		buf[n] += 1;
		n += lsize;
		if (n >= csize) 
			n = 0;
	}
	pmon_stop();
	pmon_get_cnt32(tmp,cyc);
	tmp = cyc / bytes;
	return tmp;
}

#define CACHE_SIZE_MAX (1024*1024)
#define CACHE_SIZE_MIN (1024)
#define CACHE_WAYS_MAX (128)
#define LINE_SIZE_MAX  (512)
#define LINE_SIZE_MIN  (8)

static void probe_cache_size(void *space)
{
	char *buf = space;
	int size = CACHE_SIZE_MIN;
	int bytes = CACHE_SIZE_MAX*2;
	int i, cost, cost_min = 0;

	for (i = 0; i < size; i++)
		buf[i] = 1;
	cost_min = get_acc_cost(LINE_SIZE_MIN, size, bytes, buf);
	size *= 2;

next_size:
	if (size > CACHE_SIZE_MAX)
		return;
	for (i = 0; i < size; i++)
		buf[i] = 1;
	
	cost = get_acc_cost(LINE_SIZE_MIN, size, bytes, buf);
	printk("probe cache size %dKB %d ...\n",size/1024,cost);
	if (cost <= cost_min || cost - cost_min < (cost_min)/4) {
		size *= 2;
		goto next_size;
	}

	if (glb->L1.size == 0) {
		glb->L1.name = "L1 D-cache";
		glb->L1.size = size/2;
		printk("May be %s cache %dKB found.\n","L1",size/2048);
	}
	else if (glb->L2.size == 0) {
		glb->L2.name = "L2 cache";
		glb->L2.size = size/2;
		printk("May be %s cache %dKB found.\n","L2",size/2048);
	}
	else if (glb->L3.size == 0) {
		glb->L3.name = "L3 cache";
		glb->L3.size = size/2;
		printk("May be %s cache %dKB found.\n","L3",size/2048);
	}

	cost_min = cost;
	size *= 2;
	goto next_size;
	
	return;
}
static void probe_cache_lsize(struct cache_info *cache,void *space)
{
	int bytes = CACHE_SIZE_MAX*2;
	char *buf = space;
	int i,ls,cost_min,cost_max,cost[10];

	for (i = 0; i < cache->size; i++)
		buf[i] = 1;

	printk("\n--------------Probe %s line size.\n",cache->name);
	printk("lsize %d - %d:", 8, LINE_SIZE_MAX);
	cost_min = 65535;
	cost_max = 0;
	for (ls = 8, i = 1; ls <= LINE_SIZE_MAX; ls *= 2, i++) {
		cost[i] = get_acc_cost(ls, cache->size*2, bytes, buf);
		if (cost[i] > cost_max)
			cost_max = cost[i];
		if (cost[i] < cost_min)
			cost_min = cost[i];
		printk(" %d",cost[i]);
	}
	printk("\n");
	ls = i;
	for (i = 1; i < ls; i++) {
		if (cost[i] >= cost_max || cost_max - cost[i] < cost_max/8)
			break;
	}
	cache->line_size = 8 << (i-1);
	return;
}
static void probe_cache_ways(struct cache_info *cache,void *space)
{
	int bytes = CACHE_SIZE_MAX*2;
	char *buf = space;
	int i,ways,cost_min,cost_max,cost[10];

	for (i = 0; i < cache->size; i++)
		buf[i] = 1;

	printk("\n--------------Probe %s ways num.\n",cache->name);
	printk("ways %d - %d:",2,CACHE_WAYS_MAX);
	cost_min = 65535;
	cost_max = 0;
	for (ways = 2, i = 1; ways <= CACHE_WAYS_MAX; ways *= 2, i++) {
		cost[i] = get_acc_cost(cache->size/ways, 
				       cache->size*2, bytes, buf);
		if (cost[i] > cost_max)
			cost_max = cost[i];
		if (cost[i] < cost_min)
			cost_min = cost[i];
		printk(" %d",cost[i]);
	}
	printk("\n");
	ways = i;
	for (i = 1; i < ways; i++) {
		if (cost[i] >= cost_max || cost_max - cost[i] < cost_max/8)
			break;
	}
	cache->ways = 1 << i;
	cache->cost_min = cost_min;
	cache->cost_max = cost_max;
	return;
}
static void probe_cache_cost(struct cache_info *cache,void *space)
{
	int cost;

	printk("\n--------------Probe %s hit/miss.\n",cache->name);
	cost = get_read_cost(cache->line_size, cache->size,
			     CACHE_SIZE_MAX*2, space);
	cache->hit10 = cost;
	printk("hit  %d.%d\n", cost/10, cost%10);

	cost = get_read_cost(cache->line_size, cache->size*2,
			     CACHE_SIZE_MAX*2, space);
	cache->miss10 = cost;
	printk("miss %d.%d\n", cost/10, cost%10);
	return;
}

static void report_cache(struct cache_info *cache)
{
	if (!cache->size)
		return ;
	printk("\ncache descript: %s\n",cache->name);
	printk("cache size : %dKB\n", cache->size/1024);
	printk("cache ways : %d-ways\n", cache->ways);
	printk("cache line size: %dB\n", cache->line_size);
	printk("cache min  cost: %d\n", cache->cost_min);
	printk("cache max  cost: %d\n", cache->cost_max);
	printk("cache hit   cost: %d.%d\n", cache->hit10/10, 
	       cache->hit10%10);
	printk("cache miss  cost: %d.%d\n", cache->miss10/10, 
	       cache->miss10%10);
}
int cache_probe(void)
{
	void *space;
	unsigned int cp0;
	
	asm("mfc0 %0,$16,0":"=r"(cp0));
	printk("cp0 config %x\n",cp0);

	glb = malloc(sizeof(*glb));
	memset(glb, 0, sizeof(*glb));

	printk("\n************* cache_probe() function. *************\n\n");
	space = (void*)0x82000000;
	//space = (void*)0xA2000000;
	probe_cache_size(space);

	if (glb->L1.size) {
		probe_cache_lsize(&glb->L1,space);
		probe_cache_ways(&glb->L1,space);
		probe_cache_cost(&glb->L1,space);
	}
	if (glb->L2.size) {
		probe_cache_lsize(&glb->L2,space);
		probe_cache_ways(&glb->L2,space);
		probe_cache_cost(&glb->L2,space);
	}
	if (glb->L3.size) {
		probe_cache_lsize(&glb->L3,space);
		probe_cache_ways(&glb->L3,space);
		probe_cache_cost(&glb->L3,space);
	}
	report_cache(&glb->L1);
	report_cache(&glb->L2);
	report_cache(&glb->L3);

	free(glb);
	return 0;
}
