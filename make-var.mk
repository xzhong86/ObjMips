
PREFIX		?= mips-linux-gnu-
GCC      	= $(PREFIX)gcc
AS		= $(PREFIX)as
LD		= $(PREFIX)ld
OBJCOPY 	= $(PREFIX)objcopy
OBJDUMP 	= $(PREFIX)objdump

LD_OPT		?= 
OBJDUMP_OPT	?= -D

CFLAGS		?=
CFLAGS		+= -EL -fno-pic -mno-abicalls -mabi=32 -mips32r2 -fomit-frame-pointer
CFLAGS		+= -O2 -Wall #-Werror
BUILTFLAG	= -m elf32ltsmip

ifeq ($(ROOT),)
err: 
	$(error "ROOT not defined or empty.")
endif
LIBS		?= $(ROOT)/lib/libc.a $(ROOT)/lib/libgcc.a $(ROOT)/lib/gloss.a 
DRIVERS		?= $(ROOT)/drivers/built.o
BASE		?= $(ROOT)/base/built.o
TEST		?= $(ROOT)/test/built.o

CFLAGS		+= -I$(ROOT)/include

ifneq ($(SRCS),)
OBJS	= $(addsuffix .o,$(basename $(SRCS)))
DEPS	= $(OBJS:%.o=.%.d)
endif


v_ld_cmd = $(LD) $(BUILTFLAG) -r -o $@ $(OBJS)
v_cc_cmd = $(GCC) $(ALLFLAGS) -c $< -o $@
v_as_cmd = $(GCC) $(ALLFLAGS) -D__ASSEMBLY__ -c $< -o $@
v_dep_cmd = $(GCC) $(filter -I%,$(ALLFLAGS)) -MM $< | \
	sed 's,\($*\)\.o[:]*,\1.o $@:,g' > $@.tmp; \
	if ! test -e $@ || ! cmp -s $@.tmp $@ ; then mv -f $@.tmp $@; \
	else rm -f $@.tmp;  touch $@; fi;

ifndef V
ld_cmd = @echo "BUILD -o $@"; $(v_ld_cmd)
cc_cmd = @echo "CC -c $<"; $(v_cc_cmd)
as_cmd = @echo "AS -c $<"; $(v_as_cmd)
dep_cmd = @echo "DEPEND $@"; $(v_dep_cmd)
else
ld_cmd = $(v_ld_cmd)
cc_cmd = $(v_cc_cmd)
as_cmd = $(v_as_cmd)
dep_cmd = $(v_dep_cmd)
endif
