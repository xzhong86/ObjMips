
ROOT	= $(PWD)

include config.mk

obj-y	= base/
obj-y	+= main.o
obj-y	+= test/ fw-tst/ #device/

# Command User Interface
obj-$(CONFIG_CUI) += cui/

include script/make-var.mk


all: config subdir mini dump 

config: $(ROOT)/include/config.h

$(ROOT)/include/config.h: config.mk
	$(ROOT)/script/gen_config.pl $< > $@

mini: $(OBJS) base/link.lds
	@echo "LINK -o $@"; $(LD) -T base/link.lds -o $@  $(OBJS) $(LIBS)

base/link.lds: base/link.lds.S include/config.h
	$(GCC) -E -Iinclude $< | grep -v "^#" > $@

dump: mini
	@echo "DUMP > $@"; $(OBJDUMP) -d $< >| dump

ncsim: mini
	$(OBJCOPY) --set-section-flags .text_rom=alloc,readonly,code $< mini


.PHONY: all clean image install config ncsim distclean
clean: _clean
	@echo "CLEANED"; \
	rm -f mini ; 

distclean: _distclean
	@echo "DISTCLEANED"; \
	rm -f mini dump uImage; 

include script/make-com.mk

ifeq (,$(filter clean distclean,$(MAKECMDGOALS)))
include .depend
endif
