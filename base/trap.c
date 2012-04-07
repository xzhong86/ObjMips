#include <stdio.h>
#include <cache.h>
#include <mipsop.h>
#include <mips_inst.h>
#include <irq.h>
#include <smp.h>
#include <base.h>

static void show_regs(struct cpu_regs *reg);
static void show_code(struct cpu_regs *reg);
static void show_stack(struct cpu_regs *reg);

static void seg_fault(int read, struct cpu_regs *reg, char *msg)
{
	printk("%s, %s at 0x%lx, epc: %08lx\n", msg
	       ,read?"read":"write", reg->cp0_badvaddr, reg->cp0_epc);
	show_regs(reg);
	show_code(reg);
	show_stack(reg);
	while(1) {
		cpu_wait();
	}
}
int mm_fault(unsigned long addr, int read);
void trap_entry(unsigned long *regs, unsigned int status)
{
	struct cpu_regs reg;
	unsigned int code;
	unsigned int epc,inst,x;

	reg.saved_regs = regs;
	reg.cp0_status = status;
	reg.cp0_cause  = read_c0_cause();
	reg.cp0_epc    = read_c0_epc();
	reg.cp0_badvaddr = read_c0_badvaddr();

	code = (reg.cp0_cause>>2) & 0x1f;

	if(code == 0) {
		irq_entry(&reg);
		goto trap_end;
	}

	epc = reg.cp0_epc;
	inst = 0;
	if ((epc >> 28) == 8)
		inst = *(unsigned int*)(epc);

	switch(code) {
	case 2:  /* Load at bad address */
		if (mm_fault(reg.cp0_badvaddr, 1))
			seg_fault(1,&reg,"Segmentation fault");
		break;
	case 1:  /* Write at read only page */
	case 3:  /* Store at bad address */
		if (mm_fault(reg.cp0_badvaddr, 0))
			seg_fault(0,&reg,"Segmentation fault");
		break;
	case 4:  /* Adress Error by load */
		seg_fault(1,&reg,"AdressError");
		break;
	case 5:  /* Adress Error by store */
		seg_fault(0,&reg,"AdressError");
		break;
	case 6:  /* Bus Error, fetch instruction */
		seg_fault(1,&reg,"BusError I");
		break;
	case 7:  /* Bus Error, fetch data */
		seg_fault(1,&reg,"BusError D");
		break;
	case 8:  /* syscall */
		x = inst >> 6;
		printk("syscall %d @ %08x,ignore it.\n",x,epc);
		epc += 4;
		break;
	case 9:  /* break */
		x = inst >> 16;
		printk("break %d @ %08x,ignore it.\n",x,epc);
		epc += 4;
		break;
	case 10:  /* Reserved Instruction */
		printk("Reserved Inst. %08x @ %08x,ignore it.\n",inst,epc);
		epc += 4;
		break;
	case 11:  /* CpU */
		if(1 || (inst>>26) == 17) { /* float instruction */
			reg.cp0_status |= 0x20000000;  // set SR_CU1
			//reg.cp0_status |= 0x04000000;  // set SR_FR
			//reg.cp0_status &= ~0x04000000; // clear SR_FR
			write_c0_status(reg.cp0_status);
			printk("float enable.\n");
		} else {
			printk("CpU exception, inst: %08x @ %08x,ignore it.\n"
			       ,inst,epc);
			epc += 4;
		}
		break;
	case 13:  /* trap */
		printk("trap inst: %08x @ %08x,ignore it.\n",inst,epc);
		epc += 4;
		break;
	default:
		printk("exception %d @ %08x,try ignore it.\n",code,epc);
		epc += 4;
		break;
	}
	reg.cp0_epc = epc;

trap_end:
	write_c0_status(reg.cp0_status);
	write_c0_epc(reg.cp0_epc);
	return;
}

extern void _exp_int(void);
extern void _tlb_miss(void);
int setup_trap(void)
{
	unsigned int cp0_sr,ebase;
	unsigned int *base = (unsigned int*)0x80000000;
	unsigned int *p = base;
	unsigned int entry;

	for(;p<base+0x200/4;p++) *p = 0;

	entry = (unsigned int)_tlb_miss;
	p = base;
	*p++ = INST_LUI(26,entry>>16);
	*p++ = INST_ORI(26,entry);
	*p++ = INST_JR(26);
	*p++ = 0;

	entry = (unsigned int)_exp_int;
	p = base + 0x180/4;
	*p++ = INST_LUI(26,entry>>16);
	*p++ = INST_ORI(26,entry);
	*p++ = INST_JR(26);
	*p++ = 0;

	flush_dcache_range((unsigned int)base,(unsigned int)p);
	flush_icache_range((unsigned int)base,(unsigned int)p);
#define EBASE_MASK (0x3ffff000)
	ebase = read_c0_ebase() & ~EBASE_MASK;
	ebase |= (unsigned int)base & EBASE_MASK;
	write_c0_ebase(ebase);
	
	cp0_sr = read_c0_status();
	cp0_sr &= ~0x00400000;  // clear BEV
	cp0_sr |=  0x0000ff00;  // set ST_IM2
	write_c0_status(cp0_sr);

	setup_irq();
#if 0
	unsigned int *addr_vec3 = base + 0x180/4;
	addr_vec3 += 0x20000000 / 4;
	printk("status: %08x, ebase: %08x\n",read_c0_status(),read_c0_ebase());
	printk("%08x: %08x\n",&addr_vec3[0],addr_vec3[0]);
	printk("%08x: %08x\n",&addr_vec3[1],addr_vec3[1]);
	printk("%08x: %08x\n",&addr_vec3[2],addr_vec3[2]);
	printk("%08x: %08x\n",&addr_vec3[3],addr_vec3[3]);
	if(read_c0_status() & 0x00400000)
		entry = 0xbfc00200;
	else
		entry = read_c0_ebase() & ~0xfff;
	printk("exception entry: %08x\n",entry);
#endif

	irq_enable();
	return 0;
}

static void show_regs(struct cpu_regs *reg)
{
	int cpu = smp_cpu_id();
	int i;
	
	reg->saved_regs[0] = 0;
	printk("CPU%d registers:\n",cpu);
	for (i = 0; i < 32; i++) {
		printk("  $%2d:%08lx", i, reg->saved_regs[i]);
		if (i % 4 == 3)
			printk("\n");
	}
	printk("\n");
	printk("epc     : %08lx\n", reg->cp0_epc);
	printk("badvaddr: %08lx\n", reg->cp0_badvaddr);
	printk("Status  : %08lx\n", reg->cp0_status);
	printk("Cause   : %08lx\n", reg->cp0_cause);
}
static void show_code(struct cpu_regs *reg)
{
	unsigned int *code,i;

	code = (unsigned int*)reg->cp0_epc;
	printk("\nCode around:\n");
	for(i = 4; i > 0; i--)
		printk("%08x ",*(code-i));
	printk("<%08x> ",*code);
	for(i = 1; i < 5; i++)
		printk("%08x ",*(code+i));
	printk("\n");
}
static void show_stack(struct cpu_regs *reg)
{
	unsigned int *stack;
	int i;

	if ((reg->saved_regs[29] >> 28) != 8) {
		printk("Bad stack pointer 0x%lx\n",reg->saved_regs[29]);
		return;
	}

	stack = (unsigned int*)reg->saved_regs[29];
	printk("\nStack data:\n");
	for (i = 0; i < 64; i++) {
		if (i % 8 == 0)
			printk("%p:",stack+i);
		printk(" %08x", stack[i]);
		if (i % 8 == 7)
			printk("\n");
	}
}
