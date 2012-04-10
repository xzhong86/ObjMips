
#include <stdio.h>
#include <stdlib.h>

#include <base.h>
#include <init.h>
#include <test/fw.h>

struct my_data {
	int times;
	int n;
#define ARR_NUM 100
	int arr[ARR_NUM];
};

static int my_prepare(struct fw_dev *dev)
{
	struct my_data *md;

	md = malloc(sizeof(*md));
	md->times = 0;
	dev->priv = md;
	return 0;
}

static int my_start(struct fw_dev *dev)
{
	struct my_data *md = dev->priv;
	int i;

	md->n = 0;
	for (i = 0; i < ARR_NUM; i++)
		md->arr[i] = 0;
	return 0;
}

static void my_ask(struct fw_dev *dev)
{
	struct my_data *md = dev->priv;

	md->arr[md->n] = md->n;
	if (++md->n >= ARR_NUM)
		fw_finish(dev);
}

static chk_t my_check(struct fw_dev *dev)
{
	struct my_data *md = dev->priv;
	int i;

	if (md->n != ARR_NUM)
		return CHK_FAILED;
	for (i = 0; i < ARR_NUM; i++)
		if (md->arr[i] != i)
			return CHK_FAILED;

	if (md->times ++ > 10)
		return CHK_FINISHED;
	return CHK_PASSED;
}

static void my_stop(struct fw_dev *dev)
{
	struct my_data *md = dev->priv;

	if (md->times <= 10)
		printk("no, i stopped at %d\n", md->times);
	else
		printk("o, i stopped\n");
	free(md);
}

static struct fw_ops mydev_ops = {
	.prepare = my_prepare,
	.start = my_start,
	.ask = my_ask,
	.check = my_check,
	.stop = my_stop,
};

static int init_dev(void)
{
	if (!fwdev_register("ddev1", &mydev_ops))
		return -1;
	return 0;
}
fw_initcall(init_dev,100);
