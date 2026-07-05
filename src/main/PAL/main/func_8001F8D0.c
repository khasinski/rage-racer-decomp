#include "common.h"

extern s16 D_801E4DAC;
extern u8 *D_8009F0A4;
extern u8 *D_8019C7A4;

void func_8001F330(s32 arg0, void *arg1, void *arg2);

void func_8001F8D0(s32 arg0, u8 *arg1, u8 *arg2) {
    register s32 index asm("s0");
    register u8 *primary asm("s1");
    register u8 *secondary asm("s2");
    register s32 next asm("a0");
    register s32 offset asm("v0");
    register u8 *base asm("v1");

    index = arg0;
    primary = arg1;
    secondary = arg2;

    func_8001F330(index, primary, secondary);

    if (D_801E4DAC != 0) {
        if ((index & 1) == 0) {
            index >>= 1;
            offset = index * 3;
        } else {
            asm("");
            index >>= 1;
            next = index + 1;
            if (next == 0x2EE) {
                next = 0;
            }
            offset = next * 3;
        }
        base = (u8 *)((offset << 4) + (s32)D_8009F0A4);
        *(s32 *)(primary + 0x30) = *(s32 *)(base + 0x24);
        *(s32 *)(secondary + 0x30) = *(s32 *)(base + 0x28);
    } else {
        if ((index & 1) == 0) {
            index >>= 1;
            offset = index * 7;
        } else {
            asm("");
            index >>= 1;
            next = index + 1;
            if (next == 0x505) {
                next = 0;
            }
            offset = next * 7;
        }
        offset = (offset << 2) + (s32)D_8019C7A4;
        *(s32 *)(primary + 0x30) = *(s32 *)(offset + 0x14);
    }
}
