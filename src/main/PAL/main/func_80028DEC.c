#include "common.h"

/*
 * func_80028DEC: GTE geometry primitive (sibling of func_80028120 cluster). Hand-written asm: frameless, custom register-argument convention. Unmatchable as compiled C; documented inline asm.
 */
asm(
    ".set noreorder\n"
    ".set noat\n"
    ".globl func_80028DEC\n"
    "func_80028DEC:\n"
    ".word 0x8C860050\n"
    ".word 0x00052880\n"
    ".word 0x00A63021\n"
    ".word 0x8CC70000\n"
    ".word 0xAC9F008C\n"
    ".word 0x8C820068\n"
    ".word 0x8CE50000\n"
    ".word 0x10400017\n"
    ".word 0x20E80004\n"
    ".word 0x484A0000\n"  /* cfc2 */
    ".word 0x484B0800\n"  /* cfc2 */
    ".word 0x000A6400\n"
    ".word 0x000C6403\n"
    ".word 0x000C6022\n"
    ".word 0x000A6C03\n"
    ".word 0x000D6822\n"
    ".word 0x000D6C00\n"
    ".word 0x318CFFFF\n"
    ".word 0x018D6025\n"
    ".word 0x48CC0000\n"  /* ctc2 */
    ".word 0x000B6400\n"
    ".word 0x000C6403\n"
    ".word 0x000C6022\n"
    ".word 0x000B6C03\n"
    ".word 0x000D6C00\n"
    ".word 0x318CFFFF\n"
    ".word 0x018D6025\n"
    ".word 0x484A2800\n"  /* cfc2 */
    ".word 0x48CC0800\n"  /* ctc2 */
    ".word 0x000A5022\n"
    ".word 0x48CA2800\n"  /* ctc2 */
    ".word 0x10A00008\n"
    ".word 0xAC880080\n"
    ".word 0x0C00A3A7\n"
    ".word 0x24A50004\n"
    ".word 0x8C880080\n"
    ".word 0x00000000\n"
    ".word 0x8D050000\n"
    ".word 0x1000FFF8\n"
    ".word 0x21080004\n"
    ".word 0x8C9F008C\n"
    ".word 0x00000000\n"
    ".word 0x03E00008\n"
    ".word 0x00000000\n"
);
