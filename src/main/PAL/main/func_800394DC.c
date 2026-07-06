#include "common.h"
#include "game/car.h"

extern s32 D_801E408C;
extern u8 *D_801E4150;

void func_800394DC(GameCarRuntime *obj, s32 arg1) {
    register GameCarRuntime *objReg asm("$6") = obj;
    register s32 target asm("$7");
    register u8 *state asm("$8");
    s32 index;
    register s32 advanceOffset asm("$4");
    register s32 scene asm("$3");
    register s32 offset asm("$3");
    register u8 *base asm("$2");
    register s16 *entry asm("$3");
    register s32 value asm("$5");
    register s32 valueRaw asm("$4");
    register s32 raw asm("$2");
    s32 stack[2];

    raw = objReg->field_70;
    asm volatile("" : : "r"(stack));
    scene = D_801E408C;
    target = raw >> 4;
    index = objReg->routeIndex;
    raw = (scene << 1) + scene;
    offset = (raw << 4) - raw;
    offset <<= 3;
    base = D_801E4150;
    offset += 0x84;
    entry = (s16 *)(offset + (s32)base);
    raw = (index << 1) + index;
    raw <<= 2;
    asm("addu $3,$3,$2" : "=r"(entry) : "0"(entry), "r"(raw));

    if (target < 0x20) {
        state = (u8 *)&objReg->field_BC;
        objReg->routeIndex = 0;
        target = 0;
    } else {
        state = (u8 *)&objReg->field_BC;
    }

    if (target < entry[0]) {
        goto below;
    }
    if (entry[1] < target) {
        goto advance;
    }
    if (arg1 < 4 && objReg->field_10C == 0) {
        valueRaw = objReg->field_11C;
        if (entry[2] < valueRaw) {
            value = valueRaw;
            asm volatile("" : : "r"(value));
            raw = entry[3];
            asm("slt $2,$4,$2" : "=r"(raw) : "0"(raw), "r"(valueRaw));
            if (raw != 0) {
                raw = value + *(u16 *)(entry + 4);
                objReg->field_11C = raw;
            }
        }
    }
    return;

below:
    if (entry[1] < target) {
        goto advance;
    }
    goto clear;

advance:
    {
        register s32 next asm("$2");

        next = *(s32 *)(state + 0x44);
        scene = D_801E408C;
        next++;
        advanceOffset = (next << 1) + next;
        advanceOffset <<= 2;
        *(s32 *)(state + 0x44) = next;
    }
    raw = (scene << 1) + scene;
    offset = (raw << 4) - raw;
    offset <<= 3;
    asm volatile("" : "=r"(offset) : "0"(offset));
    base = D_801E4150;
    advanceOffset += offset;
    raw = (s32)(base + advanceOffset);
    if (*(s16 *)(raw + 0x84) == -1) {
        *(s32 *)(state + 0x44) = 0;
    }
    *(s16 *)(state + 0x52) = 0;
    return;

clear:
    objReg->field_10E = 0;
}
