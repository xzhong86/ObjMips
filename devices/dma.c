#include <smp.h>
#include <base.h>
#include <irq.h>
#include <dma.h>
#include <mips.h>
#include <bitops.h>
#include <stdio.h>
#include <string.h>

#define CH_DSA	0x0
#define CH_DTA	0x4
#define CH_DTC	0x8
#define CH_DRT	0xC
#define CH_DCS	0x10
#define CH_DCM	0x14
#define CH_DDA	0x18
#define CH_DSD	0x1C

#define DMAC	0x300
#define DIRQP	0x304
#define DDR	0x308
#define DDRS	0x30C
#define DCKE	0x310
#define DCKES	0x314
#define DCKEC	0x318

#define BIT(n)	(1<<(n))

#define DMAC_HLT	BIT(3)
#define DMAC_AR		BIT(2)

#define DCS_NDESC	BIT(31)
#define DCS_AR		BIT(4)
#define DCS_TT		BIT(3)
#define DCS_HLT		BIT(2)
#define DCS_CTE		BIT(0)

#define DCM_SAI		BIT(23)
#define DCM_DAI		BIT(22)
#define DCM_SP_MSK	(0x3 << 14)
#define DCM_SP_16	BIT(15)
#define DCM_SP_8	BIT(14)
#define DCM_DP_MSK	(0x3 << 12)
#define DCM_DP_16	BIT(13)
#define DCM_DP_8	BIT(12)
#define DCM_TSZ_MSK	(0x7 << 8)
#define DCM_TSZ_SHF	8
#define DCM_TIE		BIT(1)
#define DCM_LINK	BIT(0)

#define DMA_BASE	0xB3420000
struct master;
struct dma_chan {
	volatile void *iomem;
	int chan,cpu;
	struct master *master;
	callback_f callback;
	void *data;
	/* flag bit shift */
#define FLGS_INUSE	0
#define FLGS_TRANS	1
	unsigned long flags;
};
#define CHAN_NUM	6
#define MASTER_NUM	2
struct master {
	volatile void *iomem;
	struct dma_chan chans[CHAN_NUM];
} masters[MASTER_NUM];
#define ALL_CHAN_NUM	(CHAN_NUM * MASTER_NUM)
struct dma_chan *all_chans[ALL_CHAN_NUM];

#define regr(p,off) (*(volatile unsigned*)((p)->iomem+(off)))
#define regw(p,val,off) (*(volatile unsigned*)((p)->iomem+(off)) = (val))

static void dma_irq0(int irq, void *d);
static void dma_irq1(int irq, void *d);
int dma_init(void)
{
	int i,j;

	register_irqfun(tst_irq(0,0x01000000),dma_irq0,"DMA",NULL);
	register_irqfun(tst_irq(0,0x00800000),dma_irq1,"DMA",NULL);

	volatile unsigned *clkgr0 = (unsigned*)0xB0000020;
	printk("clkgr0 %x\n",*clkgr0);
	//memset(masters,0,sizeof(masters));
	for (i = 0; i < MASTER_NUM; i++) {
		masters[i].iomem = (volatile void*)(DMA_BASE + 0x100*i);
		for (j = 0; j < CHAN_NUM; j++) {
			struct dma_chan *ch = &masters[i].chans[j];
			ch->iomem = masters[i].iomem + j * 0x20;
			ch->chan = j;
			ch->master = &masters[i];
			all_chans[i*CHAN_NUM+j] = ch;
		}
		regw(&masters[i],1,DMAC);
	}
	return 0;
}

static void wake_up(void) { return; }
static void dma_irq(struct master *master)
{
	unsigned int pending,err;

	pending = regr(master,DIRQP);
	while (pending) {
		int chan = ffs(pending)-1;
		struct dma_chan *ch = &master->chans[chan];

		//printk("dma irq master %d chan %d\n",master-masters,ch->chan);
		err = regr(ch,CH_DCS) & (DCS_HLT | DCS_AR);
		if (ch->callback)
			ch->callback(ch->data,err);
		if (ch->cpu != smp_cpu_id())
			smp_ipi_func(1<<ch->cpu, wake_up);

		regw(ch, 0, CH_DCS);
		regw(master,BIT(ch->chan),DCKEC);
		clear_bit(FLGS_TRANS,&ch->flags);
		pending &= ~BIT(chan);
	}
	err = regr(master, DMAC);
	if (err & (DMAC_HLT | DMAC_AR))
		regw(master, err & ~(DMAC_HLT | DMAC_AR), DMAC);
}
static void dma_irq0(int irq, void *d)
{
	dma_irq(&masters[0]);
}
static void dma_irq1(int irq, void *d)
{
	dma_irq(&masters[1]);
}

struct dma_chan * dma_request(void)
{
	struct dma_chan *ch;
	int i;
	for (i = 0; i < ALL_CHAN_NUM; i++) {
		ch = all_chans[i];
		if (!test_bit(FLGS_INUSE,&ch->flags))
			break;
	}
	if (i == ALL_CHAN_NUM)
		return NULL;

	set_bit(FLGS_INUSE,&ch->flags);
	//regw(ch->master,BIT(ch->chan),DCKES);
	return ch;
}
void dma_release(struct dma_chan *ch)
{
	clear_bit(FLGS_INUSE,&ch->flags);
}

#if 0
static void dump_chan_reg(struct dma_chan *ch)
{
	printk("master %d chan %d regs:\n",ch->master-masters,ch->chan);
#define D(REG) printk("\t" #REG ":\t%x\n",regr(ch,CH_##REG))
	D(DSA);	D(DTA);	D(DTC);	D(DCM);	D(DCS); D(DRT); D(DDA);
	printk("\n\tDMAC:\t%x\n",regr(ch->master,DMAC));
	printk("\tDCKE:\t%x\n",regr(ch->master,DCKE));
}
#endif

static inline unsigned get_max_tsz(unsigned int val, unsigned int *dcmp)
{
	/* tsz for 1,2,4,8,16,32,64 bytes */
	const static char dcm_tsz[7] = { 1,2,0,0,3,4,5 };
	int ord;
	
	ord = ffs(val) - 1;
	if (ord < 0)
		ord = 0;
	else if (ord > 6)
		ord = 6;
	*dcmp &= ~DCM_TSZ_MSK;
	*dcmp |= dcm_tsz[ord] << DCM_TSZ_SHF;
	/* if tsz == 8, set it to 4 */
	return ord == 3 ? 4 : 1 << ord;
}

int dma_trans(struct dma_chan *ch,struct dma_trans *trans,int num,
	      callback_f callback, void *data)
{
	unsigned int dcm;
	int tsz;
	
	if (test_bit(FLGS_TRANS,&ch->flags))
		return -1;

	regw(ch->master,BIT(ch->chan),DCKES);

	ch->callback = callback;
	ch->data = data;
	if (num == 1) {
		dcm = DCM_TIE | DCM_DAI | DCM_SAI ;
		tsz = get_max_tsz(trans[0].src|trans[0].dst|trans[0].len,&dcm);
		regw(ch,K0_TO_PHYS(trans[0].src),CH_DSA);
		regw(ch,K0_TO_PHYS(trans[0].dst),CH_DTA);
		regw(ch,trans[0].len/tsz,CH_DTC);
		regw(ch,dcm,CH_DCM);
		regw(ch,8,CH_DRT);
		regw(ch,DCS_NDESC, CH_DCS);
	}

	ch->cpu = smp_cpu_id();
	set_bit(FLGS_TRANS,&ch->flags);

	//dump_chan_reg(ch);
	regw(ch,regr(ch,CH_DCS) | DCS_CTE, CH_DCS);
	if (!callback)
		while (test_bit(FLGS_TRANS,&ch->flags)) {
			cpu_wait();
		}
	return 0;
}
