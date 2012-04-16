
#include <stdio.h>
#include <stdlib.h>

#include <base.h>
#include <init.h>
#include <mem.h>
#include <cache.h>
#include <mips.h>
#include <test/fw.h>

struct my_data {
	int times, n;
#define TEST_SIZE 100*1024
	unsigned int *mem;
	unsigned int phy;
};

static int my_prepare(struct fw_dev *dev)
{
	struct my_data *md;

	md = malloc(sizeof(*md));
	if (!md)
		return -1;
	md->times = 0;
	md->mem = mem_alloc(TEST_SIZE);
	if (!md->mem) {
		free(md);
		return -1;
	}
	md->phy = mem_get_phy(md->mem);
	dev->priv = md;
	return 0;
}

static int my_start(struct fw_dev *dev)
{
	struct my_data *md = dev->priv;
	int i;

	for (i = 0; i < TEST_SIZE/4; i++)
		md->mem[i] = 0;
	md->n = 0;
	flush_dcache_range((unsigned)md->mem,
			   (unsigned)md->mem + TEST_SIZE);
	return 0;
}

static void my_ask(struct fw_dev *dev)
{
	struct my_data *md = dev->priv;
	unsigned int *p;

	p = (unsigned int*)PHYS_TO_K1(md->phy);
	p[md->n] = md->n;
	if (++md->n >= TEST_SIZE/4)
		fw_finish(dev);
}

static chk_t my_check(struct fw_dev *dev)
{
	struct my_data *md = dev->priv;
	int i;

	if (md->n != TEST_SIZE/4)
		return CHK_FAILED;
	for (i = 0; i < TEST_SIZE/4; i++)
		if (md->mem[i] != i)
			return CHK_FAILED;

	if (md->times ++ > 10)
		return CHK_FINISHED;
	return CHK_PASSED;
}

static void my_halt(struct fw_dev *dev)
{
	struct my_data *md = dev->priv;

	if (md->times <= 10)
		printk("no, i stopped at %d\n", md->times);
	else
		printk("o, i stopped\n");
	mem_free(md->mem);
	free(md);
}

static struct fw_ops mydev_ops = {
	.prepare = my_prepare,
	.start = my_start,
	.ask = my_ask,
	.check = my_check,
	.halt = my_halt,
};

static int init_dev(void)
{
	if (!fwdev_register("ddev1", &mydev_ops))
		return -1;
	return 0;
}
fw_initcall(init_dev,10);
