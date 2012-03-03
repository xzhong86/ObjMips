#include "test-head.h"
#include <string.h>
#include <cache.h>
#include <spinlock.h>
#include <dma.h>

#define DMA_TIMES	6
#define DMA_CHAN_NUM	12
#define DMA_AREA_BASE	0x82000000
#define DMA_AREA_SIZE	0x100000
#define DMA_AREA_NUM	(DMA_CHAN_NUM*2)

struct trans_unit {
	struct dma_trans trans;
	struct trans_unit *next;
};
struct trans_pool {
	spinlock_t lock;
	struct trans_unit *head;
};
static trans_pool trans_pool;
static struct dma_trans *get_trans(void)
{
	struct trans_unit *unit;
	spin_lock(&trans_pool.lock);
	unit = trans_pool.head;
	if (unit)
		trans_pool.head = unit->next;
	spin_unlock(&trans_pool.lock);
	return unit? &unit->trans: NULL;
}
static void put_trans(struct dma_trans *trans)
{
	struct trans_unit *unit = 
		container_of(trans, struct trans_unit, trans);
	spin_lock(&trans_pool.lock);
	unit->next = trans_pool.head;
	trans_pool.head = unit;
	spin_unlock(&trans_pool.lock);
	return &unit->trans;
}
static int init_trans_pool(void)
{
	struct trans_unit *unit;
	int order,size,align;
	unsigned long src,dst,len;

	while (src < );
	order = rand() % 8;
	len = 512 << order;
	order = rand() % 8;
	dst += 1 << order;
	len -= 1 << order;
	/* FIXME Here !! */
}

struct dma_unit {
	spinlock_t lock;
	struct dma_chan *ch;
	struct dma_trans *trans;
	int cpu,val,busy,check,times;
};
static struct dma_unit *dmas;

static void dma_finished(struct dma_unit *dma,int err)
{
	spin_lock(&dma->lock);
	dma->check = 1;
	dma->busy = 0;
	spin_unlock(&dma->lock);
}
static int dma_prepare(struct dma_unit *dma)
{
	struct dma_trans *trans = get_trans();
	dma->trans = trans;
	memset((void*)trans->src, dma->val, trans->len);
	memset((void*)trans->dst, 0, trans->len);
	flush_dcache_range(trans->src,trans->src + trans->len);
	flush_dcache_range(trans->dst,trans->dst + trans->len);
	dma->cpu = smp_cpu_id();
	dma->busy = 1;
	dma_trans(dma->ch,trans,1,dma_finished,dma);
	return 0;
}
static void dma_check(struct dma_unit *dma)
{
	int i,e;
	char *src = (char*)dma->trans->src;
	char *dst = (char*)dma->trans->dst;
	for (i = 0,e = 0; i < dma->trans->len; i++)
		e += *src++ != *dst++;
	dma->check = 0;
	dma->times ++;
}
static int do_dma_test(void)
{
	int i,n,cpu;
	
	cpu = smp_cpu_id();
again:
	for (i = 0,n = 0; i < DMA_CHAN_NUM; i++) {
		if (dmas[i].busy)
			continue;
		spin_lock(&dmas[i].lock);
		if (dmas[i].busy) {
			spin_unlock(&dmas[i].lock);
			continue;
		}
		if (dmas[i].check) {
			if (dmas[i].cpu == cpu) 
				dma_check(&dmas[i]);
			spin_unlock(&dmas[i].lock);
			continue;
		}

		if (dmas[i].times == DMA_TIMES)
			n ++;
		else
			dma_prepare(&dmas[i]);
		spin_unlock(&dmas[i].lock);
	}
	if (n < DMA_CHAN_NUM)
		goto again;
}

int test_dma(void)
{
	struct dma_chan *ch;
	int i,e;

	dmas = (struct dma_unit*)malloc(sizeof(*dmas)*DMA_CHAN_NUM);
	if (!dmas)
		return -1;
	for (i = 0; i < DMA_CHAN_NUM; i++) {
		memset(&dmas[i], 0, sizeof(*dmas));
		ch = dma_request();
		if (!ch)
			goto error;
		dmas[i].ch = ch;
	}
	do_dma_test();

error:
	for (i = 0; i < DMA_CHAN_NUM; i++)
		if (dmas[i].ch)
			dma_release(dmas[i].ch);
	return -1;

	if (!(ch = dma_request()))
		return -1;
	
	t.src = 0x82000000;
	t.dst = 0x83000000;
	t.len = 0x00100000;
	memset((void*)t.src, 0x5a, t.len);
	blast_dcache();
	printk("ch = %p\n",ch);
	dma_trans(ch,&t,1,NULL);
	for (i = 0,e = 0; i < t.len; i++)
		if (((char*)t.dst)[i] != 0x5a)
			e ++;
	printk("dma err %d in %d\n",e,i);
	return 0;
}
DEF_TEST(test_dma, 10, (TESTF_REPEAT | TESTF_DEVICE));

