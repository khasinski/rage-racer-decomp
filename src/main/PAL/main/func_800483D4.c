#include "common.h"

void func_80046BA0(void *ot, s16 x0, s16 y0, s16 x1, s32 y1, s32 x2, s32 y2,
                   s32 r, s32 g, s32 b, s32 semiTrans, s32 flags);

void func_800483D4(s32 time, u8 *styleArg, u8 *recordArg) {
    register u8 *style asm("$5");
    register u8 *record asm("$6");
    register void *ot asm("$13");
    register s32 limit asm("$7");
    register s32 packedSpeed asm("$3");
    register s32 product asm("$2");
    register s32 x asm("$9");
    register s32 y0 asm("$11");
    register s32 y asm("$6");
    register s32 y1 asm("$12");
    register s32 mode asm("$10");
    register s32 semiTrans asm("$8");
    register s32 flags asm("$4");
    s32 callArgs[12];

    style = styleArg;
    record = recordArg;
    /* Match note: preserve the original independent load schedule. */
    asm volatile(
        "lw %0,0(%3)\n"
        "lui %1,0x1f80\n"
        "lw %1,4(%1)\n"
        "lw %2,8(%3)"
        : "=r"(limit), "=r"(ot), "=r"(packedSpeed)
        : "r"(record));
    if (limit < time) {
        time = limit;
    }

    limit = *(s16 *)(record + 4);
    if (packedSpeed & 0x8000) {
        product = packedSpeed | 0xFFFF0000;
    } else {
        product = packedSpeed & 0x7FFF;
    }
    product = time * product;
    asm("" : "=r"(product), "=r"(record) : "0"(product), "1"(record));
    product = (u32)product >> 5;
    product = limit + product;
    asm("" : "=r"(product) : "0"(product));

    y = *(s16 *)(record + 6);
    x = product;
    if (packedSpeed < 0) {
        product = packedSpeed >> 16;
        packedSpeed = 0xFFFF0000;
        product |= packedSpeed;
    } else {
        product = (packedSpeed >> 16) & 0x7FFF;
    }
    product = time * product;
    asm("" : "=r"(product), "=r"(style) : "0"(product), "1"(style));
    product = (u32)product >> 5;
    y += product;

    product = *(u16 *)(style + 2);
    packedSpeed = *(u16 *)(style + 6);
    y0 = product + y;
    y1 = packedSpeed + y;
    asm("" : "=r"(y0), "=r"(y1), "=r"(style) : "0"(y0), "1"(y1), "2"(style));
    product = *(u16 *)style;
    packedSpeed = *(u16 *)(style + 4);
    product = x + product;
    asm("" : "=r"(product) : "0"(product));
    limit = product;
    asm("" : "=r"(limit), "=r"(style) : "0"(limit), "1"(style));
    packedSpeed = x + packedSpeed;

    switch (style[0xB] & 3) {
    case 0:
        mode = 0;
        break;
    case 1:
        mode = 3;
        break;
    case 2:
        mode = 5;
        break;
    case 3:
        mode = 0x2BE;
        break;
    }

    {
        register s32 alpha asm("$2");

        alpha = style[0xB];
        semiTrans = alpha & 4;
        if (semiTrans != 0) {
            alpha &= 0x60;
            asm("" : "=r"(alpha) : "0"(alpha));
            flags = (u8)alpha;
        } else {
            flags = 0x80;
        }
    }

    /* Match note: keep the original o32 stack-argument and delay-slot order. */
    asm volatile(
        ".set\tnoreorder\n"
        "sll $2,$11,16\n"
        "sra $2,$2,16\n"
        "sw $2,16($sp)\n"
        "sll $2,$3,16\n"
        "sra $2,$2,16\n"
        "sw $2,20($sp)\n"
        "sll $2,$12,16\n"
        "sra $2,$2,16\n"
        "sll $6,$6,16\n"
        "sw $2,24($sp)\n"
        :
        : "r"(y0), "r"(packedSpeed), "r"(y1), "r"(y)
        : "memory");
    asm volatile(
        ".set\tnoreorder\n"
        "lbu $2,8($5)\n"
        "sll $7,$7,16\n"
        "sw $2,28($sp)\n"
        "lbu $2,9($5)\n"
        "sra $6,$6,16\n"
        "sw $2,32($sp)\n"
        "lbu $2,10($5)\n"
        "sra $7,$7,16\n"
        "sw $4,44($sp)\n"
        :
        : "r"(style), "r"(limit), "r"(flags)
        : "memory");
    asm volatile(
        ".set\tnoreorder\n"
        "sll $4,$10,2\n"
        "sll $5,$9,16\n"
        "addu $4,$13,$4\n"
        "sra $5,$5,16\n"
        "sw $8,40($sp)\n"
        "jal func_80046BA0\n"
        "sw $2,36($sp)\n"
        ".set\treorder"
        :
        : "r"(ot), "r"(x), "r"(mode), "r"(semiTrans), "m"(callArgs[0])
        : "$31", "memory");
}
