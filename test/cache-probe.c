#include <stdio.h>
#include <smp_io.h>
#include <time.h>
#include <pmon.h>
//#include <cache.h>

#define CACHE_SIZE_MAX (1024*1024)
#define CACHE_SIZE_MIN (1024)
#define CACHE_WAYS_MAX (128)
#define LINE_SIZE_MAX  (512)
#define LINE_SIZE_MIN  (8)

static struct cache_info {
	int size,ways,line_size,cost_min,cost_max;
} L1, L2;

static int get_cache_cost(int lsize,int csize,int bytes,char *buf)
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
static void probe_cache_size(void *space)
{
	char *buf = space;
	int size = CACHE_SIZE_MIN;
	int bytes = CACHE_SIZE_MAX*2;
	int lsize = LINE_SIZE_MIN;
	int i, cost,cost_max,cost_min = 0;

	for (i = 0; i < size; i++)
		buf[i] = 1;
	cost_min = get_cache_cost(LINE_SIZE_MIN, size, bytes, buf);
	size *= 2;

next_size:
	if (size > CACHE_SIZE_MAX)
		return;
	for (i = 0; i < size; i++)
		buf[i] = 1;
	
	cost = get_cache_cost(LINE_SIZE_MIN, size, bytes, buf);
	printk("probe cache size %dKB %d ...\n",size/1024,cost);
	if (cost <= cost_min || cost - cost_min < (cost_min)/4) {
		size *= 2;
		goto next_size;
	}

	cost_min = cost;
	cost_max = 0;
	
	printk("lsize %d - %d: %d",LINE_SIZE_MIN,LINE_SIZE_MAX,cost);
	for (i = LINE_SIZE_MIN*2; i <= LINE_SIZE_MAX; i *= 2) {
		cost = get_cache_cost(i, size, bytes, buf);
		printk(" %d",cost);
		if (cost > cost_max)
			cost_max = cost;
	}
	printk("\n");

	printk("size %dKB, cost %d - %d\n",size/1024,cost_min,cost_max);

	for (i = LINE_SIZE_MIN; i <= LINE_SIZE_MAX; i *= 2) {
		cost = get_cache_cost(i, size, bytes, buf);
		if (cost >= cost_max || cost_max - cost < cost_max/8)
			break;
	}
	lsize = i;
	printk("-------------- cache probe ----------------\n");
	printk("cache size %dKB, line size %dB, cost %d\n\n",
	       size/2048, lsize, cost_min);
	
	if (L1.size == 0) {
		L1.size = size/2;
		L1.line_size = lsize;
		L1.cost_min = cost_min;
		L1.cost_max = cost_max;
	}
	else if (L2.size == 0) {
		L2.size = size/2;
		L2.line_size = lsize;
		L2.cost_min = cost_min;
		L2.cost_max = cost_max;
	}

	size *= 2;
	goto next_size;
	
	return;
}
static void probe_cache_ways(struct cache_info *cache,void *space)
{
	int bytes = CACHE_SIZE_MAX*2;
	char *buf = space;
	int i,ways,cost_min,cost_max,cost[10];

	for (i = 0; i < cache->size; i++)
		buf[i] = 1;

	cost_min = 65535;
	cost_max = 0;
	for (ways = 2, i = 1; ways <= CACHE_WAYS_MAX; ways *= 2, i++) {
		cost[i] = get_cache_cost(cache->size/ways, cache->size*2, bytes, buf);
		if (cost[i] > cost_max)
			cost_max = cost[i];
		if (cost[i] < cost_min)
			cost_min = cost[i];
	}
	ways = i;
	printk("ways %d - %d:",2,CACHE_WAYS_MAX);
	for (i = 1; i < ways; i++) 
		printk(" %d",cost[i]);
	printk("\n");
	for (i = 1; i < ways; i++) {
		if (cost[i] >= cost_max || cost_max - cost[i] < cost_max/8)
			break;
	}
	cache->ways = 1 << i;
	return;
}
static void report_cache(struct cache_info *cache,char *des)
{
	printk("cache descript: %s\n",des);
	printk("cache size : %dKB\n", cache->size/1024);
	printk("cache ways : %d-ways\n", cache->ways);
	printk("cache line size: %dB\n", cache->line_size);
	printk("cache match cost: %d\n", cache->cost_min);
	printk("cache miss  cost: %d\n", cache->cost_max);
}
int cache_probe(void)
{
	void *space;
	unsigned int cp0;
	
	asm("mfc0 %0,$16,0":"=r"(cp0));
	printk("cp0 config %x\n",cp0);

	printk("\n************* cache_probe() function. *************\n\n");
	space = (void*)0x82000000;
	//space = (void*)0xA2000000;
	probe_cache_size(space);
	probe_cache_ways(&L1,space);
	probe_cache_ways(&L2,space);
	report_cache(&L1,"L1 D-cache");
	report_cache(&L2,"L2 cache");
	return 0;
}
