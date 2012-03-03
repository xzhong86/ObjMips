#include "test-head.h"
#include <cache.h>
#include <dma.h>

#define DMA_TIMES	6
#define DMA_CHAN_NUM	12
#define DMA_AREA_BASE	0x82000000
#define DMA_AREA_SIZE	0x100000
#define DMA_AREA_NUM	(DMA_CHAN_NUM*2)

#if 0
int test_dma0(void)
{
	struct dma_chan *ch;
	struct dma_unit *dma;
	int i,j,e,err = -1,align = 64;

	dmas = (struct dma_unit*)malloc(sizeof(*dmas)*DMA_CHAN_NUM);
	if (!dmas)
		return -1;
	for (i = 0; i < DMA_CHAN_NUM; i++) {
		memset(&dmas[i], 0, sizeof(*dmas));
		ch = dma_request();
		if (!ch)
			goto error;
		dmas[i].ch = ch;
		dmas[i].trans.src = DMA_AREA_BASE + DMA_AREA_SIZE*i;
		dmas[i].trans.dst = DMA_AREA_BASE + DMA_AREA_SIZE*(i+DMA_AREA_NUM/2);
		dmas[i].trans.len = DMA_AREA_SIZE - 32;
	}

again:
	for (j = 0; j < DMA_CHAN_NUM; j++) {
		char fch = 0xa5 ^ (j + align); 
		dma = dmas + j;
		memset((void*)dma->trans.src, fch, dma->trans.len);
		printk("test chan %2d %p %2x ... ",j,dma->ch,fch&0xff);
		blast_dcache();
		dma_trans(dma->ch,&dma->trans,1,NULL);
		for (i = 0,e = 0; i < dma->trans.len; i++)
			if (((char*)dma->trans.dst)[i] != fch)
				e ++;
		if (e)
			printk("dma err %d in %d\n",e,i);
		else
			printk("ok!\n");
	}
	if (align) {
		for (j = 0; j < DMA_CHAN_NUM; j++)
			dmas[j].trans.len -= align;
		align /= 2;
		goto again;
	}
	err = 0;

error:
	for (i = 0; i < DMA_CHAN_NUM; i++)
		if (dmas[i].ch)
			dma_release(dmas[i].ch);
	free(dmas);
	return err;
}
#else
typedef unsigned char u8;
static int test_dma_single(unsigned src,unsigned dst,int len)
{
	struct dma_chan *ch;
	struct dma_trans trans;
	int i,e,seed;

	ch = dma_request();
	trans.src = src;
	trans.dst = dst;
	trans.len = len;
	seed = (src + dst + len) * 1103515245;
	for (i = 0; i < len; i++) {
		((u8*)src)[i] = (i+seed) & 0xff;
	}
	writeb_dcache_range(src,src+len);
	invalid_dcache_range(dst,dst+len);

	dma_trans(ch,&trans,1,NULL,NULL);
	dma_release(ch);
	for (i = 0,e = 0; i < len; i++) {
		e += ((u8*)src)[i] != ((u8*)dst)[i];
	}
	if (e == 0) {
		printk("%x to %x len %d successed.\n",src,dst,len);
		return 0;
	}
	printk("%x to %x len %d, %d failed.\n",src,dst,len,e);
	for (i = 0,e = 0; i < len && e < 32; i++) {
		if (((u8*)src)[i] != ((u8*)dst)[i]) 
			printk("char[%d] need %x got %x",i,
			       ((u8*)src)[i], ((u8*)dst)[i]);
	}
	
	return e;
}
int test_dma0(void)
{
	unsigned int src = DMA_AREA_BASE;
	unsigned int dst = src + 0x1000000;
	test_dma_single(src+0x100, dst+0x120, 0x1003);
	test_dma_single(src+0xce,  dst+0x710, 0x3574);
	test_dma_single(src+0x4a7, dst+0x10aa, 0xa99);
	test_dma_single(src+0x19e, dst+0x4c9, 0x32d2);
	test_dma_single(src+0x3276, dst+0x1861, 0x4c9);
	return 0;
}
#endif
DEF_TEST(test_dma0, 10, (TESTF_REPEAT | TESTF_DEVICE));

