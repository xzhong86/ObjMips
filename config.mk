
# config area is used for config.h

#Start configs
# this options value must be 'y' or 'n'

CONFIG_SMP = y
CONFIG_NCSIM = n
CONFIG_SIM = n

CONFIG_CUI = y

JZSOC_4770 = y

#End configs


# this is for make-var.mk

PREFIX	= mips-linux-gnu-
CFLAGS	= -EL -mips32r2
BUILTFLAG = -m elf32ltsmip
