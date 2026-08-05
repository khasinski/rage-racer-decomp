#include <sys/types.h>

#include "common.h"
#include "game/audio.h"
#include "psyq/spu.h"
#include "psyq/snd.h"

extern u_char g_SndVoiceCount;
extern u_char g_SndVoiceState[];
extern u_char D_8009E0D0[];

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
    threshold = g_SndCurrentPriority;
    candidate = 99;
    for (voice = 0; voice < g_SndVoiceCount; voice++) {
        offset = (u_char)voice * 52;
        if (g_SndVoiceStateStatus[offset] == 0 &&
            *(u_short *)&g_SndVoiceStateEnvx[offset] == 0) {
            selected = voice;
            break;
        }
        offset = (u_char)voice * 52;
        if (*(short *)&D_8009E0D0[offset] < threshold) {
            threshold = *(short *)&D_8009E0D0[offset];
            candidate = voice;
            bestPitch = *(u_short *)&g_SndVoiceStateEnvx[offset];
            bestAge = *(u_short *)&g_SndVoiceStateAge[offset];
            candidates = 1;
        } else if (*(short *)&D_8009E0D0[offset] == threshold) {
            candidates += 1;
            if (*(u_short *)&g_SndVoiceStateEnvx[offset] < bestPitch) {
                bestAge = *(u_short *)&g_SndVoiceStateAge[offset];
                bestPitch = *(u_short *)&g_SndVoiceStateEnvx[offset];
                candidate = voice;
            } else if (*(u_short *)&g_SndVoiceStateEnvx[offset] == bestPitch) {
                if (bestAge < *(short *)&g_SndVoiceStateAge[offset]) {
                    bestAge = *(short *)&g_SndVoiceStateAge[offset];
                    candidate = voice;
                }
            }
        }
    }

    if ((u_char)selected == 99) {
        if (candidates == 0) {
            selected = g_SndVoiceCount;
        } else {
            selected = candidate;
        }
    }
    count = g_SndVoiceCount;
    if ((u_long)(u_char)selected < (u_long)count) {
        voice = 0;
        if (count != 0) {
            base = g_SndVoiceState;
            do {
                u_long ageIndex;
                long ageOffset;
                u_long age;

                ageIndex = (u_char)voice;
                ageOffset = ageIndex * 3;
                ageOffset = (ageOffset * 4) + ageIndex;
                ageOffset <<= 2;
                voice++;
                age = *(u_short *)&g_SndVoiceStateAge[ageOffset];
                age++;
                *(u_short *)((long)ageOffset + (long)base + 2) = age;
            } while ((u_long)(u_char)voice < (u_long)count);
        }
        {
            u_long selectedIndex;
            long selectedOffset;

            selectedIndex = (u_char)selected;
            selectedOffset = selectedIndex * 52;
            *(u_short *)&g_SndVoiceStateAge[selectedOffset] = 0;
            *(u_short *)&D_8009E0D0[selectedOffset] = g_SndCurrentPriority;
            if (g_SndVoiceStateStatus[selectedOffset] == 2) {
                SpuSetNoiseVoice(0, 0xFFFFFF);
            }
        }
    }
    (void)unused;
    return (u_char)selected;
}

extern u_char *g_SndCurrentVabHeader;
extern u_char D_801E4BD4;
extern u_char D_801E4BDA;
extern u_char D_801E4BDD;
extern u_char D_801E4BDB;
extern u_char D_801E4BD5;
extern u_char D_801E4BDE;
extern u_char D_801E4BE4;
extern u_short g_SndCurrentSeqSep;
extern short g_SndCurrentVoice;
extern short g_SndMonoMode;
extern u_char *g_SndSeqTable[];
extern u_char g_SndVoiceRegs[];
/* Kept raw: this is game/audio.h's g_SndVoiceFlags, but that header is included
 * here and declares it volatile, and gcc 2.6.3 rejects a requalified array
 * redeclaration outright. */
extern u_char D_8009E0A0[] asm("g_SndVoiceFlags");
extern u_short g_SndReverbOnLow;
extern u_short g_SndReverbOnHigh;
extern u_short D_8009E670;
extern u_short D_8009E674;
extern u_short D_801F2A08;
extern u_short D_801F2A0C;

void SpuVmScaleVabVolume(long arg0, long val) {
    u_long a1v, a2v, a3v;
    u_short vidx;
    long g, t, sixteen, center, hi;
    u_char *base;
    u_long kx;
    short *fp;
    long tA, F0;

    tA = g_SndCurrentVabHeader[0x18] * 16383;
    a2v = D_801E4BD4 * tA / 16129;
    a3v = a2v * D_801E4BDA * D_801E4BDD / 16129;

    F0 = g_SndCurrentVoice;
    vidx = F0 * 8;

    g = g_SndCurrentSeqSep;
    center = (u8)g;
    {
        long idx4 = center * 4;
        t = g << 16;
        sixteen = t >> 16;
        hi = (u_long)t / 16777216;
        base = *(u_char **)((u_char *)g_SndSeqTable + idx4) + hi * 172;
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

    if (g_SndMonoMode == 1) {
        if (a2v < a1v) {
            a2v = a1v;
        } else {
            a1v = a2v;
        }
    }

    a2v = a2v * a2v / 16383;
    a1v = a1v * a1v / 16383;

    *(u_short *)&g_SndVoiceRegs[vidx * 2 + 4] = val;
    *(u_short *)&g_SndVoiceRegs[vidx * 2] = a2v;
    *(u_short *)&g_SndVoiceRegs[vidx * 2 + 2] = a1v;

    fp = &g_SndCurrentVoice;
    D_8009E0A0[*fp] |= 7;
    *(u_short *)&g_SndVoiceState[*fp * 0x34 + 4] = val;
    g_SndVoiceState[*fp * 0x34 + 0x1B] = 1;

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
        g_SndReverbOnLow = a2v | g_SndReverbOnLow;
        g_SndReverbOnHigh = a1v | g_SndReverbOnHigh;
    } else {
        g_SndReverbOnLow &= ~a2v;
        g_SndReverbOnHigh &= ~a1v;
    }

    D_8009E670 = a2v | D_8009E670;
    D_8009E674 = a1v | D_8009E674;
    D_801F2A08 &= ~D_8009E670;
    D_801F2A0C &= ~D_8009E674;

    (void)arg0;
}

extern u_short g_SndDamper;
extern u_long g_SndCurrentProgTable;
extern volatile u_long g_SndCurrentToneTable;
extern u_short D_801E4BE8;
extern short D_801E4BEC;
extern short D_801E4BEE;

void SpuVmRebuildVoiceTable(void) {
    long i;
    short *packedVoicePtr;
    short *voicePtr;
    long bit;
    long voice;
    u_long *mask;
    long periodIndex;
    long tableIndex;
    u_char stackPad[8];

    i = 0;
    packedVoicePtr = &D_801E4BEC;
    voicePtr = packedVoicePtr - 1;
    bit = 1;
    voice = g_SndCurrentVoice;
    mask = g_SndVoiceSilenceHistory;

    *packedVoicePtr = voice * 8;
    D_801E4BEE = (g_SndCurrentProgActual * 16) + g_SndCurrentTone;
    *(u_short *)&g_SndVoiceRegs[0x19E + (voice * 0x34)] = 0x7FFF;

    do {
        i++;
        *mask &= ~(bit << *voicePtr);
        mask++;
    } while (i < 16);

    {
        u_long periodRaw;

        periodRaw = D_801E4BE8;
        periodIndex = periodRaw % 2;
        if (periodIndex > 0) {
            periodIndex = periodRaw << 16;
        {
        register long voiceOffset asm("$3");

        periodIndex = periodIndex >> 16;
        periodIndex = (periodIndex - 1) / 2;
        periodIndex <<= 4;
        {
            register long periodBase asm("$3");

            periodBase = g_SndCurrentProgTable;
            periodIndex += periodBase;
        }
        {
            voiceOffset = D_801E4BEC;
        }
        periodIndex = *(u_short *)(periodIndex + 0xC);
        voiceOffset <<= 1;
        *(u_short *)&g_SndVoiceRegs[6 + voiceOffset] = periodIndex;
        }
    } else {
        periodIndex = periodRaw << 16;
        {
            register long voiceOffset asm("$3");

            periodIndex = periodIndex >> 16;
            periodIndex = (periodIndex - 1) / 2;
            periodIndex <<= 4;
            {
                register long periodBase asm("$3");

                periodBase = g_SndCurrentProgTable;
                periodIndex += periodBase;
            }
            voiceOffset = D_801E4BEC;
            periodIndex = *(u_short *)(periodIndex + 0xE);
            voiceOffset <<= 1;
            *(u_short *)&g_SndVoiceRegs[6 + voiceOffset] = periodIndex;
        }
    }
    }

    {
        long voiceIndex;
        long flags;
        register short *voiceOffsetPtr asm("$5");
        u_long tableBase;

        voiceIndex = g_SndCurrentVoice;
        flags = D_8009E0A0[voiceIndex];
        flags |= 8;
        D_8009E0A0[voiceIndex] = flags;
        asm volatile("" : : "r"(flags) : "memory");
        voiceOffsetPtr = &D_801E4BEC;
        periodIndex = g_SndCurrentProgActual;
        tableIndex = g_SndCurrentTone;
        tableBase = g_SndCurrentToneTable;
        periodIndex <<= 4;
        periodIndex += tableIndex;
        periodIndex <<= 5;
        periodIndex += tableBase;
        tableIndex = *voiceOffsetPtr;
        periodIndex = *(u_short *)(periodIndex + 0x10);
        tableIndex <<= 1;
        *(u_short *)&g_SndVoiceRegs[8 + tableIndex] = periodIndex;
        asm volatile("" : : : "memory");

        periodIndex = g_SndCurrentProgActual;
        tableIndex = g_SndCurrentTone;
        periodIndex <<= 4;
        periodIndex += tableIndex;
        periodIndex <<= 5;
        periodIndex += tableBase;
        tableIndex = *voiceOffsetPtr;
        periodIndex = *(u_short *)(periodIndex + 0x12);
        tableBase = g_SndDamper;
        tableIndex <<= 1;
        periodIndex += tableBase;
        *(u_short *)&g_SndVoiceRegs[0xA + tableIndex] = periodIndex;
    }
    D_8009E0A0[g_SndCurrentVoice] |= 0x30;

    (void)stackPad;
}
