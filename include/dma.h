
#ifndef __DMA_H__
#define __DMA_H__

struct dma_trans {
	unsigned long src,dst;
	int len;
};

struct dma_chan;
struct dma_chan * dma_request(void);
void dma_release(struct dma_chan *ch);

typedef void (*callback_f)(void*,int);
int dma_trans(struct dma_chan *ch,struct dma_trans *trans,
	      int num,callback_f callback,void *data);


#endif
