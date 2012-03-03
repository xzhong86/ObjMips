
struct serial_ops {
	int (*init)(void);
	void (*putc)(const char);
	void (*puts)(const char*);
	int (*getc)(void);
};

