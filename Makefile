
ROOT		= $(PWD)

include config.mk
include make-var.mk

SUBDIR		= base drivers test

ifeq ($(CONFIG_CUI),y)
# Command User Interface
SUBDIR		+= cui
endif

BUILT = $(addprefix $(ROOT)/,$(addsuffix /built.o,$(SUBDIR)))

MAKE		+= --no-print-directory
MAKE_PASS	= PREFIX=$(PREFIX) ROOT=$(ROOT) CFLAGS="$(CFLAGS)"


all: config subdir mini dump

config: $(ROOT)/include/config.h

$(ROOT)/include/config.h: config.mk
	$(ROOT)/script/gen_config.pl $< > $@

subdir:
	@for sub in $(SUBDIR) ; do 	\
		echo "Make -C $$sub";	\
		$(MAKE) $(MAKE_PASS) -C ./$$sub || exit 1; \
	done

mini: $(BUILT) main.o
	@echo "LINK -o $@"; $(LD) -T lib/link.lds -o $@  $^ $(LIBS)

ALLFLAGS = $(CFLAGS) $(EXT_FLAGS)
main.o: main.c $(ROOT)/include/config.h
	$(cc_cmd)

dump: mini
	$(OBJDUMP) -D $< >| dump

ncsim: mini
	$(OBJCOPY) --set-section-flags .text_rom=alloc,readonly,code $< mini

.PHONY: all clean image install config subdir ncsim distclean

clean:
	@echo "CLEAN"; \
	rm -rf *.o mini ; \
	for sub in $(SUBDIR) ; do \
		$(MAKE) -C ./$$sub clean; \
	done
distclean: 
	@echo "DISTCLEAN"; \
	rm -rf mini dump uImage; \
	find -name "*.o" | xargs rm -f ; \
	find -name ".*.d" | xargs rm -f ; \
	find -name "depend" | xargs rm -f ;
