
#ifndef __FW_H__
#define __FW_H__

struct fw_ops;
struct fw_dev {
	int id;
	const char *name;
	struct fw_ops *ops;
	void *priv;
};

struct fw_ops {
	/* called only once at first */
	int  (*prepare)(struct fw_dev *);

	/* called to start one test */
	int  (*start)(struct fw_dev *);

	/* called after start, could be NULL */
	void (*ask)(struct fw_dev *);

	/* called after fw_finish() and before check, could be NULL */
	void (*finished)(struct fw_dev *);

	/* called after fw_finish() to check the test result, 
	 * if successed return 0. and failed return -1;
	 */
	int  (*check)(struct fw_dev *);

	/* stop test, stop device, force! */
	void (*stop)(struct fw_dev *);
};

/* called when test case finished */
int fw_finish(struct fw_dev *fwdev);
/* called when test case error occured */
int fw_error(struct fw_dev *fwdev);

/* set cpu mask which allowed to invoke the function in fw_ops,
 * cpumask = 5 means allow CPU0/CPU2 to invoke.
 */
int fw_set_cpuallow(struct fw_dev *fwdev, unsigned long cpumask);

struct fw_dev *fwdev_register(const char *name, struct fw_ops *ops);
void fwdev_unregister(struct fw_dev *fwdev);


/* test framework main loop */
int fw_test_loop(void);

#endif
