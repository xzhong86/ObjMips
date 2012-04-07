#include <stdlib.h>
#include <string.h>
#include <base.h>

#define isspace(c) ((c) == ' ' || (c) == '\t')

unsigned long fw_arg_arr[4] = { 0,1,1,0 };
static char **pass_argv,*pass_args;
static int pass_argc;
static void get_args(void)
{
	int argc = fw_arg_arr[0];
	char **argv = (char**)fw_arg_arr[1];
	int i,n,len;
	if (argc == 0)
		return;

	for (i = 0,len = 0; i < argc; i++)
		if (argv[i])
			len += strlen(argv[i]) + 1;
	pass_args = (char*)malloc(++len);
	for (i = 0,n = 0; i < argc; i++) {
		char *p = argv[i];
		if (!p)
			continue;
		while(*p)
			pass_args[n++] = *p++;
		pass_args[n++] = ' ';
	}
	pass_args[n++] = 0;
	for (i = 0,n = 0; i < len; ) {
		if (!isspace(pass_args[i])) {
			n ++;
			while(!isspace(pass_args[i]) && i < len) i++;
		}
		while(isspace(pass_args[i])) i++;
	}
	n++;
	pass_argv = (char**)malloc(sizeof(char*)*n);
	for (i = 0,n = 0; i < len; ) {
		if (!isspace(pass_args[i])) {
			pass_argv[n++] = &pass_args[i];
			while(!isspace(pass_args[i]) && i < len) i++;
			pass_args[i++] = 0;
		}
		while(isspace(pass_args[i]) && i < len) i++;
	}
	pass_argc = n;
	return;
}

static inline unsigned int get_str_hash(const char *str,int n)
{
	unsigned int seed = 1313131; // 31 131 1313 13131 131313 etc..
	unsigned int hash = 0;
	while (*str && n--)
		hash = hash * seed + (*str++);
	return (hash & 0x7FFFFFFF);
}
extern struct __setup_entity __setup_entity_start[];
extern struct __setup_entity __setup_entity_stop[];
static struct __setup_entity *setups;
static int setups_num;
static void do_param(char *p,int len,char *arg)
{
	unsigned int i,hash;
	hash = get_str_hash(p,len);
	for (i = 0; i < setups_num; i++)
		if (setups[i].hash == hash) {
			setups[i].fun(arg);
			break;
		}
}

void param_init(void)
{
	struct __setup_entity *e;
	int i,j,n;

	setups = __setup_entity_start;
	setups_num = __setup_entity_stop - __setup_entity_start;
	n = setups_num;

	printk("param_init: %lx %lx %lx %lx\n",
	       fw_arg_arr[0],fw_arg_arr[1],fw_arg_arr[2],fw_arg_arr[3]);
	for (e = setups; e < setups+n; e++) {
		e->hash = get_str_hash(e->name, 128);
	}

	get_args();
	printk("pass argument:");
	for (i = 0; i < pass_argc; i++)
		printk(" %s", pass_argv[i]);
	printk("\n");
	for (i = 0; i < pass_argc; i++) {
		char *s = pass_argv[i];
		for (j = 0; s[j]; j++)
			if (s[j] == '=') {
				do_param(s,j,s+j+1);
				break;
			}
	}
}
