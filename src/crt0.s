@ Copyright 2022 Vulcalien
@
@ This program is free software: you can redistribute it and/or modify
@ it under the terms of the GNU General Public License as published by
@ the Free Software Foundation, either version 3 of the License, or
@ (at your option) any later version.
@
@ This program is distributed in the hope that it will be useful,
@ but WITHOUT ANY WARRANTY; without even the implied warranty of
@ MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
@ GNU General Public License for more details.
@
@ You should have received a copy of the GNU General Public License
@ along with this program.  If not, see <https://www.gnu.org/licenses/>.

@ Based on crt0.S v1.28 by Jeff Frohwein

.text

.global _start
.arm
_start:
@@@@@@@@@@@@@@@@@@@@@@
@     ROM Header     @
@@@@@@@@@@@@@@@@@@@@@@
    @ ROM Entry Point
        b start_vector

    @ Nintendo Logo
        .long   0x51aeff24,0x21a29a69,0x0a82843d
        .long   0xad09e484,0x988b2411,0x217f81c0,0x19be52a3
        .long   0x20ce0993,0x4a4a4610,0xec3127f8,0x33e8c758
        .long   0xbfcee382,0x94dff485,0xc1094bce,0xc08a5694
        .long   0xfca77213,0x734d849f,0x619acaa3,0x27a39758
        .long   0x769803fc,0x61c71d23,0x56ae0403,0x008438bf
        .long   0xfd0ea740,0x03fe52ff,0xf130956f,0x85c0fb97
        .long   0x2580d660,0x03be63a9,0xe2384e01,0xff34a2f9
        .long   0x44033ebb,0xcb900078,0x943a1188,0x637cc065
        .long   0xaf3cf087,0x8be425d6,0x72ac0a38,0x07f8d421

    @ Game Title
        .ascii  "MINICRAFTGBA"

    @ Game Code
        .ascii  "ZMCE"

    @ Maker Code
        .byte   0x00,0x00

    @ Fixed value
        .byte   0x96

    @ Main Unit Code
        .byte   0x00

    @ Device Type
        .byte   0x00

    @ Reserved (7 Bytes)
        .space  7, 0x00

    @ Software Version
        .byte   0x00

    @ Complement Check
        .byte   0xbb

    @ Reserved (2 Bytes)
        .space  2, 0x00

@@@@@@@@@@@@@@@@@@@@@@@@@@
@     ROM Header End     @
@@@@@@@@@@@@@@@@@@@@@@@@@@

    @ Cart Backup ID
        .ascii  "FLASH1M_Vnnn"

.global start_vector
.align
.arm
start_vector:
        @ Set SP_irq
        mov     r0, #0x12               @ Switch to IRQ Mode
        msr     cpsr, r0
        ldr     sp, =__sp_irq

        @ Set SP_usr
        mov     r0, #0x1f               @ Switch to System Mode
        msr     cpsr, r0
        ldr     sp, =__sp_usr

        @ Switch to Thumb mode
        add     r0, pc, #1
        bx      r0

        .thumb

        @ Clear EWRAM
        mov     r1, #0x40
        lsl     r1, #12                 @ r1 = 0x40000
        lsl     r0, r1, #7              @ r0 = 0x2000000
        bl      ClearMem

        @ Clear IWRAM
        mov     r0, #3
        lsl     r0, #24                 @ r0 = 0x3000000
        ldr     r1, =__sp_usr_offset - 16
        bl      ClearMem

        @ Copy .data section
        ldr     r0, =__data_lma
        ldr     r1, =__data_start
        ldr     r2, =__data_end
        sub     r2, r1
        bl      CopyMem

        @ Copy .ewram section
        ldr     r0, =__ewram_lma
        ldr     r1, =__ewram_start
        ldr     r2, =__ewram_end
        sub     r2, r1
        bl      CopyMem

        @ Copy .iwram section
        ldr     r0, =__iwram_lma
        ldr     r1, =__iwram_start
        ldr     r2, =__iwram_end
        sub     r2, r1
        bl      CopyMem

        @ Call AgbMain
        mov     r0, #0                  @ int argc
        mov     r1, #0                  @ char *argv[]

        ldr     r2, =start_vector       @ Set start_vector as return address
        mov     lr, r2

        ldr     r2, =AgbMain
        bx      r2

.align
.pool

@ r0 = Start Address
@ r1 = Length
.thumb_func
ClearMem:
        @ Return if Length is 0
        cmp     r1, #0
        beq     2f

        mov     r2, #0
1: @ loop
        stmia   r0!, {r2}
        sub     r1, #4
        bgt     1b                      @ 'bgt' in case Length % 4 != 0

2: @ exit
        bx      lr

@ r0 = Source Address
@ r1 = Dest Address
@ r2 = Length
.thumb_func
CopyMem:
        @ Return if Length is 0
        cmp     r2, #0
        beq     2f

1: @ loop
        ldmia   r0!, {r3}
        stmia   r1!, {r3}
        sub     r2, #4
        bgt     1b                      @ 'bgt' in case Length % 4 != 0

2: @ exit
        bx      lr

.end
