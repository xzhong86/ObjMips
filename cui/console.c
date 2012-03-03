#include <console.h>

static struct console *def_console;
int register_console(struct console *con)
{
	def_console = con;
	return 0;
}

struct console *get_defconsole(void)
{
	return def_console;
}
