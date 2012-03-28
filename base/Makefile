

SRCS	= start.S exp_int.S tlb_miss.S trap.c mmu.c irq.c
SRCS   += base.c param.c cache.c pcpu.c smp.c mbox.c
SRCS   += smp_msg.c smp_fun.c smp_stat.c smp_puts.c
SRCS   += pmon.c sys.c cmain.c

ifeq ($(CONFIG_NCSIM),y)
SRCS   += rom.S
endif
