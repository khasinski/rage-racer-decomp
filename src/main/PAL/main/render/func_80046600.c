#include "common.h"

extern volatile s32 D_8007F61C;
extern s32 D_8007F620;
extern s32 D_8007F624;
extern s32 D_8007F628;

s32 func_80046598(s32 arg0, s32 arg1);
void func_80046248(s32 *arg0);

void func_80046600(void) {
    register s32 current asm("$2");
    register s32 tick asm("$3");
    register s32 currentOffset asm("$6");
    register s32 nextOffset asm("$5");
    register s32 currentValue asm("$7");
    register s32 scale asm("$8");
    s32 blend;
    s32 scaledTick;
    s32 values[8];

    tick = D_8007F624;
    current = D_8007F61C;
    scaledTick = tick * 10000;
    blend = func_80046598(scaledTick / *(s32 *)((s32)&D_8007F628 + (current << 5) + 0x18),
                          *(s32 *)((s32)&D_8007F628 + (current << 5) + 0x1C));

    currentOffset = D_8007F61C;
    nextOffset = D_8007F620;
    currentOffset <<= 5;
    nextOffset <<= 5;
    scale = 0x68DB8BAD;

    tick = *(s32 *)((s32)&D_8007F628 + nextOffset);
    currentValue = *(s32 *)((s32)&D_8007F628 + currentOffset);
    values[0] = (((tick - currentValue) * blend) / 10000) + currentValue;
    tick = *(s32 *)((s32)&D_8007F628 + nextOffset + 4);
    currentValue = *(s32 *)((s32)&D_8007F628 + currentOffset + 4);
    values[1] = (((tick - currentValue) * blend) / 10000) + currentValue;
    tick = *(s32 *)((s32)&D_8007F628 + nextOffset + 8);
    currentValue = *(s32 *)((s32)&D_8007F628 + currentOffset + 8);
    values[2] = (((tick - currentValue) * blend) / 10000) + currentValue;
    tick = *(s32 *)((s32)&D_8007F628 + nextOffset + 12);
    currentValue = *(s32 *)((s32)&D_8007F628 + currentOffset + 12);
    values[3] = (((tick - currentValue) * blend) / 10000) + currentValue;
    tick = *(s32 *)((s32)&D_8007F628 + nextOffset + 16);
    currentValue = *(s32 *)((s32)&D_8007F628 + currentOffset + 16);
    values[4] = (((tick - currentValue) * blend) / 10000) + currentValue;
    tick = *(s32 *)((s32)&D_8007F628 + nextOffset + 20);
    nextOffset = *(s32 *)((s32)&D_8007F628 + currentOffset + 20);
    values[5] = (((tick - nextOffset) * blend) / 10000) + nextOffset;
    func_80046248(values);

    {
        register s32 tailCurrent asm("$3");
        register s32 tailTick asm("$2");
        register s32 tailNext asm("$4");

        tailCurrent = D_8007F61C;
        tailTick = D_8007F624;
        tailCurrent = *(s32 *)((s32)&D_8007F628 + (tailCurrent << 5) + 0x18);
        tailTick++;
        tailTick = (tailTick < tailCurrent) ? tailTick : 0;
        D_8007F624 = tailTick;
        if (tailTick == 0) {
            tailNext = D_8007F620;
            tailCurrent = tailNext + 1;
            tailTick = *(s32 *)((s32)&D_8007F628 + (tailCurrent << 5) + 0x18);
            D_8007F61C = tailNext;
            tailTick = ~tailTick;
            tailTick = (tailTick != 0);
            tailTick = -tailTick;
            tailCurrent &= tailTick;
            D_8007F620 = tailCurrent;
        }
    }
}
