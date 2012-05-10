
#ifndef __CONSOLE_H__
#define __CONSOLE_H__

struct console {
	int (*getchar)(void);
	void (*putstring)(const char *);
	int (*putchar)(char);

	/* gaved by console in register function */
	void (*rx_char)(struct console *, char);
	void (*tx_end)(struct console *);
};

enum console_type { CONSOLE_TYPE_DUMMY, CONSOLE_TYPE_FIFO };
int register_console(struct console *con, enum console_type);
struct console *get_defconsole(void);

#endif
