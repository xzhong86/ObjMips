#include <stdio.h>
#include <smp_stat.h>
#include <smp_io.h>
#include <base.h>
#include <cui.h>

static int nocui;
static void setup_nocui(char *arg)
{
	nocui = arg[0] - '0';
}
__setup("nocui", setup_nocui);

int old_test(void);
int run_tests(void);
int main()
{
	if (!nocui)
		cui_shell();
	run_tests();

	return 0;
}

