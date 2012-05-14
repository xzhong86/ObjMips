/* 
 * CUI is Command based User Interface 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <base.h>
#include <console.h>
#include <signal.h>
#include <command.h>

enum vtctl {
	VT_BAD_CTL,
	VT_CURSOR_UP,VT_CURSOR_DOWN,
	VT_CURSOR_LEFT,VT_CURSOR_RIGHT,
};
static enum vtctl get_vtctl(struct console *con,char cc[2])
{
	cc[0] = con->getchar();
	cc[1] = con->getchar();
	if (cc[0] == '[') {
		switch (cc[1]) {
		case 'A': return VT_CURSOR_UP;
		case 'B': return VT_CURSOR_DOWN;
		case 'C': return VT_CURSOR_RIGHT;
		case 'D': return VT_CURSOR_LEFT;
		}
	}
	return VT_BAD_CTL;
}
static void put_vtctl(struct console *con,char cc[2])
{
	con->putchar('\033');
	con->putchar(cc[0]);
	con->putchar(cc[1]);
}
static void insert_char(char buf[],int n,char ch,int p)
{
	while (n > p) {
		buf[n] = buf[n-1];
		n --;
	}
	buf[n] = ch;
}
static void delete_char(char buf[],int n,int p)
{
	while (n > p) {
		buf[p] = buf[p+1];
		p ++;
	}
}
#define PROMPT  "CMD: "
static void fresh_str(struct console *con,char buf[],int n,int p)
{
	int i;
	con->putchar('\r');
	con->putstring(PROMPT);
	for (i = 0; i < n; i++)
		con->putchar(buf[i]);
	con->putchar(' ');
	con->putchar('\b');
	for (i = 0; i < n-p; i++)
		con->putchar('\b');
}
static char *mystrdup(char *str)
{
	char *p = (char*)malloc(strlen(str)+1);
	strcpy(p,str);
	return p;
}
#define CMDBUF_LEN 8
static char *cmdbuf[CMDBUF_LEN];
static int cmdbuf_p = CMDBUF_LEN;
static int get_cmdline(struct console *con,char *buf,int len)
{
	char cch[2];
	int n, p, cmdp;

	n = 0; p = 0; cmdp = cmdbuf_p;
	con->putstring(PROMPT);
	while (1) {
		int fresh = 0;
		char *cmd;
		char ch = con->getchar();
		switch (ch) {
		case '\r':
			con->putchar('\r');
			con->putchar('\n');
			buf[n] = 0;
			if (n > 0) {
				cmd = cmdbuf[(cmdbuf_p-1) %CMDBUF_LEN];
				if (!cmd || strcmp(cmd,buf)) {
					cmdp = cmdbuf_p % CMDBUF_LEN;
					if (cmdbuf[cmdp])
						free(cmdbuf[cmdp]);
					cmdbuf[cmdp] = mystrdup(buf);
					cmdbuf_p ++;
				}
			}
			return n;
		case '\b':
			if (p > 0) {
				delete_char(buf,n--,--p);
				fresh = 1;
			}
			break;
		case '\177': 
			if (p < n) {
				delete_char(buf,n--,p);
				fresh = 1;
			}
			break;
		case '\033': 
			switch (get_vtctl(con,cch)) {
			case VT_BAD_CTL:
				printk("bad vt ctl: %c%c\n",cch[0],cch[1]);
				break;
			case VT_CURSOR_UP:
				cmdp --;
				cmd = cmdbuf[cmdp % CMDBUF_LEN];
				if (cmdp > cmdbuf_p - CMDBUF_LEN && cmd) {
					strcpy(buf,cmd);
					n = strlen(cmd);
					p = n;
					fresh = 1;
				} else {
					cmdp ++;
				}
				break;
			case VT_CURSOR_DOWN:
				if (cmdp < cmdbuf_p) {
					cmdp ++;
					cmd = cmdbuf[cmdp % CMDBUF_LEN];
					if (cmdp != cmdbuf_p) {
						strcpy(buf,cmd);
						n = strlen(cmd);
						p = n;
					} else {
						n = 0; p = 0;
					}
					fresh = 1;
				}
				break;
			case VT_CURSOR_LEFT:
				if (p > 0) {
					p --;
					put_vtctl(con,cch);
				}
				break;
			case VT_CURSOR_RIGHT:
				if (p < n) {
					p ++;
					put_vtctl(con,cch);
				}
				break;
			}
			break;
		case '\t':
			if (p == n) {
				int i = n;
				while (buf[i] != ' ' && i > 0) i--;
				i = complete_command(&buf[i]);
				p += i; n += i;
				fresh = 1;
			}
			break;
		case '\003':
			break;
		default:
			if (ch >= 32) {
				insert_char(buf,n++,ch,p++);
				if (p == n)
					con->putchar(ch);
				else
					fresh = 1;
			} else {
				printk("unknown char: %x %d\n",ch,ch);
			}
			break;
		}
		if (fresh)
			fresh_str(con,buf,n,p);
	}
	buf[0] = 0;
	return 0;
}


static char ** build_arg(char sbuf[],int len,int *argc)
{
	char **argv,*buf;
	int i = 0, n = 0;

	argv = malloc(sizeof(char*)*64 + 512);
	buf = (char *)(argv + 64);
	for (i = 0; i < len; i++)
		buf[i] = sbuf[i];
	i = 0;
	while (i < len && n < 64-1) {
		while(buf[i] == ' ' && i < len) i++;
		argv[n++] = &buf[i];
		while(buf[i] != ' ' && i < len) i++;
		buf[i++] = 0;
	}
	argv[n] = NULL;
	*argc = n;
	return argv;
}
int cui_shell(void)
{
	struct console *con;
	char buf[512], **argv;

	signal_mask(SIGTERM);
	signal_mask(SIGEXIT);
	con = get_defconsole();
	do_command(0, NULL);	/* init */
	while (1) {
		int len, argc;
		len = get_cmdline(con,buf,512);
		argv = build_arg(buf,len,&argc);
		if (do_command(argc,argv))
			break;
	}
	return 0;
}
