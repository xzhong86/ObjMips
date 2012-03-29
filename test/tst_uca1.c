#include "test-head.h"
#include <pmon.h>
#include <cache.h>
#include <mem.h>
#include <string.h>
#include <stdlib.h>

static int test_copy(int *dst, int *src, int len)
{
	unsigned int cyc,tmp;

	printk("test %d word copy %p <= %p:",len,dst,src);

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
	int i, opt = a->nspc, spc = a->nopt;
	int addr = 0;
	for (i = 0; i < a->nspc + a->nopt; i++) {
		if (opt < spc) {
			if (a->flags & FLG_COPY) {
				*buf++ = INST_LW(2, addr, 5);
				*buf++ = INST_SW(2, addr, 4);
			} else {
				/* sw a2,addr(a0) */
				*buf++ = INST_SW(6, addr, 4);
			}
			opt += a->nspc;
			addr += 4;
		} else {
			if (a->flags & FLG_INS_READ)
				/* lw v0, addr(a1) */
				*buf++ = INST_LW(2, addr, 5);
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

typedef void (*write_fun_t)(int *addr, int *rd, int val);
static void mywrite(int nwrt, int nspc, int ins_read,
		    int *dst, int *rd, int val, int len)
{
	unsigned int i,cyc,tmp;
	write_fun_t write_fun;
	struct build_arg arg;
	
	arg.len = (nwrt+nspc+8)*sizeof(int);
	arg.buf = (unsigned int*)malloc(arg.len);
	arg.nopt = nwrt;
	arg.nspc = nspc;
	arg.flags = 0;
	if (ins_read)
		arg.flags = FLG_INS_READ;
	build_fun(&arg);

	write_fun = (write_fun_t)arg.buf;

	printk("mywrite_%d_%d %p:",nwrt,nspc,dst);
	pmon_prepare(PMON_EVENT_CYCLE);
	pmon_start();

	while (len > nwrt) {
		write_fun(dst, rd, val);
		len -= nwrt;
		dst += nwrt;
		rd  += nwrt;
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

	printk("mycopy_%d_%d %p <= %p:",ncpy,nspc,dst,src);
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

static void crosscheck(int *dst, int *crs0, int *crs1, int val, int len)
{
	unsigned int i, err = 0;
	int v0, v1, *d, *c0, *c1;

	printk("crosscheck %p %x", dst, val);

	val = 0x5a8796a5;
	v0 = crs0[0]; v1 = crs1[0];
	c0 = crs0; c1 = crs1;
	for (i = 0; i < len; ) {
		int **p,v,j,r = rand()%64;
		j = len - i > r ? len - i - 1 : r - 1;
		d = dst + i;
		for (; j >= 0; j--)
			d[j] = val;
		i += r;
		if (i >= len)
			break;

		/* insert operation */
		r = rand()%64;
		if (r & 1) {
			p = &c1;
			v = v1;
		} else {
			p = &c0;
			v = v0;
		}
		j = r & 2;
		r = r >> 2;
		if (j) {
			/* insert read check */
			for (j = 0; j < r; j++)
				err += (*p)[j] != v;
		} else {
			/* insert write */
			for (j = 0; j < r; j++)
				(*p)[j] = val;
			*p += r;
		}
	}
	for (i = 0; i < len; i ++)
		err += dst[i] != val;

	for (i = 0; i < c0 - crs0; i ++)
		err += crs0[i] != val;
	for (i = 0; i < c0 - crs0; i ++) /* store back */
		crs0[i] = v0;

	for (i = 0; i < c1 - crs1; i ++)
		err += crs1[i] != val;
	for (i = 0; i < c1 - crs1; i ++) /* store back */
		crs1[i] = v1;

	printk("\terror %d .\n", err);
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

	mywrite(64, 0, 0, p_src, p_src, 0x5a5aa5a5, len);
	test_copy(p_uncac, p_src, len);
	test_copy(p_uca,   p_src, len);
	test_copy(p_cac,   p_src, len);

	for (i = 0; i < 120; i += 8) 
		mywrite(128 - i, i, 0, p_uca, p_uca, 0x5aa55aa5, len);

	for (i = 1; i < 65; i *= 2) 
		mywrite(64, i, 1, p_uca, p_uca, 0x5aa55aa5, len);

	for (i = 1; i < 65; i *= 2) 
		mywrite(64, i, 1, p_uca, p_src, 0x5aa55aa5, len);

	for (i = 4; i < 64; i += 6) 
		mycopy(64 - i, i*2, p_cac, p_src, len);

	//for (i = 0; i < 8; i ++) 
	//	crosscheck(p_uca, p_cac, p_uncac, rand(), len);

	remove_mem_range(UNCAC_BASE);
	remove_mem_range(UCA_BASE);
	remove_mem_range(CAC_BASE);

	return 0;
}

DEF_TEST(test_uca1, 10, TESTF_REPEAT);
