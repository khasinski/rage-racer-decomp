#include "common.h"

void func_80046E00(u8 *arg0, s16 x0, s16 y0, s16 x1a, s16 y0b, s16 x0b,
                   s16 y1a, s16 x1b, s16 y1b, s32 d0, s32 d1, s32 d2,
                   s32 d3, s32 d4, s32 d5, s32 d6, s32 d7, s32 dA,
                   s32 dB, s32 dC, s32 h8, s32 f8, s32 f4, s32 dE);

void func_80048580(s32 time, u8 *desc, s32 *ctx) {
    register s32 duration asm("$7");
    register u8 *table asm("$13");
    register u8 *entry asm("$8");
    s32 velocity0;
    s32 velocity1;
    s32 x;
    s32 y;
    s32 dx;
    s32 dy;
    register s32 index asm("$10");
    register s32 posX asm("$5");
    register s32 posY asm("$5");
    register s32 posX2 asm("$3");
    register s32 posY2 asm("$6");
    u32 velocityX;
    u32 velocityY;
    u32 velocityX2;
    u32 velocityY2;
    register s32 value asm("$2");
    s32 flags;

    duration = ctx[0];
    table = *(u8 **)0x1F800004;
    velocity0 = ctx[3];
    velocity1 = ctx[4];
    entry = desc;
    if (duration < time) {
        time = duration;
    }

    posX = *(s16 *)((u8 *)ctx + 4);
    if (velocity0 & 0x8000) {
        velocityX = velocity0 | 0xFFFF0000;
    } else {
        velocityX = velocity0 & 0x7FFF;
    }
    posX += (time * velocityX) >> 5;
    asm("" : "=r"(posX) : "0"(posX));
    x = posX;
    asm("" : "=r"(x) : "0"(x));

    posY = *(s16 *)((u8 *)ctx + 6);
    if (velocity0 < 0) {
        value = velocity0 >> 16;
        velocityY = value | 0xFFFF0000;
    } else {
        value = velocity0 >> 16;
        velocityY = value & 0x7FFF;
    }
    value = posY + ((time * velocityY) >> 5);
    asm("" : "=r"(value) : "0"(value));
    y = value;
    asm("" : "=r"(y) : "0"(y) : "memory");

    posX2 = *(s16 *)((u8 *)ctx + 8);
    if (velocity1 & 0x8000) {
        velocityX2 = velocity1 | 0xFFFF0000;
    } else {
        velocityX2 = velocity1 & 0x7FFF;
    }
    value = posX2 + ((time * velocityX2) >> 5);
    asm("" : "=r"(value) : "0"(value) : "memory");
    dx = value;

    posY2 = *(s16 *)((u8 *)ctx + 0xA);
    if (velocity1 < 0) {
        value = velocity1 >> 16;
        velocityY2 = value | 0xFFFF0000;
    } else {
        value = velocity1 >> 16;
        velocityY2 = value & 0x7FFF;
    }
    posY2 += (time * velocityY2) >> 5;
    asm("" : "=r"(posY2) : "0"(posY2));
    dy = posY2;

    switch (entry[0xD] & 3) {
    case 0:
        asm(".globl func_800486C0\nfunc_800486C0 = . + 4");
        index = 0;
        break;
    case 1:
        index = 3;
        break;
    case 2:
        index = 5;
        break;
    case 3:
        index = 0x2BE;
        break;
    }

    flags = entry[0xD];
    func_80046E00(table + index * 4, x, y, x + dx, y, x, y + dy,
                  x + dx, y + dy, entry[0], entry[1], entry[2], entry[3],
                  entry[4], entry[5], entry[6], entry[7], entry[0xA],
                  entry[0xB], entry[0xC], *(u16 *)(entry + 8), flags & 8,
                  flags & 4, entry[0xE]);
}
