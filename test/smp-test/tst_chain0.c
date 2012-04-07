#include <test/test-head.h>
#include <linux/spinlock.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define NODE_NUM (1024*16)
struct chain_nd {
	arch_rwlock_t rwlock;
	int val;
	struct chain_nd *next;
};

struct glob_opt {
	arch_rwlock_t rwlock;
	struct chain_nd *ndarr; /* node array pointer */
	struct chain_nd *total_head;
	struct chain_nd *new_order;
	struct chain_nd *head0;
	struct chain_nd *head1;
};
static struct glob_opt gopt;

#ifdef CONFIG_SMP
static int reorder_mp(struct chain_nd** headp)
{
	struct chain_nd *p_in,*head;
	head = *headp;
next:
	p_in = head;
	head = head->next;
	
	struct chain_nd *pre,**pp;
	pre = NULL;
	for(pp=&gopt.new_order;*pp;pp=&(*pp)->next) {
	retry:
		if((*pp)->val >= p_in->val
		   || (*pp)->next == NULL) 
		{
			arch_rwlock_t *wlockp;
			if(pre) wlockp = &pre->rwlock;
			else wlockp = &gopt.rwlock;
			
			if(!arch_write_trylock(wlockp)) {
				pp = &pre->next;
				goto retry;
			}
			arch_read_lock(&(*pp)->rwlock);
			
			p_in->next = (*pp)->next;
			*pp = p_in;
			
			arch_read_unlock(&(*pp)->rwlock);
			arch_write_unlock(wlockp);
			p_in = NULL;
			break;
		}
		pre = *pp;
	}
	if(p_in)
		return -1;
	if(head)
		goto next;

	*headp = NULL;
	return 0;
}
static void slave_func(void)
{
	int msg;
	msg = reorder_mp(&gopt.head1);
	smp_msg_send_other(msg!=0);
}
static void push_slave(void)
{
	smp_fun_push(0,slave_func);
}
static int get_in_order_mp(void)
{
	int ret,msg;
	gopt.head0 = gopt.total_head;
	gopt.ndarr[NODE_NUM/2-1].next = NULL;
	gopt.head1 = &gopt.ndarr[NODE_NUM/2];

	smp_ipi_func(0xf,smp_msg_clear);
	smp_ipi_func(0x2,push_slave);

	ret = reorder_mp(&gopt.head0);
	smp_msg_receive(&msg);
	if(ret || msg)
		return -1;
	return 0;
}
#endif

static int get_in_order_up(void)
{
	struct chain_nd *p_in;
next:
	p_in = gopt.total_head;
	gopt.total_head = gopt.total_head->next;
	
	struct chain_nd **pp;
	for(pp=&gopt.new_order;*pp;pp=&(*pp)->next) {
		if((*pp)->val >= p_in->val) {
			p_in->next = (*pp)->next;
			*pp = p_in;
			break;
		}
	}
	if(! *pp) {
		*pp = p_in;
		p_in = NULL;
	}
	if(gopt.total_head)
		goto next;

	return 0;
}

#define POS(p) ((p) - gopt.ndarr)
static int check_result(unsigned int *hashp)
{
	unsigned int hash=0;
	struct chain_nd *p;
	if((p=gopt.new_order) == NULL)
		return -1;
	for(;p->next; p=p->next) {
		if(p->val > p->next->val)
			return -1;
		hash += POS(p) * 4;
		hash += POS(p->next);
		hash ^= p->val;
	}
	*hashp = hash;
	return 0;
}

int test_chain0(void)
{
	int i,ret;
	unsigned int hash0,hash1,ns;
	unsigned long long c0,c1;

	/* set random seed,use ost counter */
	srand(ost_read_counter());
	//srand(1234);
	memset(&gopt, 0, sizeof(gopt));

	gopt.ndarr = (struct chain_nd*)
		malloc(NODE_NUM*sizeof(struct chain_nd));
	for(i=0;i<NODE_NUM;i++) {
		gopt.ndarr[i].val = rand() % (2*NODE_NUM);
		ARCH_RW_LOCK_INIT(gopt.ndarr[i].rwlock);
		gopt.ndarr[i].next = &gopt.ndarr[i+1];
	}
	gopt.ndarr[i-1].next = NULL;
	gopt.total_head = gopt.ndarr;
	
	c0 = ost_get_cyc();
	get_in_order_up();
	c0 = ost_get_cyc() - c0;
	ret = check_result(&hash0);
	if(ret) return ret;
	ns = ost_cnt_to_nsec((unsigned int)c0);
	printk("chain reorder,UP mode,it takes %d nsecond.\n",ns);

#ifdef CONFIG_SMP
	for(i=0;i<NODE_NUM;i++) 
		gopt.ndarr[i].next = &gopt.ndarr[i+1];
	gopt.total_head = gopt.ndarr;

	c1 = ost_get_cyc();
	get_in_order_mp();
	c1 = ost_get_cyc() - c0;
	ret = check_result(&hash1);
	if(ret) return ret;
	ns = ost_cnt_to_nsec((unsigned int)c1);
	printk("chain reorder,MP mode,it takes %d nsecond.\n",ns);
	if(hash0 != hash1) return -1;
#endif

	free(gopt.ndarr);
	return 0;
}

DEF_TEST(test_chain0, 20, (TESTF_2BIG | TESTF_2CORE));
