
ALLFLAGS = $(CFLAGS) $(EXT_FLAGS)
SUBPATH = $(subst $(ROOT)/,,$(shell pwd)/)

.PHONY: subdir

built.o: $(OBJS) Makefile
	$(ld_cmd)

subdir:
	@for sub in $(DIRS) ; do \
		echo "MAKE -C $(SUBPATH)$$sub"; \
		$(v_mk_cmd) ./$$sub all || exit 1; \
	done;

%.o: %.c
	$(cc_cmd)
%.o: %.S
	$(as_cmd)

.%.d: %.c
	$(dep_cmd)
.%.d: %.S
	$(dep_cmd)

ifneq ($(DEPS),)
.depend: $(DEPS)
	@echo "GEN .depend " ; cat $^ > $@
endif

.PHONY: _clean _distclean
_clean:
	@rm -f *.o; \
	for sub in $(DIRS) ; do \
		$(v_mk_cmd) ./$$sub _clean; \
	done

_distclean:
	@rm -f *.o .depend .*.d; \
	for sub in $(DIRS) ; do \
		$(v_mk_cmd) ./$$sub _distclean; \
	done

include .depend
