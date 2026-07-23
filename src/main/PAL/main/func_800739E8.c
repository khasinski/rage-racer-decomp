#include "common.h"

extern u8 D_801E42F8;
extern u8 D_801E4BDF;
extern u8 D_8009E0B8[];
extern u8 D_8009E0BA[];
extern u8 D_8009E0BE[];
extern u8 D_8009E0D0[];
extern u8 D_8009E0D3[];

u32 func_8007A1F8(s32 on_off, u32 voice_bit);

u8 func_800739E8(s32 unused) {
    u8 candidates;
    u16 bestAge;
    u16 bestPitch;
    u8 voice;
    u8 selected;
    u8 candidate;
    u16 threshold;
    s32 offset;
    s32 count;
    u8 *base;

    selected = 99;
    bestPitch = -1;
    candidates = 0;
    bestAge = 0;
    threshold = D_801E4BDF;
    candidate = 99;
    for (voice = 0; voice < D_801E42F8; voice++) {
        offset = (u8)voice * 52;
        if (D_8009E0D3[offset] == 0 &&
            *(u16 *)&D_8009E0BE[offset] == 0) {
            selected = voice;
            goto found;
        }
        offset = (u8)voice * 52;
        if (*(s16 *)&D_8009E0D0[offset] < threshold) {
            threshold = *(s16 *)&D_8009E0D0[offset];
            candidate = voice;
            bestPitch = *(u16 *)&D_8009E0BE[offset];
            bestAge = *(u16 *)&D_8009E0BA[offset];
            candidates = 1;
        } else if (*(s16 *)&D_8009E0D0[offset] == threshold) {
            candidates += 1;
            if (*(u16 *)&D_8009E0BE[offset] < bestPitch) {
                bestAge = *(u16 *)&D_8009E0BA[offset];
                bestPitch = *(u16 *)&D_8009E0BE[offset];
                candidate = voice;
            } else if (*(u16 *)&D_8009E0BE[offset] == bestPitch) {
                if (bestAge < *(s16 *)&D_8009E0BA[offset]) {
                    bestAge = *(s16 *)&D_8009E0BA[offset];
                    candidate = voice;
                }
            }
        }
    }

found:
    if ((u8)selected == 99) {
        if (candidates == 0) {
            selected = D_801E42F8;
        } else {
            selected = candidate;
        }
    }
    count = D_801E42F8;
    if ((u32)(u8)selected < (u32)count) {
        voice = 0;
        if (count != 0) {
            base = D_8009E0B8;
            do {
                u32 ageIndex;
                s32 ageOffset;
                u32 age;

                ageIndex = (u8)voice;
                ageOffset = (ageIndex << 1) + ageIndex;
                ageOffset = (ageOffset << 2) + ageIndex;
                ageOffset <<= 2;
                voice++;
                age = *(u16 *)&D_8009E0BA[ageOffset];
                age++;
                *(u16 *)((s32)ageOffset + (s32)base + 2) = age;
            } while ((u32)(u8)voice < (u32)count);
        }
        {
            u32 selectedIndex;
            s32 selectedOffset;

            selectedIndex = (u8)selected;
            selectedOffset = selectedIndex * 52;
            *(u16 *)&D_8009E0BA[selectedOffset] = 0;
            *(u16 *)&D_8009E0D0[selectedOffset] = D_801E4BDF;
            if (D_8009E0D3[selectedOffset] == 2) {
                func_8007A1F8(0, 0xFFFFFF);
            }
        }
    }
    (void)unused;
    return (u8)selected;
}
