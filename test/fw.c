#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <smp.h>
#include <smp_io.h>
#include <spinlock.h>

#include <test/fw.h>

/* test dev struction */
struct tdev {
	spinlock_t lock;
	enum {
		ST_INIT, ST_READY, ST_RUNNING,
		ST_FINISHED, ST_ERROR, ST_STOPPED,
	} state;
	unsigned long cpuallow;
	unsigned int flags;
#define FLG_BUSY	0x1
	unsigned int times;
	struct fw_dev fwdev;
	struct fw_ops *ops;
	struct tdev *next;
};
static struct tdev *tdev_head;
static int tdev_num;

/* state machine */
static int tdev_statm(struct tdev *tdev)
{
	int ret = 0;

	switch (tdev->state) {
	case ST_INIT:
		ret = tdev->ops->prepare(&tdev->fwdev);
		if (ret) {
			printk("[FW] %s prepare failed.\n",
			       tdev->fwdev.name);
			tdev->state = ST_STOPPED;
			break;
		}
		tdev->state = ST_READY;
		break;

	case ST_READY:
		ret = tdev->ops->start(&tdev->fwdev);
		if (ret) {
			printk("[FW] %s start %d failed.\n",
			       tdev->fwdev.name, tdev->times);
			tdev->state = ST_STOPPED;
			break;
		}
		tdev->times ++;
		tdev->state = ST_RUNNING;
		break;

	case ST_RUNNING:
		if (tdev->ops->ask)
			tdev->ops->ask(&tdev->fwdev);
		break;

	case ST_FINISHED:
		if(tdev->ops->finished)
			tdev->ops->finished(&tdev->fwdev);

		ret = tdev->ops->check(&tdev->fwdev);
		if (ret != CHK_PASSED) {
			if (ret == CHK_FINISHED)
				printk("[FW] %s test finish.\n",
				       tdev->fwdev.name);
			else
				printk("[FW] %s test %d failed.\n",
				       tdev->fwdev.name, tdev->times);
			tdev->ops->stop(&tdev->fwdev);
			tdev->state = ST_STOPPED;
			break;
		}
		tdev->state = ST_READY;
		break;

	case ST_ERROR:
		printk("[FW] %s errored at %d.\n",
		       tdev->fwdev.name, tdev->times);
		tdev->ops->stop(&tdev->fwdev);
		tdev->state = ST_STOPPED;
		break;

	case ST_STOPPED:
	default:
		break;
	}

	return ret;
}

int fw_test_loop(void)
{
	struct tdev *tdev;
	int err = 0, running;

	do {
		running = 0;
		for (tdev = tdev_head; tdev; tdev = tdev->next) {
			int cpu = smp_cpu_id();

			if (!(tdev->cpuallow & (1 << cpu)))
				continue;

			running += tdev->state != ST_STOPPED;
			if (tdev->flags & FLG_BUSY)
				continue;
			if (spinlock_trylock(tdev->lock)) {
				tdev->flags |= FLG_BUSY;
			} else {
				continue;
			}

			err = tdev_statm(tdev);
			if (err)
				break;

			tdev->flags &= ~FLG_BUSY;
			spinlock_unlock(tdev->lock);
		}
	} while (!err && running);

       	return err;
}

int fw_finish(struct fw_dev *fwdev)
{
	struct tdev *tdev = container_of(fwdev, struct tdev, fwdev);

	if (tdev->state != ST_RUNNING)
		return -1;
	tdev->state = ST_FINISHED;
	return 0;
}
int fw_error(struct fw_dev *fwdev)
{
	struct tdev *tdev = container_of(fwdev, struct tdev, fwdev);

	if (tdev->state != ST_RUNNING)
		return -1;
	tdev->state = ST_ERROR;
	return 0;
}

int fw_set_cpuallow(struct fw_dev *fwdev, unsigned long cpumask)
{
	struct tdev *tdev = container_of(fwdev, struct tdev, fwdev);

	tdev->cpuallow = cpumask;
	return 0;
}


struct fw_dev *fwdev_register(const char *name, struct fw_ops *ops)
{
	struct tdev *tdev, **tpp;

	tdev = malloc(sizeof(*tdev));
	if (!tdev)
		return NULL;
	memset(tdev, 0, sizeof(*tdev));
	tdev->fwdev.name = name;
	tdev->fwdev.ops  = ops;
	tdev->fwdev.id   = tdev_num ++;
	tdev->ops = ops;
	tdev->cpuallow = 0xffff;
	tdev->state = ST_INIT;

	for (tpp = &tdev_head; *tpp; tpp = &(*tpp)->next) ;
	*tpp = tdev;

	return &tdev->fwdev;
}
void fwdev_unregister(struct fw_dev *fwdev)
{
	struct tdev *tdev,**tpp;

	tdev = container_of(fwdev, struct tdev, fwdev);
	for (tpp = &tdev_head; *tpp; tpp = &(*tpp)->next)
		if (*tpp == tdev)
			break;
	if (*tpp) {
		*tpp = tdev->next;
		free(tdev);
		tdev_num --;
	}
}
