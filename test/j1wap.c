#include <stdio.h>
#include <stdlib.h>

#include <iomap.h>
#include <cache.h>
#include <test/test-head.h>

#define DEV_REG_BASE 0

static unsigned ddr_single_val;
static unsigned *ddr_single_ptr;
static void ddr_single_write(void)
{
	static int pos = 0;
	static unsigned arr[32];
	ddr_single_val ^= rand();
	ddr_single_ptr = (unsigned*)K0_TO_K1(&arr[pos]);
	*ddr_single_ptr = ddr_single_val;
	pos = (pos + 1) % 32;
}
static int ddr_single_check(void)
{
	return *ddr_single_ptr != ddr_single_val;
}

static unsigned ddr_burst_val;
static unsigned *ddr_burst_ptr;
static void ddr_burst_write(void)
{
	static int pos = 0;
	static unsigned arr[2][64];
	unsigned val = 0, i;
	ddr_burst_ptr = &arr[pos][0];
	pos = pos ^ 1;
	for (i = 0; i < 64; i++) {
		ddr_burst_ptr[i] ^= rand();
		val ^= ddr_burst_ptr[i];
	}
	ddr_burst_val = val;
	flush_dcache_ptr(ddr_burst_ptr, 64);
}
static int ddr_burst_check(void)
{
	unsigned val = 0, i;
	for (i = 0; i < 64; i++) {
		val ^= ddr_burst_ptr[i];
	}
	return ddr_burst_val != val;
}

#define PDMA_DCSM0	0x13422000
#define PDMA_DCSM1	0x13422400
static unsigned ahb2_single_val;
static unsigned *ahb2_single_ptr;
static void ahb2_single_write(void)
{
	static int pos = 0;
	ahb2_single_val ^= rand();
	ahb2_single_ptr = (unsigned*)PHYS_TO_K1(PDMA_DCSM0) + pos;
	*ahb2_single_ptr = ahb2_single_val;
	pos = (pos + 1) % 32;
}
static int ahb2_single_check(void)
{
	return *ahb2_single_ptr != ahb2_single_val;
}

static unsigned ahb2_burst_val;
static unsigned *ahb2_burst_ptr;
static void ahb2_burst_write(void)
{
	static int pos = 0;
	unsigned val = 0, i;
	ahb2_burst_ptr = (unsigned*)PHYS_TO_K0(PDMA_DCSM1) + pos*64;
	pos = pos ^ 1;
	for (i = 0; i < 64; i++) {
		ahb2_burst_ptr[i] ^= rand();
		val ^= ahb2_burst_ptr[i];
	}
	ahb2_burst_val = val;
	flush_dcache_ptr(ahb2_burst_ptr, 64);
}
static int ahb2_burst_check(void)
{
	unsigned val = 0, i;
	for (i = 0; i < 64; i++) {
		val ^= ahb2_burst_ptr[i];
	}
	return ahb2_burst_val != val;
}


#define BUILD_R_FUN(prefix, ioaddr)					\
	static unsigned long prefix##_r_val;				\
	static void prefix##_register_write(void)			\
	{								\
		prefix##_r_val ^= rand();				\
		REG32(PHYS_TO_K1(ioaddr)) = prefix##_r_val;		\
	}								\
	static int prefix##_register_check(void)			\
	{								\
		return REG32(PHYS_TO_K1(ioaddr)) != prefix##_r_val;	\
	}
BUILD_R_FUN(ahb0, 0x13060028)		/* CIM CIMFID */
BUILD_R_FUN(ahb2, 0x13420000)		/* PDMA DSA0 */
BUILD_R_FUN(vpu, 0x13290014)		/* VPU cbzhang */
BUILD_R_FUN(ost, 0x120000e0)		/* OST OSTDR */

static struct {
	int wrote;
	void (*write)(void);
	int (*check)(void);	/* read and check */
} chk_funs[] = {
#define CHKFUN(prefix)	{ 0, prefix##_write, prefix##_check }
	CHKFUN(ddr_single),
	CHKFUN(ddr_burst),
	CHKFUN(ahb0_register),
	CHKFUN(ahb2_single),
	CHKFUN(ahb2_burst),
	CHKFUN(ahb2_register),
//	CHKFUN(vpu_register),
	CHKFUN(ost_register),
};
#define NUM (sizeof(chk_funs)/sizeof(chk_funs[0]))
static int j1wap(void)
{
	int r,t;
	for (t = 0; t < 10000*NUM; t++) {
		r = rand() % NUM;
		if (chk_funs[r].wrote) {
			if (chk_funs[r].check())
				break;
			chk_funs[r].wrote = 0;
		} else {
			chk_funs[r].write();
			chk_funs[r].wrote = 1;
		}
	}
	if (t != 10000) {
		printk("err when check %d.\n", r);
		return -1;
	}
	return 0;
}
DEF_TEST(j1wap, 30, TESTF_REPEAT);
