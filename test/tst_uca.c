#include "test-head.h"
#include <pmon.h>
#include <cache.h>
#include <mem.h>

static int test_write(int *p, int len, int val)
{
	unsigned int cyc,tmp;

	printk("test %d word write @ %p:\n",len,p);

	pmon_prepare(PMON_EVENT_CYCLE);
	pmon_start();
	while (len --) {
		*p++ = val;
	}
	pmon_stop();

	pmon_get_cnt32(tmp,cyc);
	printk("\tused %d cycles.\n",cyc);
	return 0;
}
static int test_read(int *p, int len, int val)
{
	unsigned int err,cyc,tmp;

	printk("test %d word read @ %p:\n",len,p);

	pmon_prepare(PMON_EVENT_CYCLE);
	pmon_start();
	for (err = 0; len; len--, p++) {
		err += *p != val;
	}
	pmon_stop();

	pmon_get_cnt32(tmp,cyc);
	printk("\tused %d cycles, %d errors.\n",cyc,err);
	return 0;
}

#define CACK0_BASE	0x82000000
#define CACK0_SIZE	0x00080000
#define UNCAC_BASE	0xC1000000
#define UCA_BASE	0xC2000000

int test_uca(void)
{
	int *p_cac = (int*)CACK0_BASE;
	int *p_uncac = (int*)UNCAC_BASE;
	int *p_uca = (int*)UCA_BASE;
	int len = CACK0_SIZE / sizeof(int);
	int val;

	add_mem_range(CACK0_BASE, CACK0_SIZE, UNCAC_BASE, PG_UNCACHE);
	add_mem_range(CACK0_BASE, CACK0_SIZE, UCA_BASE, PG_UCA);

	blast_dcache();

	val = 0x5a5a5a5a;
	printk("test uncache access\n");
	test_write(p_uncac, len, val);
	test_read(p_uncac, len, val);

	val = 0x65a5a5a5;
	printk("test UCA access\n");
	test_write(p_uca, len, val);
	test_read(p_uca, len, val);

	blast_dcache();

	val = 0x5a5a5a57;
	printk("test cache access\n");
	test_write(p_cac, len, val);
	test_read(p_cac, len, val);

	remove_mem_range(UNCAC_BASE);
	remove_mem_range(UCA_BASE);

	return 0;
}

DEF_TEST(test_uca, 10, TESTF_REPEAT);
