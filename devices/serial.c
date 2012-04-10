
#include <console.h>
#include "serial.h"

struct serial_ops *def_serial;

static struct console serial_console;

extern struct serial_ops serial_common;
int serial_init(void)
{
	def_serial = &serial_common;
	if (!def_serial->init()) {
		serial_console.getchar = def_serial->getc;
		serial_console.putchar = def_serial->putc;
		serial_console.putstring = def_serial->puts;
		register_console(&serial_console);
		return 0;
	}
	return -1;
}

void serial_put_string(const char *str)
{
	if(!def_serial)
		serial_init();
	def_serial->puts(str);
}

void serial_out_byte(char c)
{
	if(!def_serial)
		serial_init();
	def_serial->putc(c);
}
char serial_in_byte(void)
{
	if(!def_serial)
		serial_init();
	return def_serial->getc();
}


