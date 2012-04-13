
#ifndef __KTIME_H__
#define __KTIME_H__

#define HZ	10

/* get time in nano second */
unsigned long long sched_clock(void);

unsigned long jiffies;

typedef struct ktime {
	unsigned long sec, nsec;
} ktime_t;

ktime_t *ktime_gettime(ktime_t *);

#endif
