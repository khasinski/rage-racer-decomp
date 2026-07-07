#include "common.h"


asm(
    ".set noreorder\n"
    ".set noat\n"
    ".globl D_80064554\n"
    "D_80064554:\n"
    ".word 0x00FFFFFF\n"
    "lui $8,0x8006\n"
    "addiu $8,$8,0x4554\n"
    "addi $1,$4,-1\n"
    ".word 0x18200004\n"
    "lw $2,0($8)\n"
    "sll $1,$4,1\n"
    "jr $31\n"
    "sw $1,0($8)\n"
    "1:\n"
    "lui $1,0x00FF\n"
    "ori $1,$1,0xFFFF\n"
    "jr $31\n"
    "sw $1,0($8)");
