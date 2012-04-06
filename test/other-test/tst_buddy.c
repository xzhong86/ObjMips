
#include <stdio.h>
#include <smp_io.h>

#include <mem.h>
#include <test/test-head.h>


static int buddy(void)
{
	struct page *pg0,*pg1,*pg2;

	mem_dump_buddy();
	pg0 = mem_get_pages(1, 0);
	mem_dump_buddy();
	pg1 = mem_get_pages(4, 0);
	mem_dump_buddy();
	mem_free_pages(pg0, 1);
	mem_dump_buddy();
	mem_free_pages(pg1, 4);
	mem_dump_buddy();

	pg0 = mem_get_pages(17, 0);
	mem_dump_buddy();
	pg1 = mem_get_pages(33, 0);
	mem_dump_buddy();
	pg2 = mem_get_pages(257, 0);
	mem_dump_buddy();
	mem_free_pages(pg0, 17);
	mem_dump_buddy();
	mem_free_pages(pg1, 33);
	mem_dump_buddy();
	mem_free_pages(pg2, 257);
	mem_dump_buddy();

	return 0;
}
DEF_TEST(buddy, 30, TESTF_REPEAT);
