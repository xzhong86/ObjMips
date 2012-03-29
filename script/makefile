
ifeq ($(ROOT),)
$(error "ROOT not defined or empty.")
endif

include $(ROOT)/config.mk

include Makefile

include $(ROOT)/script/make-var.mk

.PHONY: check
all: subdir built.o check

check:
	@if [ built.o -nt . ] ; then \
		touch . ; \
	fi

include $(ROOT)/script/make-com.mk

include .depend
