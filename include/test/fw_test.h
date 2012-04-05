enum fw_state { ST_INIT,ST_READY,ST_RUNNING,ST_FINISHED,ST_ERROR,ST_STOPPED };

struct dev {
	int busy;
	enum fw_state state;
	struct fw_dev * fwdev;
	struct dev *next;
};

struct fw_dev {
	char * name;
	int id;
	int (*prepare)(struct fw_dev *);
	int (*start)(struct fw_dev *);
	int (*ask)(struct fw_dev *);
	int (*finish)(struct fw_dev *);
	int (*check)(struct fw_dev *);
	int (*reset)(struct fw_dev *);
	int (*stop)(struct fw_dev *);
	int (*error)(struct fw_dev *);
//	struct dev *dev;
	/* private data struct */
};
