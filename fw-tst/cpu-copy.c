
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <base.h>
#include <init.h>
#include <mem.h>
#include <test/fw.h>

#define MSIZE	(1024*1024)
#define BSIZE	(4*1024)
struct my_data {
	int times;
	unsigned int *mem;
	unsigned int size, pos;
};

static int cpuc_prepare(struct fw_dev *dev)
{
	struct my_data *md;

	md = malloc(sizeof(*md));
	if (!md)
		return -1;
	md->times = 0;
	md->mem = mem_alloc(MSIZE);
	if (!md->mem) {
		free(md);
		return -1;
	}
	md->size = MSIZE;
	md->pos = 1;
	dev->priv = md;
	return 0;
}

static unsigned get_int_hash(unsigned *arr, int num)
{
	unsigned hash = 0, i;

	for(i = 0; i < num; i++)
		hash = ((hash << 1) ^ arr[i]) + 5;
	return hash;
}
static int cpuc_start(struct fw_dev *dev)
{
	struct my_data *md = dev->priv;
	int i;

	md->times ++;
	md->pos = 1;
	for (i = 1; i < BSIZE/4; i++) 
		md->mem[i] = rand();
	md->mem[0] = get_int_hash(md->mem+1, BSIZE/4-1);
	return 0;
}

static void cpuc_ask(struct fw_dev *dev)
{
	struct my_data *md = dev->priv;
	unsigned int *p;

	p = md->mem + (BSIZE/4) * md->pos;
	memcpy(p, md->mem, BSIZE);
	p[0] = get_int_hash(p+1, BSIZE/4-1);
	if (++md->pos >= MSIZE/BSIZE)
		fw_finish(dev);
}

static chk_t cpuc_check(struct fw_dev *dev)
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

static void cpuc_halt(struct fw_dev *dev)
{
	struct my_data *md = dev->priv;

	if (md->times <= 10)
		printk("no, i stopped at %d\n", md->times);
	else
		printk("o, i stopped\n");
	mem_free(md->mem);
	free(md);
}

static struct fw_ops cpuc_ops = {
	.prepare = cpuc_prepare,
	.start = cpuc_start,
	.ask = cpuc_ask,
	.check = cpuc_check,
	.halt = cpuc_halt,
};

static int init_dev(void)
{
	if (!fwdev_register("cpucopy", &cpuc_ops))
		return -1;
	return 0;
}
fw_initcall(init_dev,0);
