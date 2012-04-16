

#define DMA_BASE	0xB3420000

#define CH_DSA	0x0
#define CH_DTA	0x4
#define CH_DTC	0x8
#define CH_DRT	0xC
#define CH_DCS	0x10
#define CH_DCM	0x14
#define CH_DDA	0x18
#define CH_DSD	0x1C

#define DMAC	0x1000
#define DIRQP	0x1004
#define DDR	0x1008
#define DDRS	0x100C


#define DMAC_HLT	BIT(3)
#define DMAC_AR		BIT(2)
#define DMAC_EN		BIT(0)

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

#define TEST_SIZE	(16*1024)
struct my_data {
	int times;
	int *mem;
	void *iomem;
	phy_t phy;
}

static int dma_prepare(struct fw_dev *dev)
{
	struct my_data *md;

	md = malloc(sizeof(*md));
	if (!md)
		return -1;
	md->mem = mem_alloc(TEST_SIZE);
	if (!md->mem) {
		free(md);
		return -1;
	}
	md->phy = mem_get_phy(md->mem);
	md->iomem = ioremap(DMA_BASE, 0x1000);
	dev->priv = md;
	return 0;
}

static int dma_start(struct fw_dev *dev)
{
	struct my_data *md = dev->priv;
	int i;

	md->times ++;
	for (i = 0; i < TEST_SIZE/8; i++) 
		md->mem[i] = rand();
	
	return 0;
}

static void dma_ask(struct fw_dev *dev)
{
	struct my_data *md = dev->priv;
	unsigned int *p;

	p = md->mem + (BSIZE/4) * md->pos;
	memcpy(p, md->mem, BSIZE);
	p[0] = get_int_hash(p+1, BSIZE/4-1);
	if (++md->pos >= MSIZE/BSIZE)
		fw_finish(dev);
}

static chk_t dma_check(struct fw_dev *dev)
{
	struct my_data *md = dev->priv;
	int i;

	if (md->pos != MSIZE/BSIZE)
		return CHK_FAILED;
	for (i = 0; i < MSIZE/BSIZE; i++)
		if (md->mem[0] != md->mem[i*BSIZE/4])
			return CHK_FAILED;

	if (md->times ++ > 10)
		return CHK_FINISHED;
	return CHK_PASSED;
}

static void dma_halt(struct fw_dev *dev)
{
	struct my_data *md = dev->priv;

	if (md->times <= 10)
		printk("no, i stopped at %d\n", md->times);
	else
		printk("o, i stopped\n");
	mem_free(md->mem);
	free(md);
}

static struct fw_ops dma_ops = {
	.prepare = dma_prepare,
	.start = dma_start,
	.ask = dma_ask,
	.check = dma_check,
	.halt = dma_halt,
};

static int init_dev(void)
{
	if (!fwdev_register("dma", &dma_ops))
		return -1;
	return 0;
}
fw_initcall(init_dev,0);
