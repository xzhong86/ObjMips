
ifeq ($(ROOT),)
$(error "ROOT not defined or empty.")
endif

include $(ROOT)/config.mk

ifeq ($(wildcard Makefile),)
$(error "Makefile not found.")
else
include Makefile
endif

include $(ROOT)/script/make-var.mk

.PHONY: check
all: subdir built.o check

check:
	@if [ built.o -nt . ] ; then \
		touch . ; \
	fi

include $(ROOT)/script/make-com.mk

ifneq (,$(filter-out _clean _distclean,$(MAKECMDGOALS)))
include .depend
endif
