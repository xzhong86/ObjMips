
ROOT	?= ..
include $(ROOT)/config.mk

SRCS	= drivers.c serial.c time.c intc.c pll.c pm.c dma.c
SRCS	+= com_serial.c 
SRCS	+= clk_4770.c #clk_4760.c #clk_4770.c 

include $(ROOT)/make-var.mk

all: built.o

include $(ROOT)/make-com.mk
