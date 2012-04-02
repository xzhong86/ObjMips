
ALLFLAGS = $(CFLAGS) $(EXT_FLAGS)
SUBPATH = $(subst $(ROOT)/,,$(shell pwd)/)

.PHONY: subdir

subdir: $(DIRS)

built.o: $(OBJS) Makefile
	$(ld_cmd)

%.o: %.c
	$(cc_cmd)
%.o: %.S
	$(as_cmd)

.%.d: %.c
	$(dep_cmd)
.%.d: %.S
	$(dep_cmd)

ifneq ($(DEPS) $(DIRS),)
MK_CMD = $(mk_cmd)
.depend: $(DEPS) Makefile
	@echo "GEN $(SUBPATH).depend " ; \
	rm -f $@ ; \
	if [ -n "$(DIRS)" ] ; then \
		echo ".PHONY: $(DIRS)" > $@ ; \
		echo "$(DIRS): " >> $@ ; \
		echo "	\$(value MK_CMD)" >> $@ ; \
	fi; \
	if [ -n "$(DEPS)" ] ; then \
		cat $(DEPS) >> $@ ; \
	fi ;
endif

.PHONY: _clean _distclean
_clean:
	@rm -f *.o; \
	for sub in $(DIRS) ; do \
		$(MAKE) -C ./$$sub _clean; \
	done

_distclean:
	@rm -f *.o .depend .*.d; \
	for sub in $(DIRS) ; do \
		$(MAKE) -C ./$$sub _distclean; \
	done

