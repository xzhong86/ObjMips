/* The following TEXT start address leaves space for the monitor
   workspace.  This linker script links isa32 programs for use with the
   simulator.  */
#include <config.h>

#undef mips
#define mips mips
OUTPUT_ARCH(mips)
ENTRY(_start_entry)
OUTPUT_ARCH("mips:isa32")
OUTPUT_FORMAT("elf32-tradlittlemips")
/* GROUP(-lc -lidt -lgcc) */
SEARCH_DIR(.)
__DYNAMIC  =  0;

SECTIONS
{
#ifdef CONFIG_NCSIM
  . = 0xbfc00000;
  .text_rom : {
     *(.text_rom)
  }
#endif
  . = 0x80020000;
  .text : {
     _ftext = . ;
    PROVIDE (eprol  =  .);
    *(.text)
    *(.text.*)
    *(.gnu.linkonce.t*)
    *(.mips16.fn.*)
    *(.mips16.call.*)
  }
  .init : {
    *(.init)
  }
  .fini : {
    *(.fini)
  }
  .rel.sdata : {
    PROVIDE (__runtime_reloc_start = .);
    *(.rel.sdata)
    PROVIDE (__runtime_reloc_stop = .);
  }
  PROVIDE (etext  =  .);
  _etext  =  .;


  .ctors    :
  {
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */

    KEEP (*crtbegin.o(.ctors))

    /* We don't want to include the .ctor section from
       from the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */

    KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
  }

  .dtors    :
  {
    KEEP (*crtbegin.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
  }

  . = .;
  .rodata : {
    *(.rdata)
    *(.rodata)
    *(.rodata.*)
    *(.gnu.linkonce.r*)
  }
   _fdata = ALIGN(16);
  .data : {
    *(.data)
    __setup_entity_start = .;
    *(.data.setup_entity)
    __setup_entity_stop = .;
    __initcall_entity_start = .;
    *(.data.initcall_entity)
    __initcall_entity_stop = .;
    __command_entity_start = .;
    *(.data.command_entity)
    __command_entity_stop = .;
    __test_entity_start = .;
    *(.data.test_entity)
    __test_entity_stop = .;
    *(.data.*)
    *(.gnu.linkonce.d*)
  }
  . = ALIGN(8);
  _gp = . + 0x8000;
  __global = _gp;
  .lit8 : {
    *(.lit8)
  }
  .lit4 : {
    *(.lit4)
  }
  .sdata : {
    *(.sdata)
    *(.sdata.*)
    *(.gnu.linkonce.s*)
  }
  . = ALIGN(4);
  PROVIDE (edata  =  .);
  _edata  =  .;
  _fbss = .;
  .sbss : {
    *(.sbss)
    *(.scommon)
  }
  .bss : {
    _bss_start = . ;
    *(.bss)
    *(COMMON)
  }

   PROVIDE (end = .);
   _end = .;

  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to
     the beginning of the section so we begin them at 0.  */

  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }

  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }

  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }

  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }

  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }
}
