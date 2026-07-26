#include "common.h"
#include "game/audio.h"

extern u8 D_801E42F8;
extern u8 D_801E4BDF;
extern u8 D_8009E0B8[];
extern u8 D_8009E0BA[];
extern u8 D_8009E0BE[];
extern u8 D_8009E0D0[];
extern u8 D_8009E0D3[];

u32 func_8007A1F8(s32 on_off, u32 voice_bit);

u8 SpuVmAlloc(s32 unused) asm("func_800739E8");
u8 SpuVmAlloc(s32 unused) {
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

extern u8 *D_801E413C;
extern u8 D_801E4BD4;
extern u8 D_801E4BDA;
extern u8 D_801E4BDD;
extern u8 D_801E4BDB;
extern u8 D_801E4BD5;
extern u8 D_801E4BDE;
extern u8 D_801E4BE4;
extern u16 D_801E4BE6;
extern s16 D_801E4BEA;
extern s16 D_801E3FB0;
extern u8 *D_801E79CC[];
extern u8 D_8009DF20[];
extern u8 D_8009E0A0[];
extern u16 D_8009E680;
extern u16 D_8009E684;
extern u16 D_8009E670;
extern u16 D_8009E674;
extern u16 D_801F2A08;
extern u16 D_801F2A0C;

void func_80073C50(s32 arg0, s32 val) {
    u32 a1v, a2v, a3v;
    u16 vidx;
    s32 g, t, sixteen, center, hi;
    u8 *base;
    u32 kx;
    s16 *fp;
    s32 tA, F0;

    tA = D_801E413C[0x18] * 16383;
    a2v = D_801E4BD4 * tA / 16129;
    a3v = a2v * D_801E4BDA * D_801E4BDD / 16129;

    F0 = D_801E4BEA;
    vidx = F0 << 3;

    g = D_801E4BE6;
    center = g & 0xff;
    {
        s32 idx4 = center << 2;
        t = g << 16;
        sixteen = t >> 16;
        hi = (u32)t >> 24;
        base = *(u8 **)((u8 *)D_801E79CC + idx4) + hi * 172;
    }

    a2v = a3v & 0x7FFFFFFF;
    if (sixteen != 0x21) {
        a2v = a3v * *(u16 *)(base + 116) / 127;
        a3v = a3v * *(u16 *)(base + 118) / 127;
    }

    kx = D_801E4BDE;
    if (kx < 64) {
        a1v = a3v * kx / 63;
    } else {
        a2v = a2v * (127 - kx) / 63;
        a1v = a3v;
    }

    kx = D_801E4BDB;
    if (kx < 64) {
        a1v = a1v * kx / 63;
    } else {
        a2v = a2v * (127 - kx) / 63;
    }

    kx = D_801E4BD5;
    if (kx < 64) {
        a1v = a1v * kx / 63;
    } else {
        a2v = a2v * (127 - kx) / 63;
    }

    if (D_801E3FB0 == 1) {
        if (a2v < a1v) {
            a2v = a1v;
        } else {
            a1v = a2v;
        }
    }

    a2v = a2v * a2v / 16383;
    a1v = a1v * a1v / 16383;

    *(u16 *)&D_8009DF20[vidx * 2 + 4] = val;
    *(u16 *)&D_8009DF20[vidx * 2] = a2v;
    *(u16 *)&D_8009DF20[vidx * 2 + 2] = a1v;

    fp = &D_801E4BEA;
    D_8009E0A0[*fp] |= 7;
    *(u16 *)&D_8009E0B8[*fp * 0x34 + 4] = val;
    D_8009E0B8[*fp * 0x34 + 0x1B] = 1;

    {
        s32 f = *fp;
        if (f < 16) {
            a2v = 1 << f;
            a1v = 0;
        } else {
            a2v = 0;
            a1v = 1 << (f - 16);
        }
    }

    if (D_801E4BE4 & 4) {
        D_8009E680 = a2v | D_8009E680;
        D_8009E684 = a1v | D_8009E684;
    } else {
        D_8009E680 &= ~a2v;
        D_8009E684 &= ~a1v;
    }

    D_8009E670 = a2v | D_8009E670;
    D_8009E674 = a1v | D_8009E674;
    D_801F2A08 &= ~D_8009E670;
    D_801F2A0C &= ~D_8009E674;

    (void)arg0;
}

extern u32 D_8009E59C[];
extern u16 D_8019CA68;
extern u32 D_801E4110;
extern volatile u32 D_801E416C;
extern u8 D_801E4BD7;
extern u8 D_801E4BDC;
extern u16 D_801E4BE8;
extern s16 D_801E4BEC;
extern s16 D_801E4BEE;

void func_80074134(void) {
    register s32 i asm("$6");
    register s16 *packedVoicePtr asm("$3");
    register s16 *voicePtr asm("$8");
    register s32 bit asm("$7");
    register s32 voice asm("$4");
    register u32 *mask asm("$5");
    register s32 periodIndex asm("$2");
    s32 tableIndex;
    u8 stackPad[8];

    i = 0;
    packedVoicePtr = &D_801E4BEC;
    voicePtr = packedVoicePtr - 1;
    bit = 1;
    voice = D_801E4BEA;
    mask = D_8009E59C;

    *packedVoicePtr = voice << 3;
    D_801E4BEE = (D_801E4BD7 << 4) + D_801E4BDC;
    *(u16 *)&D_8009DF20[0x19E + (voice * 0x34)] = 0x7FFF;

    do {
        i++;
        *mask &= ~(bit << *voicePtr);
        mask++;
    } while (i < 16);

    {
        register u32 periodRaw asm("$3");

        periodRaw = D_801E4BE8;
        periodIndex = periodRaw & 1;
        if (periodIndex > 0) {
            periodIndex = periodRaw << 16;
        } else {
            periodIndex = periodRaw << 16;
            goto evenPeriod;
        }
    }

    if (1) {
        register s32 voiceOffset asm("$3");

        periodIndex = periodIndex >> 16;
        periodIndex = (periodIndex - 1) / 2;
        periodIndex <<= 4;
        {
            register s32 periodBase asm("$3");

            periodBase = D_801E4110;
            periodIndex += periodBase;
        }
        {
            voiceOffset = D_801E4BEC;
        }
        periodIndex = *(u16 *)(periodIndex + 0xC);
        voiceOffset <<= 1;
        *(u16 *)&D_8009DF20[6 + voiceOffset] = periodIndex;
    } else {
evenPeriod:
        {
            register s32 voiceOffset asm("$3");

            periodIndex = periodIndex >> 16;
            periodIndex = (periodIndex - 1) / 2;
            periodIndex <<= 4;
            {
                register s32 periodBase asm("$3");

                periodBase = D_801E4110;
                periodIndex += periodBase;
            }
            voiceOffset = D_801E4BEC;
            periodIndex = *(u16 *)(periodIndex + 0xE);
            voiceOffset <<= 1;
            *(u16 *)&D_8009DF20[6 + voiceOffset] = periodIndex;
        }
    }

    {
        register s32 voiceIndex asm("$3");
        register s32 flags asm("$2");
        register s16 *voiceOffsetPtr asm("$5");
        register u32 tableBase asm("$4");

        voiceIndex = D_801E4BEA;
        flags = g_SndVoiceFlags[voiceIndex];
        flags |= 8;
        g_SndVoiceFlags[voiceIndex] = flags;
        asm volatile("" : : "r"(flags) : "memory");
        voiceOffsetPtr = &D_801E4BEC;
        periodIndex = D_801E4BD7;
        tableIndex = D_801E4BDC;
        tableBase = D_801E416C;
        periodIndex <<= 4;
        periodIndex += tableIndex;
        periodIndex <<= 5;
        periodIndex += tableBase;
        tableIndex = *voiceOffsetPtr;
        periodIndex = *(u16 *)(periodIndex + 0x10);
        tableIndex <<= 1;
        *(u16 *)&D_8009DF20[8 + tableIndex] = periodIndex;
        asm volatile("" : : : "memory");

        periodIndex = D_801E4BD7;
        tableIndex = D_801E4BDC;
        periodIndex <<= 4;
        periodIndex += tableIndex;
        periodIndex <<= 5;
        periodIndex += tableBase;
        tableIndex = *voiceOffsetPtr;
        periodIndex = *(u16 *)(periodIndex + 0x12);
        tableBase = D_8019CA68;
        tableIndex <<= 1;
        periodIndex += tableBase;
        *(u16 *)&D_8009DF20[0xA + tableIndex] = periodIndex;
    }
    g_SndVoiceFlags[D_801E4BEA] |= 0x30;

    (void)stackPad;
}
