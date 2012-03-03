#include "test-head.h"

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

int test_float1(void)
{
	asm_test();

	return 0;
}
DEF_TEST(test_float1, 10, (TESTF_REPEAT));
