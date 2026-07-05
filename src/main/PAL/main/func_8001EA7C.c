#include "common.h"

extern volatile u32 D_8009AF5C;
extern volatile u32 D_8009AF60;
extern volatile u32 D_8009AF64;
extern volatile u32 D_8009AF68;
extern volatile u16 D_8019CE94;
extern volatile u16 D_8019CE96;
extern volatile u16 D_801C067C;
extern volatile u16 D_801C067E;
extern volatile u32 D_801E4B34;

void func_8001EA7C(void *arg0, s32 arg1, s32 arg2) {
    volatile u32 *words;
    volatile u16 *halves;
    u32 word0;
    u32 word1;
    u32 word3;
    u32 word4;
    u16 half18;
    u16 half1A;
    u16 half20;
    u32 word28;
    u16 half22;

    words = arg0;
    halves = arg0;
    word0 = D_8009AF5C;
    word1 = D_8009AF60;
    word3 = D_8009AF64;
    word4 = D_8009AF68;
    words[2] = 0;
    words[5] = 0;
    words[0] = word0;
    words[1] = word1;
    words[3] = word3;
    words[4] = word4;
    half18 = D_8019CE94;
    halves[0xC] = half18;
    half1A = D_8019CE96;
    halves[0xD] = half1A;
    half20 = D_801C067C;
    halves[0x10] = half20;
    word28 = D_801E4B34;
    half22 = D_801C067E;
    halves[0x16] = arg1;
    halves[0x17] = arg2;
    words[0xD] = 0;
    words[0xA] = word28;
    halves[0x18] = 0x18;
    ((u16 *)arg0)[0x11] = half22;
}
