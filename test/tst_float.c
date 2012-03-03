#include "test-head.h"
#include <string.h>

#define G 	9.8
#define Af	2.5		/* air f, f = Af*v^2*r^2 */

struct kanon {
	float x,y,r,m;
	double vx,vy,ax,ay;
};

static int calc_kanon(struct kanon *kan,float scale)
{
	int i;
	
	for (i = 0; kan->y > kan->r; i++) {
		kan->x += kan->vx * scale;
		kan->y += kan->vy * scale;
		kan->vx += kan->ax * scale;
		kan->vy += kan->ay * scale;
		kan->ax = kan->vx*kan->vx*kan->r*kan->r*Af/kan->m;
		if (kan->vx > 0)
			kan->ax = -kan->ax;
		kan->ay = kan->vy*kan->vy*kan->r*kan->r*Af/kan->m;
		if (kan->vy > 0)
			kan->ay = -kan->ay;
		kan->ay -= G;
	}
	return i;
}
static void print_kanon(struct kanon *kan)
{
	printk("m:%.2f r:%.f  x:%f  y:%f\n",kan->m,kan->r,kan->x,kan->y);
	printk("\tvx:%f  vy:%f\n",kan->vx,kan->vy);
	printk("\tax:%f  ay:%f\n",kan->ax,kan->ay);
}

#if 0
static int
find_N(int enough)
{
	int		tries;
	static long	N = 10000;
	static long long usecs = 0;
	usecs += 100;
	if(usecs > enough)
		usecs = 1;
	printk("enough %d\n",enough);
	for (tries = 0; tries < 10; ++tries) {
		if (0.98 * enough < usecs && usecs < 1.02 * enough)
			return (N);
		if (usecs < 1000)
			N *= 10;
		else {
			double  n = N;
//			asm volatile ("break 0x18");
//			printf("\t\t\t\t\t  n:%f \n",n);
			n /= usecs;
//			printf("\t\t\t\t\t  n:%f N:%u usecs:%lld enough:%d\n",n,N,usecs,enough);
			n *= enough;
//			printf("\t\t\t\t\t  n:%f N:%u usecs:%lld enough:%d\n",n,N,usecs,enough);
			N = n + 1;
//			printf("\t\t\t\t\t  n:%f \n",n);
//			asm volatile ("break 0x19");
		}
		usecs += 150;
		printk(" N:%lu usecs:%lld\n",N,usecs);
	}
	return (tries);
}
#endif

static void asm_test(void)
{
	double df2;
	asm volatile (
		"	la	$8, 0x2710	\n"
		"	mtc1	$8, $f4		\n"
		"	mtc1	$0, $f5		\n"
		"	la	$8, 0x1071	\n"
		"	mtc1	$8, $f0		\n"
		"	mtc1	$0, $f1		\n"
		"	cvt.d.w	$f0, $f0	\n"
		"	bgez	$2, 1f		\n"
		"	cvt.d.w	$f2, $f4	\n"
		"	add.d	$f2, $f2, $f24	\n"
		"1:				\n"
		"	div.d	$f2, $f2, $f0	\n"
		"	addiu	$4, $0, 0	\n"
		"	s.d	$f2, %0		\n"
		::"m"(df2)
		:"$8","$4");
	printk("%llx\n",(long long)df2);
}

int test_float(void)
{
	struct kanon kanon,tmp;
	int ret;

	asm_test();

	memset(&kanon,0,sizeof(kanon));
	kanon.r = 0.05;
	kanon.m = 1;

	kanon.x = 0;
	kanon.y = 5;
	memcpy(&tmp,&kanon,sizeof(kanon));
	ret = calc_kanon(&tmp, 0.001);
	printk("ret = %d\n",ret);
	print_kanon(&tmp);

	kanon.x = 0;
	kanon.y = 20;
	memcpy(&tmp,&kanon,sizeof(kanon));
	ret = calc_kanon(&tmp, 0.001);
	printk("ret = %d\n",ret);
	print_kanon(&tmp);

	kanon.x = 0;
	kanon.y = 2;
	kanon.vx = 100;
	kanon.vy = 100;
	memcpy(&tmp,&kanon,sizeof(kanon));
	ret = calc_kanon(&tmp, 0.001);
	printk("ret = %d\n",ret);
	print_kanon(&tmp);

#if 0
	ret = find_N(1000);
	printk("ret = %d\n",ret);
	ret = find_N(2000);
	printk("ret = %d\n",ret);
	ret = find_N(4000);
	printk("ret = %d\n",ret);
#endif
	return 0;
}
DEF_TEST(test_float, 10, (TESTF_REPEAT));
