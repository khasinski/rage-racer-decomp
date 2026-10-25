.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

/* PsyQ kernel call trampolines this pre-release build needs beyond retail's
 * src/main/PAL/lib/libapi/bios_call_stubs.c (see that file's own comment for
 * why these are hand-written, not compiler output: a raw jump through a
 * register with the call number set in the jump's own delay slot). Each of
 * these four is independently called by address elsewhere in this unit
 * (src/main/DEMO/lib/libapi/demo_kernel_stubs.c), hence four separate labels
 * rather than one opaque block. */

glabel func_80074B78                /* B(0x14) */
    /* 5D378 80074B78 B0000A24 */  addiu      $t2, $zero, 0xB0
    /* 5D37C 80074B7C 08004001 */  jr         $t2
    /* 5D380 80074B80 14000924 */   addiu     $t1, $zero, 0x14
endlabel func_80074B78
    /* 5D384 80074B84 00000000 */  nop

glabel func_80074B88                /* B(0x15) */
    /* 5D388 80074B88 B0000A24 */  addiu      $t2, $zero, 0xB0
    /* 5D38C 80074B8C 08004001 */  jr         $t2
    /* 5D390 80074B90 15000924 */   addiu     $t1, $zero, 0x15
endlabel func_80074B88
    /* 5D394 80074B94 00000000 */  nop

glabel func_80074B98                /* B(0x16) */
    /* 5D398 80074B98 B0000A24 */  addiu      $t2, $zero, 0xB0
    /* 5D39C 80074B9C 08004001 */  jr         $t2
    /* 5D3A0 80074BA0 16000924 */   addiu     $t1, $zero, 0x16
endlabel func_80074B98
    /* 5D3A4 80074BA4 00000000 */  nop

glabel ChangeClearRCntStub          /* B(0x5B) */
    /* 5D3A8 80074BA8 B0000A24 */  addiu      $t2, $zero, 0xB0
    /* 5D3AC 80074BAC 08004001 */  jr         $t2
    /* 5D3B0 80074BB0 5B000924 */   addiu     $t1, $zero, 0x5B
endlabel ChangeClearRCntStub
    /* 5D3B4 80074BB4 00000000 */  nop
