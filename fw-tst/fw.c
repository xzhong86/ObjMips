#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <smp.h>
#include <base.h>
#include <spinlock.h>
#include <command.h>

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
#define FLG_UNTEST	0x2
	unsigned int times;
	unsigned int hash;
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
		if (tdev->ops->reset)
			ret = tdev->ops->reset(&tdev->fwdev);
		if (!ret)
			ret = tdev->ops->start(&tdev->fwdev);
		if (ret) {
			printk("[FW] %s reset/start %d failed.\n",
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
			if (ret == CHK_FINISHED) {
				printk("[FW] %s test finish.\n",
				       tdev->fwdev.name);
				ret = 0;
			} else
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
		if (tdev->ops->release)
			tdev->ops->release(&tdev->fwdev);
		tdev->state = ST_STOPPED;
		break;

	case ST_STOPPED:
	default:
		break;
	}

	return ret;
}

static int fw_test_all(void)
{
	struct tdev *tdev;
	int err = 0, running;

	do {
		running = 0;
		for (tdev = tdev_head; tdev; tdev = tdev->next) {
			int cpu = smp_cpu_id();

			if (tdev->flags & FLG_UNTEST)
				continue;
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

			tdev->flags &= ~FLG_BUSY;
			spinlock_unlock(tdev->lock);
			if (err) 
				break;
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

static unsigned int get_str_hash(const char *str)
{
	unsigned int seed = 1313131; // 31 131 1313 13131 131313 etc..
	unsigned int hash = 0;
	while (*str)
		hash = hash * seed + (*str++);
	return (hash & 0x7FFFFFFF);
}
struct fw_dev *fwdev_register(const char *name, struct fw_ops *ops)
{
	struct tdev *tdev, **tpp;

	if (!ops || !ops->prepare || !ops->start || !ops->check
	    || !ops->stop)
		return NULL;

	tdev = malloc(sizeof(*tdev));
	if (!tdev)
		return NULL;
	memset(tdev, 0, sizeof(*tdev));
	name = name? name: "???";
	tdev->fwdev.name = name;
	tdev->fwdev.ops  = ops;
	tdev->fwdev.id   = tdev_num ++;
	tdev->hash = get_str_hash(name);
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

static void fw_reset_all(unsigned flags)
{
	struct tdev *tdev;

	for (tdev = tdev_head; tdev; tdev = tdev->next) {
		tdev->state = ST_INIT;
		tdev->flags = flags;
	}
}
static void fw_show_all(void)
{
	struct tdev *tdev;
	int n = 0;

	printk("All devices in FW test:\n");
	for (tdev = tdev_head; tdev; tdev = tdev->next)
		printk("%3d %s\n", n++, tdev->fwdev.name);
}
struct tdev *fw_get_by_name(const char *name)
{
	unsigned int hash = get_str_hash(name);
	struct tdev *tdev;

	for (tdev = tdev_head; tdev; tdev = tdev->next) 
		if (tdev->hash == hash 
		    && strcmp(name, tdev->fwdev.name) == 0)
			break;
	return tdev;
}

static void do_fw_help(char *str)
{
	printk("fw-tst all\nfw-tst show\n");
	printk("fw-tst dev0 dev1 ...\n");
}
extern int cache_probe(void);
static int do_fw(int argc,char *argv[])
{
	if (argc < 2) {
		do_fw_help(NULL);
		return 0;
	}

	if (strcmp(argv[1],"all") == 0) {
		fw_reset_all(0);
		fw_test_all();
	}
	else if (strcmp(argv[1],"show") == 0)
		fw_show_all();
	else {
		int i;
		fw_reset_all(FLG_UNTEST);
		for (i = 1; i < argc; i++) {
			struct tdev *tdev;
			tdev = fw_get_by_name(argv[i]);
			if (!tdev) {
				printk("[FW] unknown device %s\n",argv[i]);
				return -1;
			}
			tdev->flags = 0;
		}
		fw_test_all();
	}
	return 0;
}
DEF_COMMAND("fwtest",do_fw,do_fw_help);
