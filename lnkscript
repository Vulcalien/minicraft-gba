/* Linker Script v1.3 by Jeff Frohwein              */
/*  v1.0 - Original release                         */
/*  v1.1 - Added proper .data section support       */
/*  v1.2 - Added support for c++ & iwram overlays   */
/*       - Major contributions by Jason Wilkins.    */
/*  v1.3 - .ewram section now can be used when      */
/*         compiling for MULTIBOOT mode. This fixes */
/*         malloc() in DevKitAdvance which depends  */
/*         on __eheap_start instead of end to define*/
/*         the starting location of heap space.     */
/*         External global variable __gba_iwram_heap*/
/*         support added to allow labels end, _end, */
/*         & __end__ to point to end of iwram or    */
/*         the end of ewram.                        */

/* This file is released into the public domain   */
/* for commercial or non-commercial use with no   */
/* restrictions placed upon it.                   */

/* NOTE!!!: This linker script defines the RAM &  */
/*   ROM start addresses. In order for it to work */
/*   properly, remove -Ttext and -Tbss linker     */
/*   options from your makefile if they are       */
/*   present.                                     */

/* You can use the following to view section      */
/* addresses in your .elf file:                   */
/*   objdump -h file.elf                          */
/* Please note that empty sections may incorrectly*/
/* list the lma address as the vma address for    */
/* some versions of objdump.                      */

OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)
/* SEARCH_DIR(/bin/arm); */

/* By default this linker script will generate code        */
/* for flash carts located at 0x8000000. In order to       */
/* generate code that is compiled at 0x2000000 that        */
/* will run on flash carts or in multiboot mode then       */
/* you need to add the following variable to your main     */
/* project file. It's value is NOT important but           */
/* IT MUST BE A GLOBAL VARIABLE OR IT WILL NOT WORK:       */

/*   #define MULTIBOOT int __gba_multiboot;                */
/* Then use it like this: MULTIBOOT                        */

/* By default this linker script will set the labels       */
/* end, _end, & __end__ at the end of ewram. To force      */
/* them to be set to the end of iwram then you need to     */
/* add the following variable to your main                 */
/* project file. It's value is NOT important but           */
/* IT MUST BE A GLOBAL VARIABLE OR IT WILL NOT WORK:       */

/*   #define IWRAMHEAP int __gba_iwram_heap;               */
/* Then use it like this: IWRAMHEAP                        */

/* The linker script function "var1 += var2;" sometimes    */
/* reports incorrect values in the *.map file but the      */
/* actual value it calculates is usually, if not always,   */
/* correct. If you leave out the ". = ALIGN(4);" at the    */
/* end of each section then the return value of SIZEOF()   */
/* is sometimes incorrect and "var1 += var2;" appears to   */
/* not work as well. "var1 += var2" style functions are    */
/* avoided below as a result.                              */

/* The linker script MEMORY directive is not used here due    */
/* to the fact that __text_start is not always a fixed value. */

__text_start = DEFINED (__gba_multiboot) ? 0x2000000 : 0x8000000;
/* __ewram_start     = 0x2000000; */ /* Removed in v1.3 */
__eheap_end       = 0x2040000;
__iwram_start     = 0x3000000;
__iheap_end       = 0x3008000 - 0x400;
__sp_usr          = 0x3008000 - 0x100;
__sp_irq          = 0x3008000 - 0x60;
__intr_vector_buf = 0x3008000 - 4;
__sp_usr_offset   = __sp_usr - __iwram_start;
__intr_vect_offset = __intr_vector_buf - __sp_usr;

SECTIONS
{
  .text __text_start :   /* ALIGN (4): */
  {
    *(EXCLUDE_FILE (*text.iwram*) .text)
    *(.text.*)
    *(.stub)
    /* .gnu.warning sections are handled specially by elf32.em.  */
    *(.gnu.warning)
    *(.gnu.linkonce.t*)
    *(.glue_7)
    *(.glue_7t)
    . = ALIGN(4);  /* REQUIRED. LD is flaky without it. */
  } = 0xff
/*  laddr = ADDR(.text) + SIZEOF(.text); */
  __text_end = .;

  .rodata :
  {
    *(.rodata)
    *all.rodata*(*)
    *(.roda)
    *(.rodata.*)
    *(.gnu.linkonce.r*)
    SORT(CONSTRUCTORS)
    . = ALIGN(4);   /* REQUIRED. LD is flaky without it. */
  } = 0xff
/*  laddr += SIZEOF(.rodata); */

  .ctors :
  {
    /* gcc uses crtbegin.o to find the start of the constructors, so
       we make sure it is first.  Because this is a wildcard, it
       doesn't matter if the user does not actually link against
       crtbegin.o; the linker won't look for a file to match a
       wildcard.  The wildcard also means that it doesn't matter which
       directory crtbegin.o is in.  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
    . = ALIGN(4);   /* REQUIRED. LD is flaky without it. */
  } = 0
/*  laddr += SIZEOF(.ctors); */
  laddr = ADDR(.text) + SIZEOF(.text) + SIZEOF(.rodata) + SIZEOF(.ctors);

  .dtors :
  {
    KEEP (*crtbegin.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
    . = ALIGN(4);   /* REQUIRED. LD is flaky without it. */
  } = 0
/*  laddr += SIZEOF(.dtors); */
  laddr = ADDR(.text) + SIZEOF(.text) + SIZEOF(.rodata) + SIZEOF(.ctors) + SIZEOF(.dtors);

  .eh_frame :
  {
    KEEP (*(.eh_frame))
    . = ALIGN(4);   /* REQUIRED. LD is flaky without it. */
  } = 0
/*  laddr += SIZEOF(.eh_frame); */

  .gcc_except_table :
  {
    *(.gcc_except_table)
    . = ALIGN(4);   /* REQUIRED. LD is flaky without it. */
  } = 0
/*  laddr += (SIZEOF(.gcc_except_table) + 3) & ~ 3; */
/*  __iwram_lma = laddr; */
  __iwram_lma = (ADDR(.text) + SIZEOF(.text) + SIZEOF(.rodata) + SIZEOF(.ctors) + SIZEOF(.dtors) + SIZEOF(.eh_frame) + SIZEOF(.gcc_except_table) + 3) & ~ 3;

  .iwram __iwram_start : AT (__iwram_lma)
  {
    __iwram_start = ABSOLUTE(.) ;
    *(.iwram)
    *iwram.*(.text)
    . = ALIGN(4);   /* REQUIRED. LD is flaky without it. */
  } = 0xff
/*  laddr += SIZEOF(.iwram); */
/*  __data_lma = laddr; */
  __data_lma = ADDR(.text) + SIZEOF(.text) + SIZEOF(.rodata) + SIZEOF(.ctors) + SIZEOF(.dtors) + SIZEOF(.eh_frame) + SIZEOF(.gcc_except_table) + SIZEOF(.iwram);

  __iwram_end = . ;

  .bss ALIGN(4) :
  {
   __bss_start = ABSOLUTE(.);
   __bss_start__ = ABSOLUTE(.);
   *(.dynbss)
   *(.gnu.linkonce.b*)
   *(COMMON)
   . = ALIGN(4);    /* REQUIRED. LD is flaky without it. */
  }

  __bss_end = . ;
  __bss_end__ = . ;

  .data ALIGN(4) : AT (__data_lma)
  {
    __data_start = ABSOLUTE(.);
    *(.data)
    *(.data.*)
    *(.gnu.linkonce.d*)
    CONSTRUCTORS
    . = ALIGN(4);  /* REQUIRED. LD is flaky without it. */
  } = 0xff
/*  laddr += SIZEOF(.data); */
/*  __iwram_overlay_lma = laddr; */
  __iwram_overlay_lma = ADDR(.text) + SIZEOF(.text) + SIZEOF(.rodata) + SIZEOF(.ctors) + SIZEOF(.dtors) + SIZEOF(.eh_frame) + SIZEOF(.gcc_except_table) + SIZEOF(.iwram) + SIZEOF(.data);

  __data_end  =  .;
  PROVIDE (edata = .);
/*  __data_lma = LOADADDR(.data); */
  __iwram_overlay_start = . ;

  OVERLAY : NOCROSSREFS AT (__iwram_overlay_lma)
  {
     .iwram0 { *(.iwram0) . = ALIGN(4);}
     .iwram1 { *(.iwram1) . = ALIGN(4);}
     .iwram2 { *(.iwram2) . = ALIGN(4);}
     .iwram3 { *(.iwram3) . = ALIGN(4);}
     .iwram4 { *(.iwram4) . = ALIGN(4);}
     .iwram5 { *(.iwram5) . = ALIGN(4);}
     .iwram6 { *(.iwram6) . = ALIGN(4);}
     .iwram7 { *(.iwram7) . = ALIGN(4);}
     .iwram8 { *(.iwram8) . = ALIGN(4);}
     .iwram9 { *(.iwram9) . = ALIGN(4);}
  } = 0xff
/*  laddr += (SIZEOF(.iwram0)+SIZEOF(.iwram1)+SIZEOF(.iwram2)+SIZEOF(.iwram3)+SIZEOF(.iwram4)+SIZEOF(.iwram5)+SIZEOF(.iwram6)+SIZEOF(.iwram7)+SIZEOF(.iwram8)+SIZEOF(.iwram9)); */
/*  __ewram_lma = laddr; */
  __ewram_lma = ADDR(.text) + SIZEOF(.text) + SIZEOF(.rodata) + SIZEOF(.ctors) + SIZEOF(.dtors) + SIZEOF(.eh_frame) + SIZEOF(.gcc_except_table) + SIZEOF(.iwram) + SIZEOF(.data) + SIZEOF(.iwram0)+SIZEOF(.iwram1)+SIZEOF(.iwram2)+SIZEOF(.iwram3)+SIZEOF(.iwram4)+SIZEOF(.iwram5)+SIZEOF(.iwram6)+SIZEOF(.iwram7)+SIZEOF(.iwram8)+SIZEOF(.iwram9);

/*  __iwram_overlay_lma = LOADADDR (.iwram0); */
  __iwram_overlay_end = . ;
/*  _end = . ; */
/*  __end__ = . ; */
/*  PROVIDE (end = .); */
  __iheap_start = . ;

  /* v1.3 */
  __ewram_start = DEFINED (__gba_multiboot) ? __ewram_lma : 0x2000000;

  .ewram __ewram_start : AT (__ewram_lma)
  {
/*    __ewram_start = ABSOLUTE(.); */
    *(.ewram)
    . = ALIGN(4);  /* REQUIRED. LD is flaky without it. */
  } = 0xff
/*  laddr += SIZEOF(.ewram); */
/*  __ewram_overlay_lma = laddr; */
  __ewram_overlay_lma = ADDR(.text) + SIZEOF(.text) + SIZEOF(.rodata) + SIZEOF(.ctors) + SIZEOF(.dtors) + SIZEOF(.eh_frame) + SIZEOF(.gcc_except_table) + SIZEOF(.iwram) + SIZEOF(.data) + SIZEOF(.iwram0)+SIZEOF(.iwram1)+SIZEOF(.iwram2)+SIZEOF(.iwram3)+SIZEOF(.iwram4)+SIZEOF(.iwram5)+SIZEOF(.iwram6)+SIZEOF(.iwram7)+SIZEOF(.iwram8)+SIZEOF(.iwram9) + SIZEOF(.ewram);

/*  __ewram_lma = LOADADDR(.ewram); */
  __ewram_end = . ;
  __ewram_overlay_start = . ;

  OVERLAY ALIGN(4): NOCROSSREFS AT (__ewram_overlay_lma)
  {
     .ewram0 { *(.ewram0) . = ALIGN(4);}
     .ewram1 { *(.ewram1) . = ALIGN(4);}
     .ewram2 { *(.ewram2) . = ALIGN(4);}
     .ewram3 { *(.ewram3) . = ALIGN(4);}
     .ewram4 { *(.ewram4) . = ALIGN(4);}
     .ewram5 { *(.ewram5) . = ALIGN(4);}
     .ewram6 { *(.ewram6) . = ALIGN(4);}
     .ewram7 { *(.ewram7) . = ALIGN(4);}
     .ewram8 { *(.ewram8) . = ALIGN(4);}
     .ewram9 { *(.ewram9) . = ALIGN(4);}
  } = 0xff

  __ewram_overlay_end = . ;

  __eheap_start = . ;

  _end = DEFINED (__gba_iwram_heap) ? __iheap_start : .; /* v1.3 */
  __end__ = _end ; /* v1.3 */
  PROVIDE (end = _end); /* v1.3 */

  /* Stabs debugging sections.  */
  .stab 0 : { *(.stab) }
  .stabstr 0 : { *(.stabstr) }
  .stab.excl 0 : { *(.stab.excl) }
  .stab.exclstr 0 : { *(.stab.exclstr) }
  .stab.index 0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment 0 : { *(.comment) }
  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.  */
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
  .stack 0x80000 : { _stack = .; *(.stack) }
  /* These must appear regardless of  .  */
}
