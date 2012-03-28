
ifeq ($(ROOT),)
$(error "ROOT not defined or empty.")
endif

include $(ROOT)/config.mk

include Makefile

include $(ROOT)/script/make-var.mk

all: subdir built.o

include $(ROOT)/script/make-com.mk
