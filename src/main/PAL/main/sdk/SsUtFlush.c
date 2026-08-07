#include "common.h"
#include "psyq/spu.h"
#include "psyq/snd.h"
#include "game/audio.h"

extern volatile u_short *g_SndSpuRegs;
extern u_char g_SndVoiceRegs[];
extern u_char g_SndVoiceRegsPitch[];
extern u_char g_SndVoiceState[];
extern u_char g_SndVoiceStateAutoVol[];
/* The four pending key registers, flushed below into the SPU as
 * spu[0xC4]/[0xC5] = KON 0x1F801D88 and spu[0xC6]/[0xC7] = KOFF 0x1F801D8C.
 * So D_8009E670/74 are key-ON and D_801F2A08/0C key-OFF, not the other way
 * round. All four MUST keep the raw D_ spelling: %hi/%lo pairs in inline asm
 * elsewhere stringify them. */
extern volatile u_short D_801F2A08;
extern volatile u_short D_801F2A0C;
extern volatile u_short D_8009E670;
extern volatile u_short D_8009E674;
extern volatile u_short g_SndReverbOnLow;
extern volatile u_short g_SndReverbOnHigh;
extern u_char g_SndVoiceCount;
extern volatile u_char g_SndReservedVoiceCount;

void SpuVmAutoVolTick(long voice);

void SsUtFlush(void) {
    volatile long stack[4];
    register long i asm("$16");
    register long voiceOffset asm("$17");
    register long voiceValue asm("$2");
    long voiceIndex;
    long two;
    long oneSaved;
    u_long commonMask;
    u_short keyOffLow;
    u_short keyOffHigh;
    u_short keyOnLow;
    u_short keyOnHigh;
    u_short reverbOnLow;
    u_short reverbOnHigh;

    {
        long count;
        long historyWork;
        u_long *historyBase;
        u_long *history;
        u_long one;
        long voiceCount;
        u_short *pitchPtr;
        volatile u_short *spu;

        i = 0;
        historyWork = g_SndVoiceSilenceIndex;
        count = g_SndVoiceCount;
        historyBase = g_SndVoiceSilenceHistory;
        historyWork = (historyWork + 1) & 0xF;
        g_SndVoiceSilenceIndex = historyWork;
        historyWork = (long)&historyBase[historyWork];
        *(u_long *)historyWork = 0;
        if (count > 0) {
        history = (u_long *)historyWork;
        one = 1;
        voiceCount = count;
        pitchPtr = (u_short *)g_SndVoiceStateEnvx;
        spu = g_SndSpuRegs;
        do {
            *pitchPtr = spu[6];
            if (*pitchPtr == 0) {
                *history |= one << i;
            }
            i++;
            pitchPtr = (u_short *)((u_char *)pitchPtr + 0x34);
            spu += 8;
        } while (i < voiceCount);
        }
    }

    if (g_SndReservedVoiceCount == 0) {
        u_long *historyScan;

        commonMask = ~0U;
        i = 0;
        historyScan = g_SndVoiceSilenceHistory;
        do {
            commonMask &= *historyScan++;
            i++;
        } while (i < 15);

        {
            long activeVoices;

            activeVoices = g_SndVoiceCount;
        if (activeVoices > 0) {
            i = 0;
            oneSaved = 1;
            two = 2;
            voiceOffset = 0;
for (;;) {
            if ((commonMask & (oneSaved << i)) != 0) {
                if (g_SndVoiceStateStatus[voiceOffset] == two) {
                    SpuSetNoiseVoice(0, 0xFFFFFF);
                }
                g_SndVoiceStateStatus[voiceOffset] = 0;
            }
            activeVoices = g_SndVoiceCount;
            i++;
            if (i < activeVoices) {
                voiceOffset += 0x34;
                continue;
            }
break;
}
        }
        }
    }

    {
        u_short mask;
        u_short activeMask;

        i = 0;
        voiceIndex = 0;
        mask = D_801F2A08;
        activeMask = D_8009E670;
        mask = ~mask;
        activeMask &= mask;
        mask = D_801F2A0C;
        asm("" : "=r"(mask) : "0"(mask) : "$17");
        voiceOffset = 0;
        D_8009E670 = activeMask;
        activeMask = D_8009E674;
        mask = ~mask;
        activeMask &= mask;
        D_8009E674 = activeMask;
    }
    do {
        if (*(short *)&g_SndVoiceStateAutoVol[voiceOffset] != 0) {
            SpuVmAutoVolTick(voiceIndex >> 16);
        }
        if (*(short *)&g_SndVoiceStateAutoPan[voiceOffset] != 0) {
            SpuVmAutoPanTick(voiceIndex >> 16);
        }
        voiceOffset += 0x34;
        voiceValue = 0x10000;
        voiceIndex += voiceValue;
        i++;
    } while (i < 24);

    {
        register u_char *flagsPtr asm("$5");
        u_char *src0;
        u_char *src2;
        u_char *src8;
        u_char *src10;
        long spuOffset;
        u_short value;
        u_char *srcBase;

        spuOffset = 0;
        flagsPtr = g_SndVoiceFlags;
        srcBase = g_SndVoiceRegs;
        src10 = srcBase + 10;
        src8 = srcBase + 8;
        src2 = srcBase + 2;
        src0 = srcBase;
        do {
        if (*flagsPtr % 2) {
            voiceValue = (long)((u_char *)g_SndSpuRegs + spuOffset);
            value = *(u_short *)src0;
            ((volatile u_short *)voiceValue)[0] = value;
            value = *(u_short *)src2;
            ((volatile u_short *)voiceValue)[1] = value;
        }
        if (*flagsPtr & 4) {
            voiceValue = (long)((u_char *)g_SndSpuRegs + spuOffset);
            value = *(u_short *)&g_SndVoiceRegsPitch[spuOffset];
            ((volatile u_short *)voiceValue)[2] = value;
        }
        if (*flagsPtr & 8) {
            voiceValue = (long)((u_char *)g_SndSpuRegs + spuOffset);
            value = *(u_short *)&g_SndVoiceRegsAddr[spuOffset];
            ((volatile u_short *)voiceValue)[3] = value;
        }
        if (*flagsPtr & 0x10) {
            voiceValue = (long)((u_char *)g_SndSpuRegs + spuOffset);
            value = *(u_short *)src8;
            ((volatile u_short *)voiceValue)[4] = value;
            value = *(u_short *)src10;
            ((volatile u_short *)voiceValue)[5] = value;
        }
        *flagsPtr = 0;
        flagsPtr++;
        src2 += 16;
        src10 += 16;
        src8 += 16;
        spuOffset += 16;
        src0 += 16;
        } while ((long)flagsPtr < (long)g_SndVoiceState);
    }

    {
    volatile u_short *spu = g_SndSpuRegs;
    keyOffLow = D_801F2A08;
    keyOffHigh = D_801F2A0C;
    keyOnLow = D_8009E670;
    keyOnHigh = D_8009E674;
    reverbOnLow = g_SndReverbOnLow;
    reverbOnHigh = g_SndReverbOnHigh;
    D_801F2A08 = 0;
    D_801F2A0C = 0;
    D_8009E670 = 0;
    D_8009E674 = 0;
    spu[0xC6] = keyOffLow;
    spu[0xC7] = keyOffHigh;
    spu[0xC4] = keyOnLow;
    spu[0xC5] = keyOnHigh;
    spu[0xCC] = reverbOnLow;
    spu[0xCD] = reverbOnHigh;
    }

    asm(".globl func_80076060\nfunc_80076060 = SsUtFlush + 0xbc");
    (void)stack;
}
