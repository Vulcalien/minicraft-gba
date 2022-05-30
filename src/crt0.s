@********************************************************************
@*   crt0.S v1.28 by Jeff Frohwein                                  *
@********************************************************************

@ v1.0 - Original release
@ v1.1 - Added proper .data section support
@ v1.2 - Added support for c++, overlays, interrupts, and
@        far calls (__FarFunction & __FarProcedure).
@      - Some ideas from Jason Wilkins & Mike Heckenbach.
@ v1.21- Killed the dumb test bug left in the code.
@ v1.22- Killed dumb bug "numero dos" in multiple interrupts routine. Thanks Mike H. :)
@ v1.23- Now correctly handles zero length .bss section.
@ v1.24- Loop back to start_vector now works if main {} exits.
@ v1.25- __FarProcedure now works. It was missing a .thumb_func directive.
@ v1.26- Added missing Serial Interrupt processing to __MultipleInterrupts section.
@        Added __FastInterrupt option for minimal interrupt processing.
@        Optimized __MultipleInterrupts section to save 4 bytes of stack space.
@        Added __ISRinIWRAM option that puts interrupt processing in IWRAM by default.
@        Options passed to main() or AgbMain() are now set to 0. (Thanks to DarkFader)
@ v1.27- Even though it might not cause any problems for anyone "as is",
@        changed .SECTION .iwram to .SECTION .iwram,"ax",%progbits
@        just to be safe. That is the more correct description/definition.
@        Added warning below about small default interrupt stack.
@ v1.28- Added force alignment (align 4) to CopyMem & ClearMem to
@        prevent infinite loops in cases where LD (buggy?) fails
@        to align(4). (Thanks to Mark Price & others.)
@
@ This file is released into the public domain for commercial
@ or non-commercial usage with no restrictions placed upon it.

    .TEXT

@ Comment out the next line ONLY if you plan to never support
@ multiboot mode and want to save a few bytes by removing
@ multiboot support code. Otherwise, leave it alone. It wont
@ disturb code designed to run only on flash carts.
@
@ The normal way to enable generating code that works with
@ both multiboot and flash carts is to add the following to
@ your C code in your main project file AS A GLOBAL VARIABLE:
@
@   #define MULTIBOOT int __gba_multiboot;
@ Then use it like this : MULTIBOOT
@
@ IT MUST BE A GLOBAL VARIABLE OR IT WILL NOT WORK!
@ If this variable is not defined somewhere in your project
@ then code will be generated to run out of ROM instead of
@ EXRAM. The value of this variable is not important.

 .equ __MultiBootInclude, 1

@ If you are compiling for multiboot dedicated (will not
@ run in a cart) code then uncomment the following. Normally
@ you should leave this commented out so that a multiboot
@ image will run in a cart as well (by copying from ROM to RAM).
@
@ This sets the maker code to "MB  " which is a key that
@ some emulators look for to know to load the rom image
@ at 0x2000000 instead of the standard 0x8000000.

@ .equ __MultibootDedicated, 1

@ There are two methods for clearing memory and
@ copying appropriate setup data. The fast & bulky
@ method is GBA DMA copy/clear but some emulators
@ do not accurately do DMA. If you have an inaccurate
@ emulator or want to conserve ROM space then comment
@ out the following line. There is not much advantage
@ gained by doing DMA copy/clear.

@ .equ __DMACopyClear, 1

@ Uncomment the following line to support C++ development.
@ You also need to name your main C function the following:
@    int main (void)  ...instead of... int AgbMain (void)
@ Doing so will cause ~5500 bytes of c++ support code to be
@ linked in with your project so do not enable c++ support
@ unless you plan to use it.

.equ __CPPSupport, 1

@ Comment out the following line to disable interrupt support
@ in your code and to save some space in this file.

@ .equ __InterruptSupport, 1


@ Comment out the following line to put interrupt support in
@ ROM instead of IWRAM. Interrupt support in ROM will slow
@ down interrupt execution and has no advantage other than
@ saving a little bit of IWRAM.

 .equ __ISRinIWRAM, 1

@ NOTE: Only ONE of the following 3 interrupt options may be
@ uncommented. Also, __InterruptSupport above must be uncommented
@ for any of the following to have an effect.
@  If __InterruptSupport is uncommented then you MUST select ONE
@ AND ONLY ONE of the following 3 interrupt options.
@
@ __FastInterrupts
@   Uncomment this line for minimal interrupt processing.
@   This allows 160 bytes of interrupt stack space with the
@   default lnkscript.
@
@ __SingleInterrupts
@   Uncomment this line if you wish to use a table of function
@   pointers to process specific interrupts. This allows 160
@   bytes of interrupt stack space with the default lnkscript.
@
@ __MultipleInterrupts
@   Uncomment this line to allow multiple-interrupts-at-once
@   support. If you have several interrupts where one can
@   occur while another is being serviced then you need to
@   enable this option. This option uses the main stack instead
@   of the interrupt stack so you have access to a larger stack.

.equ __FastInterrupts, 1
@ .equ __SingleInterrupts, 1
@ .equ __MultipleInterrupts, 1


@ Uncomment the following line to disable sound and enter an
@ infinite loop if cart is removed during game play. You
@ must have the cart interrupt enabled for this to work and
@ __ISRinIWRAM, above, must be enabled (not commented out.)

@ .equ __HandleCartInterrupt, 1

@ The following prevents IRQ stack overflow by switching to
@ System mode (User stack) when handling multiple interrupts.
@ To force use of IRQ stack only, comment out the following line.

 .equ __SwitchToUserStack, 1

@ !!!! NOTE: THE COPY ROUTINES IN THIS FILE WORK ON 4 BYTE
@ BOUNDARIES. YOUR LINKER SCRIPT MUST ALIGN SECTION STARTS
@ AND SECTION ENDS FOR SECTIONS THAT GET COPIED TO RAM WITH
@ ALIGN(4) !!!!

    .GLOBAL     _start
_start:
        .ALIGN
        .CODE 32
    @ Start Vector

        b       rom_header_end

    @ Nintendo Logo Character Data (8000004h)
@        .fill   156,1,0
	.long 0x51aeff24,0x21a29a69,0x0a82843d
	.long 0xad09e484,0x988b2411,0x217f81c0,0x19be52a3
	.long 0x20ce0993,0x4a4a4610,0xec3127f8,0x33e8c758
	.long 0xbfcee382,0x94dff485,0xc1094bce,0xc08a5694
	.long 0xfca77213,0x734d849f,0x619acaa3,0x27a39758
	.long 0x769803fc,0x61c71d23,0x56ae0403,0x008438bf
	.long 0xfd0ea740,0x03fe52ff,0xf130956f,0x85c0fb97
	.long 0x2580d660,0x03be63a9,0xe2384e01,0xff34a2f9
	.long 0x44033ebb,0xcb900078,0x943a1188,0x637cc065
	.long 0xaf3cf087,0x8be425d6,0x72ac0a38,0x07f8d421

    @ Game Title (80000A0h)
        .byte   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        .byte   0x00,0x00,0x00,0x00

 .ifdef __MultibootDedicated
    @ Game Code (80000ACh)
        .ascii  "MB  "
 .else
    @ Game Code (80000ACh)
        .byte   0x00,0x00,0x00,0x00
 .endif

    @ Maker Code (80000B0h)
        .byte   0x30,0x31

    @ Fixed Value (80000B2h)
        .byte   0x96

    @ Main Unit Code (80000B3h)
        .byte   0x00

    @ Device Type (80000B4h)
        .byte   0x00

    @ Unused Data (7Byte) (80000B5h)
        .byte   0x00,0x00,0x00,0x00,0x00,0x00,0x00

    @ Software Version No (80000BCh)
        .byte   0x00

    @ Complement Check (80000BDh)
        .byte   0xf0

    @ Checksum (80000BEh)
        .byte   0x00,0x00

    .ALIGN
    .ARM                                @ ..or you can use CODE 32 here

rom_header_end:
        b       start_vector        @ This branch must be here for proper
                                    @ positioning of the following header.
                                    @ DO NOT REMOVE IT.

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ The following reserved bytes are used if the code is compiled for      @
@ multiboot mode. It does not hurt anything to leave this header in
@ even if the code is not compiled for multiboot. The GBA BIOS will
@ auto-patch the first two bytes with 0x03 and 0x01, respectively,
@ before running any code if it is executed as multiboot.
@

@ The following two bytes are included even for non-multiboot supporting
@ builds to guarantee that any generic library code that depends on them
@ will still be functional.

    .GLOBAL     __boot_method, __slave_number

__boot_method:
        .byte   0       @ boot method (0=ROM boot, 3=Multiplay boot)
__slave_number:
        .byte   0       @ slave # (1=slave#1, 2=slave#2, 3=slave#3)

 .ifdef __MultiBootInclude

        .byte   0       @ reserved
        .byte   0       @ reserved
        .word   0       @ reserved
        .word   0       @ reserved
        .word   0       @ reserved
        .word   0       @ reserved
        .word   0       @ reserved
        .word   0       @ reserved
 .endif
@                                                                        @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


@@@@@@@@@@@@@@@@@@@@@@
@        Reset       @
@@@@@@@@@@@@@@@@@@@@@@

    .GLOBAL     start_vector
    .ALIGN
    .ARM                                @ ..or you can use CODE 32 here
start_vector:
        mov     r0, #0x12               @ Switch to IRQ Mode
        msr     cpsr, r0
        ldr     sp,=__sp_irq            @ Set SP_irq
        mov     r0, #0x1f               @ Switch to System Mode
        msr     cpsr, r0
        ldr     sp,=__sp_usr            @ Set SP_usr

@ Enter Thumb mode
        adr    r0,1f + 1                @ add r0,pc,#1 also works here
                                        @  for those that want to conserve labels.
        bx     r0

        .THUMB                          @ ..or you can use .CODE 16 here
1:

 .ifdef __InterruptSupport
        ldr     r1, =__intr_vector_buf  @ Set Interrupt Address
        ldr     r0, =intr_main
        str     r0, [r1]
 .endif

 .ifdef __MultiBootInclude

@ *** Multiboot Copy Routine ***
@   Check the Program Counter to see if code is running
@ at 0x2000000 or 0x8000000. If it is running at 0x8000000
@ then copy 256K bytes of it to 0x2000000 and then branch
@ to 0x2000000.
@   The reason for all this is to allow a program to be used
@ "as is" with an flash cart/emulator or with an MBV2-style
@ multiboot cable.
@   NOTE: You can also detect if this ROM is running from
@ 0x2000000 by checking the multiboot header above.

        ldr     r0,=__text_start
        lsl     r0,#5           @ Was code compiled at 0x08000000 or higher?
        bcs     DoEWRAMClear    @ yes, you can not run it in external WRAM

@ Make sure we are in ExWRAM

        mov     r0,pc
        lsl     r0,#5           @ Are we running from ROM (0x8000000 or higher) ?
        bcc     SkipEWRAMClear  @ No, so no need to do a copy.

@ We were started in ROM, silly emulators. :P
@ So we need to copy to ExWRAM.

        mov     r3,#0x40
        lsl     r3,#12          @ r3 = 0x40000
        lsl     r2,r3,#7        @ r2 = 0x2000000
        mov     r6,r2           @ r6 = 0x2000000
        lsl     r1,r2,#2        @ r1 = 0x8000000

        bl      CopyMem

@ Jump to the code to execute

        bx      r6
 .endif

DoEWRAMClear:
@ Clear External WRAM to 0x00

        mov     r1,#0x40
        lsl     r1,#12          @ r1 = 0x40000
        lsl     r0,r1,#7        @ r0 = 0x2000000
        bl      ClearMem

SkipEWRAMClear:
@        ldr     r0,=AgbMain
@        bx r0

@ Clear Internal WRAM to 0x00
        mov     r0,#3
        lsl     r0,#24          @ r0 = 0x3000000
        ldr     r1,=__sp_usr_offset - 16
        bl      ClearMem

 .ifdef __MultiBootInclude
@ Clear BSS section to 0x00
@ (Sometimes BSS may be in External WRAM)
        ldr     r0,=__bss_start
        ldr     r1,=__bss_end
        sub     r1,r0
        bl      ClearMem
 .endif

@ Copy initialized data (data section) from LMA to VMA (ROM to RAM)
        ldr     r1,=__data_lma
        ldr     r2,=__data_start
        ldr     r4,=__data_end
        bl      CopyMemChk

@ Copy internal work ram (iwram section) from LMA to VMA (ROM to RAM)
        ldr     r1,=__iwram_lma
        ldr     r2,=__iwram_start
        ldr     r4,=__iwram_end
        bl      CopyMemChk

@ Copy internal work ram overlay 0 (iwram0 section) from LMA to VMA (ROM to RAM)
        ldr     r2,=__load_stop_iwram0
        ldr     r1,=__load_start_iwram0
        sub     r3,r2,r1         @ Is there any data to copy?
        beq     CIW0Skip         @ no

        ldr     r2,=__iwram_overlay_start
        bl      CopyMem
CIW0Skip:

@ Copy external work ram (ewram section) from LMA to VMA (ROM to RAM)
        ldr     r1,=__ewram_lma
        ldr     r2,=__ewram_start
        ldr     r4,=__ewram_end
        bl      CopyMemChk

@ Copy external work ram overlay 0 (ewram0 section) from LMA to VMA (ROM to RAM)
        ldr     r2,=__load_stop_ewram0
        ldr     r1,=__load_start_ewram0
        sub     r3,r2,r1         @ Is there any data to copy?
        beq     CEW0Skip         @ no

        ldr     r2,=__ewram_overlay_start
        bl      CopyMem
CEW0Skip:

@ Jump to user code

        mov     r0,#0            @ int argc
        mov     r1,#0            @ char *argv[]

        ldr     r3,=start_vector
        mov     lr,r3            @ Set start_vector as return address

.ifdef __CPPSupport
        ldr     r3,=main
.else
        ldr     r3,=AgbMain
.endif
        bx      r3


    .GLOBAL     __FarFunction,__FarProcedure
    .THUMB_FUNC
__FarFunction:
    .THUMB_FUNC
__FarProcedure:
        bx      r0
        nop
        nop          @ This nop is here to allow unmapped memory to be used as
                     @ as a delay of almost 1 sec with a 1 cycle resolution.
                     @ Read this for technical info:
                     @  http://www.devrs.com/gba/files/gbadevfaqs.php#RepeatUses

@ Clear memory to 0x00 if length != 0
@ r0 = Start Address
@ r1 = Length

ClearMem:
@        cmp     r1,#0           @ Is length zero?
@        beq     ClearMX         @ yes, exit

        mov     r2,#3            @ These commands are used in cases where
        add     r1,r2            @ the length is not a multiple of 4,
        bic     r1,r2            @ even though it should be.

        beq     ClearMX         @ Length is zero so exit

.ifdef __DMACopyClear
        ldr     r2,reg_base
        lsr     r1,#2           @ r1 = (length/4) & 0xffff

        adr     r3,fill_val
        str     r3,[r2,#0x4]    @ Set source address (fill value)
        str     r0,[r2,#0x8]    @ Set destination address (fill dest address)
        strh    r1,[r2,#0xc]    @ Set DMA length
        ldr     r1,=0x8500      @ dma_clrb
        strh    r1,[r2,#0xe]    @ Start DMA
.else
        mov     r2,#0
ClrLoop:
        stmia   r0!,{r2}
        sub     r1,#4
        bne     ClrLoop
.endif
ClearMX:
        bx      lr

@ Copy memory if length != 0
@ r1 = Source Address
@ r2 = Dest Address
@ r4 = Dest Address + Length

CopyMemChk:
        sub     r3,r4,r2         @ Is there any data to copy?
@        beq     CIDExit          @ no

@ Copy memory
@ r1 = Source Address
@ r2 = Dest Address
@ r3 = Length

CopyMem:
        mov     r0,#3            @ These commands are used in cases where
        add     r3,r0            @ the length is not a multiple of 4,
        bic     r3,r0            @ even though it should be.

        beq     CIDExit          @ Length is zero so exit

.ifdef __DMACopyClear
        ldr     r0,reg_base
        lsr     r3,#2           @ r3 = (length/4) & 0xffff

        str     r1,[r0,#0x4]    @ Set source address
        str     r2,[r0,#0x8]    @ Set destination address
        strh    r3,[r0,#0xc]    @ Set DMA length
        ldr     r3,=0x8400      @ dma_copy
        strh    r3,[r0,#0xe]    @ Start DMA
.else
CIDLoop:
        ldmia   r1!,{r0}
        stmia   r2!,{r0}
        sub     r3,#4
        bne     CIDLoop
.endif
CIDExit:
If_Undefined_Reference__rename_main_or_AgbMain_to_each_other_in_your_C_file:
        bx      lr

        .ALIGN

.ifdef __DMACopyClear
fill_val: .word  0
reg_base: .word  0x040000d0
.endif

        .ALIGN
        .POOL

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@                  Interrupt Processing                  @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

 .ifdef __ISRinIWRAM
    .SECTION    .iwram,"ax",%progbits
 .endif

    .EXTERN     IntrTable
    .EXTERN     InterruptProcess
    .GLOBAL     intr_main
    .ALIGN
    .ARM

@ NOTE: Ifyou copy the following code (start: intr_main -
@ end: intr_main_end) to internal WRAM then do not forget
@ to copy everything between these two labels. The .POOL
@ data must be copied since it is used by intr_main.

@ NOTE2: If __ISRinIWRAM is defined then the copy to
@ IWRAM is done automatically for you.

 .ifdef __InterruptSupport

  .ifdef __FastInterrupts
intr_main:
        ldr     r0,=InterruptProcess
        bx      r0
  .endif

  .ifdef __SingleInterrupts
intr_main:
                                         @ Single interrupts support
        mov     r3, #0x4000000           @ REG_BASE
        ldr     r2, [r3,#0x200]!         @ Read REG_IE
        and     r1, r2, r2, lsr #16      @ r1 = IE & IF
        ldr     r2, =IntrTable

        ands    r0, r1, #1               @ V-Blank Interrupt
        bne     jump_intr
        add     r2, r2, #4
        ands    r0, r1, #2               @ H-Blank Interrupt
        bne     jump_intr
        add     r2, r2, #4
        ands    r0, r1, #4               @ V Counter Interrupt
        bne     jump_intr
        add     r2, r2, #4
        ands    r0, r1, #8               @ Timer 0 Interrupt
        bne     jump_intr
        add     r2, r2, #4
        ands    r0, r1, #0x10            @ Timer 1 Interrupt
        bne     jump_intr
        add     r2, r2, #4
        ands    r0, r1, #0x20            @ Timer 2 Interrupt
        bne     jump_intr
        add     r2, r2, #4
        ands    r0, r1, #0x40            @ Timer 3 Interrupt
        bne     jump_intr
        add     r2, r2, #4
        ands    r0, r1, #0x80            @ Serial Communication Interrupt
        bne     jump_intr
        add     r2, r2, #4
        ands    r0, r1, #0x100           @ DMA0 Interrupt
        bne     jump_intr
        add     r2, r2, #4
        ands    r0, r1, #0x200           @ DMA1 Interrupt
        bne     jump_intr
        add     r2, r2, #4
        ands    r0, r1, #0x400           @ DMA2 Interrupt
        bne     jump_intr
        add     r2, r2, #4
        ands    r0, r1, #0x800           @ DMA3 Interrupt
        bne     jump_intr
        add     r2, r2, #4
        ands    r0, r1, #0x1000          @ Key Interrupt
        bne     jump_intr
        add     r2, r2, #4
        ands    r0, r1, #0x2000          @ Cart Interrupt

   .ifdef __HandleCartInterrupt
        strneb  r0, [r3, #0x84 - 0x200]  @ Stop sound if cart removed (REG_SOUNDCNT_X)
loop:   bne     loop                     @ Infinite loop if cart removed
   .endif

jump_intr:
        strh    r0, [r3, #2]             @ IF Clear
        ldr     r0, [r2]                 @ Jump to user IRQ process
        bx      r0
  .endif

  .ifdef __MultipleInterrupts
intr_main:
                                         @ Multiple interrupts support
        mov     r2, #0x4000000           @ REG_BASE
        ldr     r3, [r2,#0x200]!         @ r2 = IE : r3 = IF|IE
        ldrh    r1, [r2, #0x8]           @ r1 = IME
        mrs     r0, spsr
        stmfd   sp!, {r0-r2,lr}          @ {spsr, IME, REG_IE, lr}  // IF|IE

        mov     r0, #1                   @ IME = 1 (To permit multiple interrupts if
                                         @ an interrupt occurs)
        strh    r0, [r2, #0x8]
        and     r1, r3, r3, lsr #16      @ r1 = IE & IF
        ldr     r12, =IntrTable

        ands    r0, r1, #1               @ V-blank interrupt
        bne     jump_intr
        add     r12,r12, #4
        ands    r0, r1, #2               @ H-blank interrupt
        bne     jump_intr
        add     r12,r12, #4
        ands    r0, r1, #4               @ V-counter interrupt
        bne     jump_intr
        add     r12,r12, #4
        ands    r0, r1, #8               @ Timer 0 interrupt
        bne     jump_intr
        add     r12,r12, #4
        ands    r0, r1, #0x10            @ Timer 1 interrupt
        bne     jump_intr
        add     r12,r12, #4
        ands    r0, r1, #0x20            @ Timer 2 interrupt
        bne     jump_intr
        add     r12,r12, #4
        ands    r0, r1, #0x40            @ Timer 3 interrupt
        bne     jump_intr
        add     r12,r12, #4
        ands    r0, r1, #0x80            @ Serial Communication Interrupt
        bne     jump_intr
        add     r12,r12, #4
        ands    r0, r1, #0x100           @ DMA 0 interrupt
        bne     jump_intr
        add     r12,r12, #4
        ands    r0, r1, #0x200           @ DMA 1 interrupt
        bne     jump_intr
        add     r12,r12, #4
        ands    r0, r1, #0x400           @ DMA 2 interrupt
        bne     jump_intr
        add     r12,r12, #4
        ands    r0, r1, #0x800           @ DMA 3 interrupt
        bne     jump_intr
        add     r12,r12, #4
        ands    r0, r1, #0x1000          @ Key interrupt
        bne     jump_intr
        add     r12,r12, #4
        ands    r0, r1, #0x2000          @ Cart interrupt

   .ifdef __HandleCartInterrupt
        strneb  r0, [r2, #0x84 - 0x200]  @ Stop sound if cart removed (REG_SOUNDCNT_X)
loop:   bne     loop                     @ Infinite loop if cart removed
   .endif

jump_intr:
        strh    r0, [r2, #2]             @ Clear IF

@ Enable multiple interrupts & switch to system
@ mode if __SwitchToUserStack is defined.

        mrs     r3, cpsr
   .ifdef __SwitchToUserStack
        bic     r3, r3, #0xdf            @ \__
        orr     r3, r3, #0x1f            @ /  --> Enable IRQ & FIQ. Set CPU mode to System.
   .else
        bic     r3, r3, #0xc0            @ Enable IRQ & FIQ
   .endif
        msr     cpsr, r3

        ldr     r0, [r12]

        stmfd   sp!, {lr}
        adr     lr, IntrRet
        bx      r0
IntrRet:
        ldmfd   sp!, {lr}

@ Disable multiple interrupts & switch to IRQ Mode
@ if __SwitchToUserStack is defined.

        mrs     r3, cpsr
   .ifdef __SwitchToUserStack
        bic     r3, r3, #0xdf            @ \__
        orr     r3, r3, #0x92            @ /  --> Disable IRQ. Enable FIQ. Set CPU mode to IRQ.
   .else
        orr     r3, r3, #0x80            @ Disable IRQ.
   .endif
        msr     cpsr, r3

        ldmfd   sp!, {r0-r2,lr}          @ {spsr, IME, REG_IE, lr}  //IF|IE
@        strh    r3,  [r2]                @ set IE
        strh    r1,  [r2, #0x8]          @ restore REG_IME
        msr     spsr, r0                 @ restore spsr
        bx      lr

  .endif

    .ALIGN
    .POOL               @ Ifyou copy the intr_main routine, above, to internal
                        @ RAM then copy the pool data as well because IntrTable
                        @ address is stored here. Use intr_main_end as last address+1.
intr_main_end:
 .endif

    .ALIGN
    .POOL


    .END

