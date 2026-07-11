#include "common.h"

typedef struct {
    s32 field_0;
    u8 pad4[4];
    s32 step;
    s16 side;
    s16 phase;
    s32 out0;
    s32 out1;
    s32 out2;
    u8 pad1C[0x18];
} Unk3F2A4;

extern Unk3F2A4 D_801E4FB8[];
extern u8 D_8007E360[];
extern s16 D_8007E3D8[];
extern s16 D_8007E3E0[];

void func_8003F2A4(s32 arg0) {
    Unk3F2A4 *entry;
    register s32 phase asm("$2");
    register s32 side asm("$10");
    register s32 step asm("$6");
    register s32 baseIndex asm("$7");
    register s32 altIndex asm("$4");
    register s32 phaseOffset asm("$12");
    register s32 phaseShift asm("$9");
    register s16 *limitPtr asm("$8");
    register s16 *tailLimitPtr asm("$8");
    register s32 denom asm("$5");
    register s32 temp asm("$3");
    register s32 value asm("$2");

    entry = &D_801E4FB8[arg0];
    asm("" : "=r"(entry) : "0"(entry));
    limitPtr = D_8007E3D8;
    side = entry->side;
    phase = entry->phase;
    step = entry->step;
    baseIndex = side << 4;
    phaseShift = phase << 5;
    baseIndex += phaseShift;
    phaseOffset = phase << 1;
    limitPtr = (s16 *)((s32)limitPtr + phaseOffset);
    denom = *limitPtr;
    temp = *(s32 *)(D_8007E360 + baseIndex);
    value = (denom - step) * temp;
    altIndex = (1 - side) << 4;
    altIndex += phaseShift;
    value = (value + (step * *(s32 *)(D_8007E360 + altIndex))) / denom;
    entry->out0 = value;

    denom = *limitPtr;
    temp = *(s32 *)(D_8007E360 + baseIndex + 4);
    value = (denom - step) * temp;
    value = (value + (step * *(s32 *)(D_8007E360 + altIndex + 4))) / denom;
    entry->out1 = value;

    denom = *limitPtr;
    temp = *(s32 *)(D_8007E360 + baseIndex + 8);
    value = (denom - step) * temp;
    value = (value + (step * *(s32 *)(D_8007E360 + altIndex + 8))) / denom;
    entry->out2 = value;

    if (entry->step >= *limitPtr) {
        entry->step = 0;
        entry->field_0 = 0;
        entry->side ^= 1;
        return;
    }

    phase = (s32)D_8007E3E0;
    tailLimitPtr = (s16 *)(phaseOffset + phase);
    asm("" : "=r"(tailLimitPtr) : "0"(tailLimitPtr));
    if (entry->field_0 >= *tailLimitPtr) {
        entry->step = entry->step + 1;
        entry->field_0 = *tailLimitPtr;
        return;
    }
    entry->field_0 = entry->field_0 + 1;
}
