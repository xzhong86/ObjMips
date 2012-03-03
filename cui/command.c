#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <smp_io.h>
#include <command.h>

static struct __command_entity **cmds_tbl;
static int cmds_num;

int register_command(char *name,command_fun_t fun)
{
	return 0;
}
int unregister_command(char *name)
{
	return 0;
}

static inline unsigned int get_str_hash(const char *str,int n)
{
	unsigned int seed = 1313131; // 31 131 1313 13131 131313 etc..
	unsigned int hash = 0;
	while (*str && n--)
		hash = hash * seed + (*str++);
	return (hash & 0x7FFFFFFF);
}

extern struct __command_entity __command_entity_start[];
extern struct __command_entity __command_entity_stop[];
static int static_cmds;
static int init_commands(void)
{
	struct __command_entity **cmds;
	int i,n;
	n = __command_entity_stop - __command_entity_start;
	if (n == 0)
		return 0;
	cmds = (typeof(cmds))malloc(sizeof(*cmds)*(cmds_num+n));
	for (i = 0; i < cmds_num; i++)
		cmds[i] = cmds_tbl[i];
	for (; i < cmds_num + n; i++) {
		struct __command_entity *cmd;
		cmd = &__command_entity_start[i-cmds_num];
		cmds[i] = cmd;
		cmd->hash = get_str_hash(cmd->name,64);
	}
	cmds_num += n;
	if (cmds_tbl)
		free(cmds_tbl);
	cmds_tbl = cmds;
	static_cmds = n;
	return cmds_num;
}
static struct __command_entity *find_cmd_by_name(const char *name)
{
	struct __command_entity *cmd;
	unsigned int i,hash;

	hash = get_str_hash(name,64);
	for (i = 0; i < cmds_num; i++) {
		cmd = cmds_tbl[i];
		if (cmd->hash != hash)
			continue;
		if (strcmp(cmd->name,name) == 0)
			return cmd;
	}
	return NULL;
}
int do_command(int argc,char *argv[])
{
	struct __command_entity *cmd;
	int ret = 0;

	if (static_cmds == 0)
		init_commands();
	if (argc < 1)
		return 0;
	if (strcmp(argv[0],"exit") == 0)
		return -1;

	cmd = find_cmd_by_name(argv[0]);
	if (cmd) {
		if (cmd->fun) {
			ret = cmd->fun(argc,argv);
			if (ret)
				printk("command %s failed %d\n",cmd->name,ret);
		} else {
			printk("command %s has NULL fun\n",cmd->name);
		}
	} else {
		printk("Bad command: %s\n",argv[0]);
	}
	return 0;
}
int complete_command(char *str)
{
	return 0;
}


static int cmd_help(int argc,char *argv[])
{
	struct __command_entity *cmd;
	int i;
	if (argc > 1) {
		cmd = find_cmd_by_name(argv[1]);
		if (cmd) {
			if (cmd->help)
				cmd->help(argv[2]);
			else
				printk("Command %s has no help\n",cmd->name);
			return 0;
		} else {
			printk("unknown command %s\n",argv[1]);
		}
	}

	printk("Supported commands:\n");
	for (i = 0; i < cmds_num; i++)
		printk("  %s\n",cmds_tbl[i]->name);
	return 0;
}
DEF_COMMAND("help",cmd_help,NULL);
