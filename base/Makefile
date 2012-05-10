

obj-y	= start.o exp_int.o tlb_miss.o trap.o mmu.o irq.o
obj-y   += base.o param.o cache.o pcpu.o smp.o 
obj-y   += smp_msg.o smp_fun.o 
obj-y   += pmon.o sys.o cmain.o init.o
obj-y	+= mem.o ktime.o

obj-y 	+= switch.o thread.o sem.o

obj-y 	+= console.o

# Basic devices for OBJ-MIPS
obj-y	+= dev-ddrc.o dev-intc.o dev-uart.o dev-ost.o

obj-$(CONFIG_NCSIM) += rom.o
