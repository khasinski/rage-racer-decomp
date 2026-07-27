#include <sys/types.h>

#include "common.h"
#include "game/audio.h"

extern u_char D_801E42F8;
extern u_char D_801E4BDF;
extern u_char D_8009E0B8[];
extern u_char D_8009E0BA[];
extern u_char D_8009E0BE[];
extern u_char D_8009E0D0[];
extern u_char D_8009E0D3[];

u_long func_8007A1F8(long on_off, u_long voice_bit);

u_char SpuVmAlloc(long unused) asm("func_800739E8");
u_char SpuVmAlloc(long unused) {
    u_char candidates;
    u_short bestAge;
    u_short bestPitch;
    u_char voice;
    u_char selected;
    u_char candidate;
    u_short threshold;
    long offset;
    long count;
    u_char *base;

    selected = 99;
    bestPitch = -1;
    candidates = 0;
    bestAge = 0;
    threshold = D_801E4BDF;
    candidate = 99;
    for (voice = 0; voice < D_801E42F8; voice++) {
        offset = (u_char)voice * 52;
        if (D_8009E0D3[offset] == 0 &&
            *(u_short *)&D_8009E0BE[offset] == 0) {
            selected = voice;
            goto found;
        }
        offset = (u_char)voice * 52;
        if (*(short *)&D_8009E0D0[offset] < threshold) {
            threshold = *(short *)&D_8009E0D0[offset];
            candidate = voice;
            bestPitch = *(u_short *)&D_8009E0BE[offset];
            bestAge = *(u_short *)&D_8009E0BA[offset];
            candidates = 1;
        } else if (*(short *)&D_8009E0D0[offset] == threshold) {
            candidates += 1;
            if (*(u_short *)&D_8009E0BE[offset] < bestPitch) {
                bestAge = *(u_short *)&D_8009E0BA[offset];
                bestPitch = *(u_short *)&D_8009E0BE[offset];
                candidate = voice;
            } else if (*(u_short *)&D_8009E0BE[offset] == bestPitch) {
                if (bestAge < *(short *)&D_8009E0BA[offset]) {
                    bestAge = *(short *)&D_8009E0BA[offset];
                    candidate = voice;
                }
            }
        }
    }

found:
    if ((u_char)selected == 99) {
        if (candidates == 0) {
            selected = D_801E42F8;
        } else {
            selected = candidate;
        }
    }
    count = D_801E42F8;
    if ((u_long)(u_char)selected < (u_long)count) {
        voice = 0;
        if (count != 0) {
            base = D_8009E0B8;
            do {
                u_long ageIndex;
                long ageOffset;
                u_long age;

                ageIndex = (u_char)voice;
                ageOffset = (ageIndex << 1) + ageIndex;
                ageOffset = (ageOffset << 2) + ageIndex;
                ageOffset <<= 2;
                voice++;
                age = *(u_short *)&D_8009E0BA[ageOffset];
                age++;
                *(u_short *)((long)ageOffset + (long)base + 2) = age;
            } while ((u_long)(u_char)voice < (u_long)count);
        }
        {
            u_long selectedIndex;
            long selectedOffset;

            selectedIndex = (u_char)selected;
            selectedOffset = selectedIndex * 52;
            *(u_short *)&D_8009E0BA[selectedOffset] = 0;
            *(u_short *)&D_8009E0D0[selectedOffset] = D_801E4BDF;
            if (D_8009E0D3[selectedOffset] == 2) {
                func_8007A1F8(0, 0xFFFFFF);
            }
        }
    }
    (void)unused;
    return (u_char)selected;
}

extern u_char *D_801E413C;
extern u_char D_801E4BD4;
extern u_char D_801E4BDA;
extern u_char D_801E4BDD;
extern u_char D_801E4BDB;
extern u_char D_801E4BD5;
extern u_char D_801E4BDE;
extern u_char D_801E4BE4;
extern u_short D_801E4BE6;
extern short D_801E4BEA;
extern short D_801E3FB0;
extern u_char *D_801E79CC[];
extern u_char D_8009DF20[];
extern u_char D_8009E0A0[];
extern u_short D_8009E680;
extern u_short D_8009E684;
extern u_short D_8009E670;
extern u_short D_8009E674;
extern u_short D_801F2A08;
extern u_short D_801F2A0C;

void func_80073C50(long arg0, long val) {
    u_long a1v, a2v, a3v;
    u_short vidx;
    long g, t, sixteen, center, hi;
    u_char *base;
    u_long kx;
    short *fp;
    long tA, F0;

    tA = D_801E413C[0x18] * 16383;
    a2v = D_801E4BD4 * tA / 16129;
    a3v = a2v * D_801E4BDA * D_801E4BDD / 16129;

    F0 = D_801E4BEA;
    vidx = F0 << 3;

    g = D_801E4BE6;
    center = g & 0xff;
    {
        long idx4 = center << 2;
        t = g << 16;
        sixteen = t >> 16;
        hi = (u_long)t >> 24;
        base = *(u_char **)((u_char *)D_801E79CC + idx4) + hi * 172;
    }

    a2v = a3v & 0x7FFFFFFF;
    if (sixteen != 0x21) {
        a2v = a3v * *(u_short *)(base + 116) / 127;
        a3v = a3v * *(u_short *)(base + 118) / 127;
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

    *(u_short *)&D_8009DF20[vidx * 2 + 4] = val;
    *(u_short *)&D_8009DF20[vidx * 2] = a2v;
    *(u_short *)&D_8009DF20[vidx * 2 + 2] = a1v;

    fp = &D_801E4BEA;
    D_8009E0A0[*fp] |= 7;
    *(u_short *)&D_8009E0B8[*fp * 0x34 + 4] = val;
    D_8009E0B8[*fp * 0x34 + 0x1B] = 1;

    {
        long f = *fp;
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

extern u_long D_8009E59C[];
extern u_short D_8019CA68;
extern u_long D_801E4110;
extern volatile u_long D_801E416C;
extern u_char D_801E4BD7;
extern u_char D_801E4BDC;
extern u_short D_801E4BE8;
extern short D_801E4BEC;
extern short D_801E4BEE;

void func_80074134(void) {
    register long i asm("$6");
    register short *packedVoicePtr asm("$3");
    register short *voicePtr asm("$8");
    register long bit asm("$7");
    register long voice asm("$4");
    register u_long *mask asm("$5");
    register long periodIndex asm("$2");
    long tableIndex;
    u_char stackPad[8];

    i = 0;
    packedVoicePtr = &D_801E4BEC;
    voicePtr = packedVoicePtr - 1;
    bit = 1;
    voice = D_801E4BEA;
    mask = D_8009E59C;

    *packedVoicePtr = voice << 3;
    D_801E4BEE = (D_801E4BD7 << 4) + D_801E4BDC;
    *(u_short *)&D_8009DF20[0x19E + (voice * 0x34)] = 0x7FFF;

    do {
        i++;
        *mask &= ~(bit << *voicePtr);
        mask++;
    } while (i < 16);

    {
        register u_long periodRaw asm("$3");

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
        register long voiceOffset asm("$3");

        periodIndex = periodIndex >> 16;
        periodIndex = (periodIndex - 1) / 2;
        periodIndex <<= 4;
        {
            register long periodBase asm("$3");

            periodBase = D_801E4110;
            periodIndex += periodBase;
        }
        {
            voiceOffset = D_801E4BEC;
        }
        periodIndex = *(u_short *)(periodIndex + 0xC);
        voiceOffset <<= 1;
        *(u_short *)&D_8009DF20[6 + voiceOffset] = periodIndex;
    } else {
evenPeriod:
        {
            register long voiceOffset asm("$3");

            periodIndex = periodIndex >> 16;
            periodIndex = (periodIndex - 1) / 2;
            periodIndex <<= 4;
            {
                register long periodBase asm("$3");

                periodBase = D_801E4110;
                periodIndex += periodBase;
            }
            voiceOffset = D_801E4BEC;
            periodIndex = *(u_short *)(periodIndex + 0xE);
            voiceOffset <<= 1;
            *(u_short *)&D_8009DF20[6 + voiceOffset] = periodIndex;
        }
    }

    {
        register long voiceIndex asm("$3");
        register long flags asm("$2");
        register short *voiceOffsetPtr asm("$5");
        register u_long tableBase asm("$4");

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
        periodIndex = *(u_short *)(periodIndex + 0x10);
        tableIndex <<= 1;
        *(u_short *)&D_8009DF20[8 + tableIndex] = periodIndex;
        asm volatile("" : : : "memory");

        periodIndex = D_801E4BD7;
        tableIndex = D_801E4BDC;
        periodIndex <<= 4;
        periodIndex += tableIndex;
        periodIndex <<= 5;
        periodIndex += tableBase;
        tableIndex = *voiceOffsetPtr;
        periodIndex = *(u_short *)(periodIndex + 0x12);
        tableBase = D_8019CA68;
        tableIndex <<= 1;
        periodIndex += tableBase;
        *(u_short *)&D_8009DF20[0xA + tableIndex] = periodIndex;
    }
    g_SndVoiceFlags[D_801E4BEA] |= 0x30;

    (void)stackPad;
}
