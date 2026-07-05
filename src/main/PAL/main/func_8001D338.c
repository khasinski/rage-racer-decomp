#include "common.h"

extern u8 *D_8019CA64;
extern u16 D_801E444C[];
extern u16 D_801E444E[];
extern u16 D_801E6F2C[];

void func_8001D338(s32 arg0, s32 arg1) {
    register s32 index asm("a2");
    register u16 *dst asm("a3");
    register u16 *src asm("a0");
    register u16 *src0 asm("a2");
    register s32 row0 asm("t1");
    register s32 row1 asm("t2");
    register s32 adjusted asm("v0");
    register s32 outer asm("t0");
    s32 j;
    u16 value;
    u16 fill;

    adjusted = arg1 + ((u32)arg1 >> 31);
    row1 = (adjusted >> 1) + 10;
    __asm__("" : "=r"(row1) : "0"(row1));
    arg1 &= 1;
    index = 1;
    dst = D_801E444E;

    __asm__ volatile(
        "srl   $v0,$a0,31\n"
        "addu  $v0,$a0,$v0\n"
        "sra   $t1,$v0,1\n"
        "andi  $a0,$a0,1\n"
        "lui   $v1,%%hi(D_8019CA64)\n"
        "lw    $v1,%%lo(D_8019CA64)($v1)\n"
        "sll   $v0,$t1,5\n"
        "addu  $v0,$v0,$t1\n"
        "sll   $v0,$v0,6\n"
        "sll   $a0,$a0,5\n"
        "addu  $v0,$v0,$v1\n"
        "addu  $a0,$a0,$v0\n"
        "addiu $a0,$a0,2\n"
        : "=r"(src), "=r"(row0)
        :
        : "$2", "$3");

    do {
        *dst++ = *src++;
        index++;
    } while (index < 12);

    if (index < 16) {
        register s32 byteIndex asm("a0");

        byteIndex = index * 2;
        __asm__ volatile(
            "lui   $v0,%%hi(D_801E444C)\n"
            "addiu $v0,$v0,%%lo(D_801E444C)\n"
            "addu  $a3,$a0,$v0\n"
            "lui   $v1,%%hi(D_8019CA64)\n"
            "lw    $v1,%%lo(D_8019CA64)($v1)\n"
            "sll   $v0,$t2,5\n"
            "addu  $v0,$v0,$t2\n"
            "sll   $v0,$v0,6\n"
            "addu  $v0,$v0,$v1\n"
            "sll   $v1,$a1,5\n"
            "addu  $v1,$v1,$v0\n"
            "addu  $a0,$a0,$v1\n"
            : "=r"(src), "=r"(dst)
            : "0"(byteIndex), "r"(row1), "r"(arg1)
            : "$2", "$3");

        do {
            *dst++ = *src++;
            index++;
        } while (index < 16);
    }

    dst = D_801E6F2C;
    outer = 0;
    __asm__ volatile(
        "sll   $v0,$t1,5\n"
        "addu  $v0,$v0,$t1\n"
        "lui   $v1,%%hi(D_8019CA64)\n"
        "lw    $v1,%%lo(D_8019CA64)($v1)\n"
        "sll   $v0,$v0,6\n"
        "addu  $v0,$v0,$v1\n"
        "addiu $a2,$v0,0x40\n"
        "sll   $v0,$t2,5\n"
        "addu  $v0,$v0,$t2\n"
        "sll   $v0,$v0,6\n"
        "addu  $v0,$v0,$v1\n"
        "addiu $a0,$v0,0x40\n"
        : "=r"(src0), "=r"(src)
        : "r"(row1), "r"(row0)
        : "$2", "$3");

    for (; outer < 64; outer++) {
        for (j = 0; j < 16; j++) {
            value = *src0;

            if ((value & 0x000F) == 0) {
                fill = *src & 0x000F;
                value |= fill;
            }
            if ((value & 0x00F0) == 0) {
                fill = *src & 0x00F0;
                value |= fill;
            }
            if ((value & 0x0F00) == 0) {
                fill = *src & 0x0F00;
                value |= fill;
            }
            src0++;
            if ((value & 0xF000) == 0) {
                fill = *src & 0xF000;
                value |= fill;
            }

            *dst++ = value;
            src++;
        }
    }
}
