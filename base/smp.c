
#include <stdio.h>
#include <mipsop.h>
#include <cache.h>
#include <bitops.h>
#include <mips_inst.h>
#include <irq.h>
#include <smp.h>
#include <smp_io.h>
#include <smp_msg.h>
#include <smp_fun.h>
#include <smp_stat.h>
#include <pcpu.h>

#ifdef CONFIG_SMP

#define BOOT_NR_CPU 1

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
#define get_smp_mbox2()		__read_32bit_c0_register($20, 2)
#define set_smp_mbox2(val)	__write_32bit_c0_register($20, 2, val)
#define get_smp_mbox3()		__read_32bit_c0_register($20, 3)
#define set_smp_mbox3(val)	__write_32bit_c0_register($20, 3, val)

#if 1
#include <linux/spinlock.h>
static arch_spinlock_t smplock = { .lock = 0 };

#define smp_spinlock() arch_spin_lock(&smplock)
#define smp_spinunlock() arch_spin_unlock(&smplock)

#else
#define smp_spinlock() do {				\
	unsigned int cpu,tmp;				\
	__asm__ __volatile__ (				\
	"	.set  noreorder \n"			\
	"	mfc0  %0,$15,1  \n"			\
	"	andi  %0,%0,0xf \n"			\
	"	addiu %0,%0,1   \n"			\
	"1:	mtc0  %0,$12,6  \n"			\
	"	nop		\n"			\
	"	mfc0  %1,$12,5  \n"			\
	"	bne   %1,%0,1b  \n"			\
	"	nop		\n"			\
	"	.set  reorder   \n"			\
	:"=r"(cpu),"=r"(tmp)				\
		);					\
	} while(0)
#define smp_spinunlock() set_smp_lock(0)

#endif
	
#define smp_ipi_unmask(mask) do {		\
		unsigned int reim;		\
		reim = get_smp_reim();		\
		reim |= (mask) & 0xff;		\
		set_smp_reim(reim);		\
	} while(0)
#define smp_ipi_mask(mask) do {			\
		unsigned int reim;		\
		reim = get_smp_reim();		\
		reim &= ~((mask) & 0xff);	\
		set_smp_reim(reim);		\
	} while(0)

#define smp_clr_pending(mask) do {			\
		unsigned int stat;			\
		stat = get_smp_status();		\
		stat &= ~((mask) & 0xff);		\
		set_smp_status(stat);			\
	} while(0)

#define DEBUG
#ifdef  DEBUG
static void smp_dump_regs(void);
#else
#define smp_dump_regs()  do { } while (0)
#endif

static int smp_flag = 0;
#define SMP_FLAG (*(volatile int*)K0_TO_K1(&smp_flag))
#define SMP_FLAG_IDLE 9999

static unsigned long cpu_ready,cpu_start,cpu_running,bootsp;
extern void _start_secondary(void);
static int prepare_cpus(int max_cpus)
{
	unsigned int start = PCPU_BASE(1) | 0x20000000;
	unsigned int entry = (unsigned int)_start_secondary;
	unsigned long *p = (unsigned long*)start;
	unsigned int reim = get_smp_reim();
	unsigned int ctrl = get_smp_ctrl();
	
	int up, i = 0;
	unsigned int pflag = (unsigned int)K0_TO_K1(&smp_flag);

	*p++ = INST_LUI(26,pflag>>16);
	*p++ = INST_ORI(26,pflag);
	*p++ = INST_LW(2,0,26);
	*p++ = INST_ADDIU(2,2,1);
	*p++ = INST_SW(2,0,26);

	*p++ = INST_LUI(15,(unsigned long)&cpu_start>>16);
	*p++ = INST_ORI(15,(unsigned long)&cpu_start);
	*p++ = INST_LUI(24,(unsigned long)&cpu_ready>>16);
	*p++ = INST_ORI(24,(unsigned long)&cpu_ready);
	*p++ = INST_LUI(25,(unsigned long)&cpu_running>>16);
	*p++ = INST_ORI(25,(unsigned long)&cpu_running);

	*p++ = INST_LUI(29,(unsigned long)&bootsp>>16);
	*p++ = INST_ORI(29,(unsigned long)&bootsp);
	*p++ = INST_LUI(8,entry>>16);
	*p++ = INST_ORI(8,entry);
	*p++ = INST_JR(8);
	*p++ = 0;

	reim = get_smp_reim();
	reim &= 0xffff;
	reim |= start & 0xffff0000;
	set_smp_reim(reim);
	ctrl = 0xfffe;  // set ctrl.RPC
	//ctrl = 0x8000fffe;  // set ctrl.RPC
	set_smp_ctrl(ctrl);

	set_smp_mbox0(0);
	set_smp_mbox1(0);
	set_smp_mbox2(0);
	set_smp_mbox3(0);
	set_smp_status(0);

	blast_cache_all();

	ctrl = get_smp_ctrl();
	ctrl &= ~((1 << max_cpus) -1);   // clear ctrl.SW_RST1
	set_smp_ctrl(ctrl);
	smp_printf("smp ctrl  : %08x\n",get_smp_ctrl());

	p = K0_TO_K1(&cpu_ready);
	do {
		up = 1;
		for (i = 0; i < max_cpus; i++)
			if (test_bit(i, p))
				up ++;
	} while(up < max_cpus);

	printk("[SMP] %d cpus start up.\n",up);
	return 0;
}
int start_cpu(int cpu)
{
	unsigned long *p;

	p = K0_TO_K1(&cpu_ready);
	if (!test_bit(cpu, p))
		return -1;
	
	bootsp = PCPU_SP_TOP(cpu);
	smp_wmb();
	set_bit(cpu, &cpu_start);
	while (!test_bit(cpu, &cpu_running))
		;
	return 0;
}

int smp_running_cpus;
static int max_cpus = BOOT_NR_CPU;
static void setup_max_cpus(char *arg)
{
	max_cpus = arg[0] - '0';
}
__setup("maxcpus", setup_max_cpus);
int setup_smp(void)
{
	int i;
	
	smp_running_cpus = 1;
	prepare_cpus(max_cpus);	
	for (i = 1; i < max_cpus; i++)
		if (start_cpu(i))
			printk("start cpu %d failed.\n",i);
		else
			smp_running_cpus ++;
	set_bit(0, &cpu_running);
	return 0;
}

static void smp_cpu_idle(void)
{
	SMP_STAT_FUNC();
	SMP_FLAG = SMP_FLAG_IDLE;
	while(1) {
		smp_fun_run();
		irq_disable();
		if (smp_fun_empty())
			cpu_wait();
		irq_enable();
	}
}
extern int per_cpu_init_0(void);
static void secondary_trap(void)
{
	unsigned int cp0_sr;
	cp0_sr = read_c0_status();
	cp0_sr &= ~0x00400000;  // clear BEV
	cp0_sr |=  0x0000ff00;  // set ST_IM2
	write_c0_status(cp0_sr);
}
void start_secondary(void)
{
	void (*fun)(void);
	SMP_STAT_FUNC();
	per_cpu_init_0();
	secondary_trap();
	SMP_FLAG = 5;
	fun = smp_cpu_idle;
	if(((unsigned int)fun >> 28) == 0xA)  	/* 0xAxxxxxxx */
		fun -= 0x20000000/4;
	int cpu = smp_cpu_id();
	SMP_FLAG = cpu * 32;
	//fun();
	irq_enable();
	printk("CPU%d ready! sp=%x\n",cpu,__current_sp);
	smp_cpu_idle();
}

#define SMP_IPI_IRQ 3
int setup_ipi(void)
{
	register_irqfun(SMP_IPI_IRQ,smp_ipi_interrupt,"IPI");
	smp_ipi_unmask(0xf);
	return 0;
}

int smp_ipi_func(int cpumask,void (*fun)(void))
{
	int i, times = 0;

	if(! irq_isenable())
		printk("IRQ disable in smp_ipi_func()\n");

	cpumask &= cpu_running;
	do {
		int cpu = 0;
		irq_disable();
		smp_spinlock();
#define TEST(CPU) if (! get_smp_mbox##CPU())	\
			cpu |= 1 << (CPU)
		TEST(0);
		TEST(1);
		TEST(2);
		TEST(3);
#undef  TEST
		if((cpu & cpumask) != cpumask) {
			smp_spinunlock();
			irq_enable();

			for(i=0;i<30;i++) 
				asm("nop");

			if(times++ > 10000000) {
				smp_dump_regs();
				times = 0;
			}
			continue;
		}

#define SEND(CPU) if (cpumask & (1<<(CPU)))			\
			set_smp_mbox##CPU((unsigned int)fun)
		SEND(0);
		SEND(1);
		SEND(2);
		SEND(3);
#undef  SEND
		smp_spinunlock();
		irq_enable();
		break;
	} while(1);
	return 0;
}
int smp_ipi_func_spec(int cpumask,void (*fun)(void))
{
	cpumask &= cpu_running;
	irq_disable();
	smp_spinlock();
#define SEND(CPU) if (cpumask & (1<<(CPU)))		\
		set_smp_mbox##CPU((unsigned int)fun)
	SEND(0);
	SEND(1);
	SEND(2);
	SEND(3);
#undef  SEND
	smp_spinunlock();
	irq_enable();

#define WAIT(CPU) if (cpumask & (1<<(CPU)))	\
		while(get_smp_mbox##CPU())
	WAIT(0);
	WAIT(1);
	WAIT(2);
	WAIT(3);
#undef  WAIT
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
	ipi_arg.fun = fun;
	ipi_arg.data = data;
	smp_ipi_func(cpumask,(void(*)(void))SMP_IPI_CALLFUN);
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
	unsigned int type,mbox = 0;

	SMP_STAT_FUNC();
	
	smp_spinlock();
	switch (cpu) {
#define CASE(CPU)					\
		case CPU:				\
			mbox = get_smp_mbox##CPU();	\
			set_smp_mbox##CPU(0);		\
			smp_clr_pending(1<<(CPU));	\
			break
		CASE(0);
		CASE(1);
		CASE(2);
		CASE(3);
#undef CASE
	}
	smp_spinunlock();

	type = mbox & 0x3;
	if(type==0 && mbox)
		((void (*)(void))mbox)();
	else if(type == SMP_IPI_CALLFUN)
		int_call_func();
	else if(type == SMP_IPI_MBOX)
		mbox_ipi_entry(mbox);
	
}

#ifdef DEBUG
static void smp_dump_regs(void)
{
	printk("smp ctrl  : %08x\n",get_smp_ctrl());
	printk("smp status: %08x\n",get_smp_status());
	printk("smp reim  : %08x\n",get_smp_reim());
	printk("smp lock  : %08x\n",get_smp_lock());
	printk("smp val   : %08x\n",get_smp_val());
	printk("smp mbox0 : %08x\n",get_smp_mbox0());
	printk("smp mbox1 : %08x\n",get_smp_mbox1());
	printk("smp mbox2 : %08x\n",get_smp_mbox2());
	printk("smp mbox3 : %08x\n",get_smp_mbox3());
}
#endif

#else /* !CONFIG_SMP */

void start_secondary(void)
{
	while(1) {
		asm("nop");
	}
}

#endif
