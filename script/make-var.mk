
PREFIX		?= mips-linux-gnu-
GCC      	= $(PREFIX)gcc
AS		= $(PREFIX)as
LD		= $(PREFIX)ld
OBJCOPY 	= $(PREFIX)objcopy
OBJDUMP 	= $(PREFIX)objdump

LD_OPT		?= 
OBJDUMP_OPT	?= -D

CFLAGS		?= -EL -mips32r2
CFLAGS		+= -fno-pic -mno-abicalls -mabi=32 -fomit-frame-pointer
CFLAGS		+= -O2 -Wall #-Werror
BUILTFLAG	?= -m elf32ltsmip

ifeq ($(ROOT),)
$(error "ROOT not defined or empty.")
endif
LIBS		?= $(ROOT)/lib/libc.a $(ROOT)/lib/libgcc.a $(ROOT)/lib/gloss.a 
DRIVERS		?= $(ROOT)/drivers/built.o
BASE		?= $(ROOT)/base/built.o

CFLAGS		+= -I$(ROOT)/include

UNKNOWN	= $(filter-out %.o %/,$(obj-y))
ifneq ($(UNKNOWN),)
$(error "unknown obj: $(UNKNOWN)")
endif

DEPS	= $(patsubst %.o,.%.d,$(filter %.o,$(obj-y)))
OBJS	= $(patsubst %/,%/built.o,$(obj-y))
DIRS	= $(patsubst %/,%,$(filter %/, $(obj-y)))

MAKE	= make --no-print-directory
MAKE	+= ROOT=$(ROOT) --makefile $(ROOT)/script/makefile

v_mk_cmd = $(MAKE) -C $@
v_ld_cmd = $(LD) $(BUILTFLAG) -r -o $@ $(OBJS)
v_cc_cmd = $(GCC) $(ALLFLAGS) -c $< -o $@
v_as_cmd = $(GCC) $(ALLFLAGS) -D__ASSEMBLY__ -c $< -o $@
v_dep_cmd = $(GCC) $(filter -I%,$(ALLFLAGS)) -MM $< | \
	sed 's,\($*\)\.o[:]*,\1.o $@:,g' >| $@;

ifndef V
mk_cmd = @echo "MAKE -C $(SUBPATH)$@"; $(v_mk_cmd)
ld_cmd = @echo "BUILD -o $(SUBPATH)$@"; $(v_ld_cmd)
cc_cmd = @echo "CC -c $(SUBPATH)$<"; $(v_cc_cmd)
as_cmd = @echo "AS -c $(SUBPATH)$<"; $(v_as_cmd)
dep_cmd = @echo "DEPEND $(SUBPATH)$@"; $(v_dep_cmd)
else
mk_cmd = $(v_mk_cmd)
ld_cmd = $(v_ld_cmd)
cc_cmd = $(v_cc_cmd)
as_cmd = $(v_as_cmd)
dep_cmd = $(v_dep_cmd)
endif
