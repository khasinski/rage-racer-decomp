#include "common.h"

s32 func_8002A788(s32 arg0, s32 arg1);
s32 func_80068568(s32 arg0);
s32 func_80068634(s32 arg0);

void func_80038CE8(u8 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    register u8 *obj asm("$17") = arg0;
    register s32 velX asm("$16") = arg1;
    register s32 angle asm("$18");
    register s32 tempAngle asm("$19");
    register s32 velZ asm("$6") = arg2;
    register s32 baseAngleReg asm("$4");
    s32 angleTemp;
    register s32 baseAngle asm("$3");
    s32 value;
    s32 mode;

    *(s16 *)(obj + 0x80) = 1;

    if (arg3 < 2) {
        baseAngle = 0xC00 - *(u16 *)(obj + 0xB4);
        baseAngleReg = baseAngle;
        baseAngle = (s16)baseAngle;
        if (*(s32 *)(obj + 0x34) < 0) {
            angleTemp = (baseAngle - 0x400) & 0xFFF;
        } else {
            angleTemp = (baseAngle + 0x400) & 0xFFF;
        }

        value = *(s32 *)(obj + 0xA4);
        angle = angleTemp;
        if (value >= 0x321) {
            if (value < 0x709) {
                velX = *(u16 *)(obj + 0xA4);
            } else {
                velX = 0x708;
            }
            value = func_80068568(func_8002A788((s16)baseAngleReg, *(s32 *)(obj + 0x24)));
            value = (s16)velX * value;
            if (value < 0) {
                value += 0xFFFF;
            }
            value >>= 16;
        } else {
            value = func_80068568(func_8002A788((s16)baseAngleReg, *(s32 *)(obj + 0x24)));
            value *= 50;
            if (value < 0) {
                value += 0xFFF;
            }
            value >>= 12;
        }

        velX = value + 10;
        tempAngle = angle;
        angle = (s16)velX;
        value = func_80068568(tempAngle) * angle;
        if (value < 0) {
            value += 0xFFF;
        }
        velX = value >> 12;
        value = func_80068634(tempAngle) * angle;
        if (value < 0) {
            value += 0xFFF;
        }
        velZ = value >> 12;
        mode = 0x1E;
    } else {
        if (arg3 < 4) {
            baseAngle = 0xC00 - *(u16 *)(obj + 0xB4);
            baseAngle = (s16)baseAngle;
            if (*(s32 *)(obj + 0x34) < 0) {
                angle = (baseAngle - 0x400) & 0xFFF;
            } else {
                angle = (baseAngle + 0x400) & 0xFFF;
            }

            value = func_80068568(angle) * 20;
            if (value < 0) {
                value += 0xFFF;
            }
            velX = value >> 12;
            value = func_80068634(angle) * 20;
            if (value < 0) {
                value += 0xFFF;
            }
            velZ = value >> 12;
        } else {
            velX = (velX + (s32)((u32)velX >> 31)) >> 1;
            velZ = (velZ + (s32)((u32)velZ >> 31)) >> 1;
        }
        mode = 0xF;
    }

    *(s16 *)(obj + 0x82) = mode;
    *(s16 *)(obj + 0x7C) = velX;
    *(s16 *)(obj + 0x7E) = velZ;
}
