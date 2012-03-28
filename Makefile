
ROOT	= $(PWD)

include config.mk

SRCS	= main.c
DIRS	= base/ drivers/ test/

ifeq ($(CONFIG_CUI),y)
# Command User Interface
DIRS	+= cui/
endif

include script/make-var.mk


all: config subdir mini dump

config: $(ROOT)/include/config.h

$(ROOT)/include/config.h: config.mk
	$(ROOT)/script/gen_config.pl $< > $@

mini: $(OBJS)
	@echo "LINK -o $@"; $(LD) -T lib/link.lds -o $@  $^ $(LIBS)

dump: mini
	$(OBJDUMP) -D $< >| dump

ncsim: mini
	$(OBJCOPY) --set-section-flags .text_rom=alloc,readonly,code $< mini


.PHONY: all clean image install config ncsim distclean
clean: 
	@echo "CLEAN"; \
	rm -r mini ; \
	$(MAKE) _clean
distclean: 
	@echo "DISTCLEAN"; \
	rm -f mini dump uImage; \
	$(MAKE) _distclean

include script/make-com.mk
