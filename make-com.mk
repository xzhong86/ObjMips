
ALLFLAGS = $(CFLAGS) $(EXT_FLAGS)

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

depend: $(DEPS)
	@echo "GEN depend" ; cat $^ > $@

.PHONY: clean distclean
clean:
	rm -f *.o 
distclean: clean
	rm -f depend .*.d

include depend
