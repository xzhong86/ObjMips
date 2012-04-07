
#include <stdio.h>
#include <string.h>

#include <base.h>
#include <pcpu.h>
#include <smp.h>

struct munit {
	struct munit *next; // must be the first
	int size;  // size<0 if free
	char data[0];
};
struct pcpu_st {
	unsigned int base;
	unsigned int malloc_p;
	struct munit *mhead;
};


#define PCPU_ST_SP(sp)							\
	( (struct pcpu_st*)(PCPU_SP_BASE() + sizeof(struct per_cpu_var)) )

static int init_rand_seed(void);
int pcpu_init(void)
{
	struct per_cpu_var *pv;
	pv = PCPU_VAR();
	memset(pv,0,sizeof(*pv));
	
	struct pcpu_st *mm;
	mm = PCPU_ST_SP(&mm);
	mm->base = (unsigned int)mm;
	mm->malloc_p = mm->base + sizeof(struct pcpu_st);
	mm->mhead = NULL;

	init_rand_seed();
	return 0;
}

#define MIN_SPACE 0x1000 // 4k
static void *_pcpu_malloc(int size)
{
	void *p;
	struct pcpu_st *mm;
	mm = PCPU_ST_SP(&mm);
	if((unsigned int)&mm - mm->malloc_p - size <= MIN_SPACE)
		return NULL;
	p = (void*)mm->malloc_p;
	mm->malloc_p += size;
	return p;
}

void *pcpu_malloc(int size)
{
	struct pcpu_st *mm;
	mm = PCPU_ST_SP(&mm);
	struct munit **mpp = &mm->mhead;

	if(size <= 0) return NULL;
	if(size%8)  // 8 16 24 32 ...
		size = (size/8+1)*8;

	for(; *mpp; mpp=&(*mpp)->next)
		if((*mpp)->size<0 && (-(*mpp)->size)>=size)
			break;
	if(*mpp)
		goto find_mu;

	*mpp = (struct munit*)_pcpu_malloc(sizeof(struct munit)+size);
	if(! *mpp)
		return NULL;
	(*mpp)->next = NULL;
	(*mpp)->size = size;
	return (void*)&(*mpp)->data[0];

find_mu:
	if((-(*mpp)->size) - size <= sizeof(struct munit)*2+8) {
		(*mpp)->size = - (*mpp)->size;
		return (void*)&(*mpp)->data[0];
	}
	struct munit *mp = (struct munit*)&(*mpp)->data[size];
	mp->next = (*mpp)->next;
	(*mpp)->next = mp;
	(*mpp)->size = - (*mpp)->size;
	mp->size = - ((*mpp)->size - size - sizeof(struct munit));
	(*mpp)->size = size;
	return (void*)&(*mpp)->data[0];
}


static int attach_free(void)
{
	struct pcpu_st *mm;
	mm = PCPU_ST_SP(&mm);

	struct munit **pp = &mm->mhead;

next_free:
	if(! *pp)
		return 0;

	if((*pp)->next == NULL) {
		if((*pp)->size <0) {
			mm->malloc_p -= sizeof(struct munit) - (*pp)->size;
			//assert(mm->malloc_p == *pp)
			*pp = NULL;
		}
		return 0;
	}
	if((*pp)->size < 0 && (*pp)->next->size <0) {
		(*pp)->size += (*pp)->next->size - sizeof(struct munit);
		(*pp)->next  = (*pp)->next->next;
		goto next_free;
	} else {
		pp = &(*pp)->next;
		goto next_free;
	}
	
	return 0;
}
int pcpu_free(void *p)
{
	struct pcpu_st *mm;
	mm = PCPU_ST_SP(&mm);
	struct munit **mpp = &mm->mhead;

	if(! p) return -1;
	for(; *mpp; mpp=&(*mpp)->next)
		if((void*)(*mpp)->data == p)
			break;
	if(! *mpp)
		return -1;
	if((*mpp)->size < 0)
		return -1;

	(*mpp)->size = - (*mpp)->size;
	attach_free();
	return 0;
}

void pcpu_dump(void)
{
	struct pcpu_st *mm;
	mm = PCPU_ST_SP(&mm);
	struct munit *mp = mm->mhead;
	int n = 0;
	
	printk("[PCPU] dump mm info\n");
	for(; mp; mp=mp->next) {
		printk("    %3d size:%-5d data:%p  ",
		       n,mp->size,&mp->data[0]);
		printk("next:%p\n",mp->next);
		n++;
	}
}

#define RAND_SEED  (PCPU_VAR()->rand_seed)
static int init_rand_seed(void)
{
	return RAND_SEED = 1;
}
int pcpu_rand(void)
{
	RAND_SEED = RAND_SEED * 1103515245 + 12345;
	return ((unsigned int)(RAND_SEED >>16));
}
void pcpu_srand(unsigned int seed)
{
	RAND_SEED = seed;
}
