@ GBA ROM Header

.section .header, "ax"

.global _start
.arm
_start:

    @@@@@@@@@@@@@@@@@@@@@@
    @     ROM Header     @
    @@@@@@@@@@@@@@@@@@@@@@

    @ ROM Entry Point
        b start_vector

    @ Nintendo Logo
        .word   0x51aeff24, 0x21a29a69, 0x0a82843d
        .word   0xad09e484, 0x988b2411, 0x217f81c0, 0x19be52a3
        .word   0x20ce0993, 0x4a4a4610, 0xec3127f8, 0x33e8c758
        .word   0xbfcee382, 0x94dff485, 0xc1094bce, 0xc08a5694
        .word   0xfca77213, 0x734d849f, 0x619acaa3, 0x27a39758
        .word   0x769803fc, 0x61c71d23, 0x56ae0403, 0x008438bf
        .word   0xfd0ea740, 0x03fe52ff, 0xf130956f, 0x85c0fb97
        .word   0x2580d660, 0x03be63a9, 0xe2384e01, 0xff34a2f9
        .word   0x44033ebb, 0xcb900078, 0x943a1188, 0x637cc065
        .word   0xaf3cf087, 0x8be425d6, 0x72ac0a38, 0x07f8d421

    @ Game Title
        .ascii  "MINICRAFTGBA"

    @ Game Code
        .ascii  "ZMCE"

    @ Maker Code
        .byte   0x00, 0x00

    @ Fixed value
        .byte   0x96

    @ Main Unit Code
        .byte   0x00

    @ Device Type
        .byte   0x00

    @ Reserved (7 Bytes)
        .space  7, 0x00

    @ Software Version
        .byte   0x04

    @ Header Checksum
        .byte   0xb7

    @ Reserved (2 Bytes)
        .space  2, 0x00

    @@@@@@@@@@@@@@@@@@@@@@@@@@
    @     ROM Header End     @
    @@@@@@@@@@@@@@@@@@@@@@@@@@

    @ Cart Backup ID
        .ascii  "FLASH1M_Vnnn"

.end
