
#include <stdio.h>
#include "mipsop.h"
#include "cacheop.h"
#include "mips_inst.h"
#include "irq.h"
#include "smp.h"
#include "smp_msg.h"
#include "smp_fun.h"
#include "pcpu.h"

#ifdef CONFIG_SMP

#define get_smp_ctrl()		__read_32bit_c0_register($12, 2)
#define set_smp_ctrl(val)	__write_32bit_c0_register($12, 2, val)
#define get_smp_status()	__read_32bit_c0_register($12, 3)
#define set_smp_status(val)	__write_32bit_c0_register($12, 3, val)
#define get_smp_reim()		__read_32bit_c0_register($12, 4)
#define set_smp_reim(val)	__write_32bit_c0_register($12, 4, val)
#define get_smp_lock()		__read_32bit_c0_register($12, 5)
#define set_smp_lock(val)	__write_32bit_c0_register($12, 5, val)
#define get_smp_val()		__read_32bit_c0_register($12, 6)
#define set_smp_val(val)	__write_32bit_c0_register($12, 6, val)

#define get_smp_mbox0()		__read_32bit_c0_register($20, 0)
#define set_smp_mbox0(val)	__write_32bit_c0_register($20, 0, val)
#define get_smp_mbox1()		__read_32bit_c0_register($20, 1)
#define set_smp_mbox1(val)	__write_32bit_c0_register($20, 1, val)

#if 0
#define smp_spinlock() do {			\
		int cpu = smp_cpu_id() + 1;	\
		set_smp_val(cpu);		\
		while(get_smp_lock() != cpu)	\
			set_smp_val(cpu);	\
	} while(0)
#define smp_spinunlock() set_smp_lock(0)

#else
#define smp_spinlock() do {				\
	unsigned int cpu,tmp;				\
	__asm__ __volatile__ (				\
	"	mfc0  %0,$15,1  \n"			\
	"	addiu %0,%0,1   \n"			\
	"1:	mtc0  %0,$12,6  \n"			\
	"	nop		\n"			\
	"	mfc0  %1,$12,5  \n"			\
	"	bne   %1,%0,1b  \n"			\
	"	nop		\n"			\
	:"=r"(cpu),"=r"(tmp)				\
		);					\
	} while(0)
#define smp_spinunlock() set_smp_lock(0)

#endif
	
#define smp_ipi_unmask(mask) do {		\
		unsigned int reim;		\
		reim = get_smp_reim();		\
		reim |= (mask) & 0x3;		\
		set_smp_reim(reim);		\
	} while(0)
#define smp_ipi_mask(mask) do {			\
		unsigned int reim;		\
		reim = get_smp_reim();		\
		reim &= ~((mask) & 0x3);	\
		set_smp_reim(reim);		\
	} while(0)


static int smp_flag = 0;
#define SMP_FLAG (*(volatile int*)K0_TO_K1(&smp_flag))

extern void _start_secondary(void);
int setup_smp(void)
{
	unsigned int start = PCPU_BASE(1) | 0x20000000;
	unsigned int sp = PCPU_SP_TOP(1);
	unsigned int a0 = 0;
	unsigned int entry = (unsigned int)_start_secondary;
	unsigned int *p = (unsigned int*)start;
	unsigned int reim = get_smp_reim();
	unsigned int ctrl = get_smp_ctrl();
	
	int i = 0;
	while(ctrl & 0x0302) {
		printf("error! bad ctrl %08x\n",ctrl);
		ctrl = get_smp_ctrl();
		if(i++ > 5) break;
	}
	
	printf("cpu: %d\n",smp_cpu_id());
	printf("smp ctrl  : %08x\n",get_smp_ctrl());
	printf("smp status: %08x\n",get_smp_status());
	printf("smp reim  : %08x\n",get_smp_reim());

	//entry = K0_TO_K1(entry);
	
	*p++ = LUI(29,sp>>16);
	*p++ = ORI(29,sp);
	*p++ = LUI(4,a0>>16);
	*p++ = ORI(4,a0);
	*p++ = LUI(8,entry>>16);
	*p++ = ORI(8,entry);
	*p++ = JR(8);
	*p++ = 0;

	flush_dcache_range((unsigned int)start,(unsigned int)p);

	//start = K0_TO_K1(start);

	reim &= 0xffff;
	reim |= start & 0xffff0000;
	set_smp_reim(reim);
	ctrl |= 0x200;  // set ctrl.RPC1
	set_smp_ctrl(ctrl);

#if 1
	unsigned int *addr = (unsigned int*)start;
	printf("debug for setup secondary cpu.\n");
	printf("%p: %08x\n",&addr[0],addr[0]);
	printf("%p: %08x\n",&addr[1],addr[1]);
	printf("%p: %08x\n",&addr[2],addr[2]);
	printf("%p: %08x\n",&addr[3],addr[3]);
	printf("%p: %08x\n",&addr[4],addr[4]);
	printf("%p: %08x\n",&addr[5],addr[5]);
	printf("%p: %08x\n",&addr[6],addr[6]);
	printf("%p: %08x\n",&addr[7],addr[7]);
	printf("start: %08x, entry: %08x\n",start,entry);
	printf("smp ctrl  : %08x\n",get_smp_ctrl());
	printf("smp status: %08x\n",get_smp_status());
	printf("smp reim  : %08x\n",get_smp_reim());
	printf("smp lock  : %08x\n",get_smp_lock());
	printf("smp val   : %08x\n",get_smp_val());
	printf("smp mbox0 : %08x\n",get_smp_mbox0());
	printf("smp mbox1 : %08x\n",get_smp_mbox1());
	
	if(0 && smp_cpu_id() == 0) {
		reim = get_smp_reim();
		reim = 0x0200;
		set_smp_reim(reim);
		
		ctrl = get_smp_ctrl();
		ctrl = 0x0001;
		set_smp_ctrl(ctrl);
	}
#endif

	ctrl = get_smp_ctrl();
	ctrl &= ~0x2;   // clear ctrl.SW_RST1
	set_smp_ctrl(ctrl);
	printf("smp ctrl  : %08x\n",get_smp_ctrl());

	__asm__ __volatile__ (
		"	.set   noreorder	\n"
		"	addiu  $2,$0,128	\n"
		"1:	subu   $2,$2,1		\n"
		"	bne    $2,$0,1b		\n"		"	nop    			\n"
		"	.set   reorder		\n"
		::);		/* wait 128 cycles at least */
	printf("Waiting for secondary cpu ...\n");
	while(SMP_FLAG == 0) ;
	printf("Secondary cpu working now.\n");
	return 0;
}

static void smp_cpu_idle(void)
{
	while(1) {
		smp_fun_run();
		asm("wait");
	}
}
extern int per_cpu_init_0(void);
void start_secondary(void)
{
	void (*fun)(void);
	per_cpu_init_0();
	fun = smp_cpu_idle;
	if(((unsigned int)fun >> 28) == 0xA)  	/* 0xAxxxxxxx */
		fun -= 0x20000000;
	int cpu = smp_cpu_id();
	printf("slave%d stand up!\n",cpu);
	SMP_FLAG = cpu;
	fun();
}

#define SMP_IPI_IRQ 3
int setup_ipi(void)
{
	register_irqfun(SMP_IPI_IRQ,smp_ipi_interrupt,"IPI");
	smp_ipi_unmask(0x3);
	return 0;
}

int smp_ipi_func(int cpumask,void (*fun)(void))
{
	smp_spinlock();
	smp_ipi_mask(0x3);

	if(cpumask & 1)
		set_smp_mbox0((unsigned int)fun);
	if(cpumask & 2)
		set_smp_mbox1((unsigned int)fun);

	smp_ipi_unmask(0x3);
	smp_spinunlock();
	return 0;
}


#define SMP_IPI_RESCED   1
#define SMP_IPI_CALLFUN  2
#define SMP_IPI_MBOX     3
static struct ipi_fun_t {
	void (*fun)(void *);
	void *data;
} ipi_arg;
int smp_call_func(int cpumask,void (*fun)(void *),void *data)
{
	smp_ipi_mask(0x3);
	smp_spinlock();

	ipi_arg.fun = fun;
	ipi_arg.data = data;
	if(cpumask & 1)
		set_smp_mbox0(SMP_IPI_CALLFUN);
	if(cpumask & 2)
		set_smp_mbox1(SMP_IPI_CALLFUN);

	smp_spinunlock();
	smp_ipi_unmask(0x3);
	return 0;
}
static void int_call_func()
{
	ipi_arg.fun(ipi_arg.data);
}

void mbox_ipi_entry(unsigned int);
void smp_ipi_interrupt(void)
{
	int cpu = smp_cpu_id();
	unsigned int mbox,type;

	smp_spinlock();
	
	if(cpu == 0) 
		mbox = get_smp_mbox0();
	else
		mbox = get_smp_mbox1();

	type = mbox & 0x3;
	if(type==0 && mbox)
		((void (*)(void))mbox)();
	else if(type == SMP_IPI_CALLFUN)
		int_call_func();
	else if(type == SMP_IPI_MBOX)
		mbox_ipi_entry(mbox);
	
	smp_spinunlock();
}

#else /* !CONFIG_SMP */

void start_secondary(void)
{
	while(1) {
		asm("nop");
	}
}

#endif
