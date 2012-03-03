#include "test-head.h"
#include <pmon.h>
#include <cache.h>
#include <mem.h>
#include <string.h>
#include <stdlib.h>

static int test_copy(int *dst, int *src, int len)
{
	unsigned int cyc,tmp;

	printk("test %d word copy %p <= %p:\n",len,dst,src);

	pmon_prepare(PMON_EVENT_CYCLE);
	pmon_start();
	memcpy(dst, src, len*(sizeof(int)));
	pmon_stop();

	pmon_get_cnt32(tmp,cyc);
	printk("\tused %d cycles.\n",cyc);
	return 0;
}

#include <mips_inst.h>
struct build_arg {
	unsigned int *buf, len;
	int nopt, nspc;
	int flags;
#define FLG_COPY	0x01
#define FLG_INS_READ	0x10
};
static int build_fun(struct build_arg *a)
{
	unsigned int *buf = a->buf;
	int i, w, opt = a->nspc, spc = a->nopt;
	int addr = 0;
	for (i = 0; i < a->nspc + a->nopt; i++) {
		if (opt < spc) {
			if (a->flags & FLG_COPY) {
				*buf++ = INST_LW(2, addr, 5);
				*buf++ = INST_SW(2, addr, 4);
			} else {
				/* sw a1,addr(a0) */
				*buf++ = INST_SW(5, addr, 4);
			}
			opt += a->nspc;
			addr += 4;
		} else {
			if (a->flags & FLG_INS_READ)
				/* lw v0, addr(a0) */
				*buf++ = INST_LW(2, addr, 4);
			else
				*buf++ = INST_NOP;
			spc += a->nopt;
		}
	}
	*buf++ = INST_JR(31);
	*buf++ = INST_NOP;
	{
		unsigned int start = (unsigned int)a->buf;
		unsigned int end = start + a->len;
		flush_dcache_range(start, end);
		flush_icache_range(start, end);
	}

	return 0;
}

typedef void (*write_fun_t)(int *addr, int val);
static void mywrite(int nwrt, int nspc,
		    int *dst, int val, int len)
{
	unsigned int i,cyc,tmp;
	write_fun_t write_fun;
	struct build_arg arg;
	
	arg.len = (nwrt+nspc+8)*sizeof(int);
	arg.buf = (unsigned int*)malloc(arg.len);
	arg.nopt = nwrt;
	arg.nspc = nspc;
	arg.flags = FLG_INS_READ;
	build_fun(&arg);

	write_fun = (write_fun_t)arg.buf;

	printk("mywrite_%d_%d %p:\n",nwrt,nspc,dst);
	pmon_prepare(PMON_EVENT_CYCLE);
	pmon_start();

	while (len > nwrt) {
		write_fun(dst, val);
		len -= nwrt;
		dst += nwrt;
	}
	for (i = 0; i < len; i++)
		*dst++ = val;

	pmon_stop();
	pmon_get_cnt32(tmp,cyc);
	printk("\tused %d cycles.\n",cyc);
	free(arg.buf);
}

typedef void (*copy_fun_t)(int *dst, int *src);
static void mycopy(int ncpy, int nspc,
		   int *dst, int *src, int len)
{
	unsigned int i,cyc,tmp;
	copy_fun_t copy_fun;
	struct build_arg arg;
	
	arg.len = (ncpy*2+nspc+8)*sizeof(int);
	arg.buf = (unsigned int*)malloc(arg.len);
	arg.nopt = ncpy;
	arg.nspc = nspc;
	arg.flags = FLG_COPY;
	build_fun(&arg);

	copy_fun = (copy_fun_t)arg.buf;

	printk("mycopy_%d_%d %p <= %p:\n",ncpy,nspc,dst,src);
	pmon_prepare(PMON_EVENT_CYCLE);
	pmon_start();

	while (len > ncpy) {
		copy_fun(dst, src);
		dst += ncpy;
		src += ncpy;
		len -= ncpy;
	}
	for (i = 0; i < len; i++)
		*dst++ = *src++;

	pmon_stop();
	pmon_get_cnt32(tmp,cyc);
	printk("\tused %d cycles.\n",cyc);
	free(arg.buf);
}

#define MBLK_BASE	0x82000000
#define MBLK_SIZE	0x00080000
#define UNCAC_BASE	0xC1000000
#define UCA_BASE	0xC2000000
#define CAC_BASE	0xC3000000

int test_uca1(void)
{
	int *p_src = (int*)MBLK_BASE;
	int *p_cac = (int*)CAC_BASE;
	int *p_uncac = (int*)UNCAC_BASE;
	int *p_uca = (int*)UCA_BASE;
	int len = MBLK_SIZE / sizeof(int);
	unsigned long base = MBLK_BASE;
	int i;

	base = base + MBLK_SIZE;
	add_mem_range(base, MBLK_SIZE, UNCAC_BASE, PG_UNCACHE);
	base = base + MBLK_SIZE;
	add_mem_range(base, MBLK_SIZE, UCA_BASE, PG_UCA);
	base = base + MBLK_SIZE;
	add_mem_range(base, MBLK_SIZE, CAC_BASE, PG_NORMAL);

	test_copy(p_uncac, p_src, len);
	test_copy(p_uca,   p_src, len);
	test_copy(p_cac,   p_src, len);

	for (i = 4; i <= 64; i += 4) 
		mywrite(64 - i, i, p_uca, 0x5aa55aa5, len);

	for (i = 4; i <= 64; i += 4) 
		mycopy(64 - i, i*2, p_cac, p_src, len);

	remove_mem_range(UNCAC_BASE);
	remove_mem_range(UCA_BASE);
	remove_mem_range(CAC_BASE);

	return 0;
}

DEF_TEST(test_uca1, 10, TESTF_REPEAT);
