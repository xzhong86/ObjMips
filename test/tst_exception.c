#include "test-head.h"

int test_exception(void)
{
	asm volatile (
		"	addiu	$2, $0, 2	\n"
		"	addiu	$3, $0, 3	\n"
		"	addiu	$4, $0, 4	\n"
		"	addiu	$5, $0, 5	\n"
		"	addiu	$6, $0, 6	\n"
		"	addiu	$7, $0, 7	\n"
		"	lw	$2, 1($29)	\n"
		"	ssnop			\n"
		);
	return 0;
}

DEF_TEST(test_exception, 10, (TESTF_2CORE));
