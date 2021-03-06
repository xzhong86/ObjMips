
#ifndef __TEST_FW_H__
#define __TEST_FW_H__

struct fw_ops;
struct fw_dev {
	int id;
	const char *name;
	struct fw_ops *ops;
	void *priv;
};

typedef enum { 
	CHK_PASSED, CHK_FAILED, 
	/* return CHK_FINISHED will stop device without error */
	CHK_FINISHED, 
} chk_t;

struct fw_ops {
	/* called only once at first */
	int  (*prepare)(struct fw_dev *);

	/* always called before start test, could be NULL */
	int  (*reset)(struct fw_dev *);

	/* called to start one test */
	int  (*start)(struct fw_dev *);

	/* called after start, could be NULL */
	void (*ask)(struct fw_dev *);

	/* called after fw_finish() and before check, could be NULL */
	void (*finished)(struct fw_dev *);

	/* called after fw_finish() to check the test result, 
	 * if successed return 0. and failed return -1;
	 */
	chk_t (*check)(struct fw_dev *);

	/* stop test, stop device, force! */
	void (*halt)(struct fw_dev *);

	/* release resource after halt(), like free mem,unmap IO,
	   unregister IRQ, could be NULL if done in halt() */
	void (*release)(struct fw_dev *);
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


#endif
