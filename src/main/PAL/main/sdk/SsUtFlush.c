#include "common.h"
#include "psyq/spu.h"

extern long g_SndVoiceSilenceIndex asm("D_8009E598");
extern u_long g_SndVoiceSilenceHistory[] asm("D_8009E59C");
extern volatile u_short *g_SndSpuRegs asm("D_8009A588");
extern u_char g_SndVoiceRegs[] asm("D_8009DF20");
extern u_char g_SndVoiceRegsPitch[] asm("D_8009DF24");
extern u_char g_SndVoiceRegsAddr[] asm("D_8009DF26");
extern u_char g_SndVoiceFlags[] asm("D_8009E0A0");
extern u_char g_SndVoiceState[] asm("D_8009E0B8");
extern u_char g_SndVoiceStateEnvx[] asm("D_8009E0BE");
extern u_char g_SndVoiceStateStatus[] asm("D_8009E0D3");
extern u_char g_SndVoiceStateAutoVol[] asm("D_8009E0D4");
extern u_char g_SndVoiceStateAutoPan[] asm("D_8009E0E0");
/* The four pending key registers, flushed below into the SPU as
 * spu[0xC4]/[0xC5] = KON 0x1F801D88 and spu[0xC6]/[0xC7] = KOFF 0x1F801D8C.
 * So D_8009E670/74 are key-ON and D_801F2A08/0C key-OFF, not the other way
 * round. All four MUST keep the raw D_ spelling: %hi/%lo pairs in inline asm
 * elsewhere stringify them. */
extern volatile u_short D_801F2A08;
extern volatile u_short D_801F2A0C;
extern volatile u_short D_8009E670;
extern volatile u_short D_8009E674;
extern volatile u_short g_SndReverbOnLow asm("D_8009E680");
extern volatile u_short g_SndReverbOnHigh asm("D_8009E684");
extern u_char D_801E42F8;
extern volatile u_char g_SndReservedVoiceCount asm("D_801E4D88");

void SpuVmAutoVolTick(long voice) asm("func_80074ECC");
void SpuVmAutoPanTick(long voice) asm("func_800753CC");

void SsUtFlush(void) asm("func_80075FA4");
void SsUtFlush(void) {
    volatile long stack[4];
    /* These pins are load-bearing: removing any one changes .text. */
    register long i asm("$16");
    register long voiceOffset asm("$17");
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
        count = D_801E42F8;
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

            activeVoices = D_801E42F8;
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
            activeVoices = D_801E42F8;
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
        /* This barrier is load-bearing: removing it changes .text. */
        asm("" : "=r"(mask) : "0"(mask) : "$17");
        voiceOffset = 0;
        D_8009E670 = activeMask;
        activeMask = D_8009E674;
        mask = ~mask;
        activeMask &= mask;
        D_8009E674 = activeMask;
    }
    do {
        /* This pin is load-bearing: removing it changes .text. */
        register long voiceStep asm("$2");

        if (*(short *)&g_SndVoiceStateAutoVol[voiceOffset] != 0) {
            SpuVmAutoVolTick(voiceIndex >> 16);
        }
        if (*(short *)&g_SndVoiceStateAutoPan[voiceOffset] != 0) {
            SpuVmAutoPanTick(voiceIndex >> 16);
        }
        voiceOffset += 0x34;
        voiceStep = 0x10000;
        voiceIndex += voiceStep;
        i++;
    } while (i < 24);

    {
        /* This pin is load-bearing: removing it changes .text. */
        register u_char *flagsPtr asm("$5");
        u_char *src0;
        u_char *src2;
        u_char *src8;
        u_char *src10;
        long spuOffset;
        /* This pin is load-bearing: removing it changes .text. */
        register volatile u_short *spu asm("$2");
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
        if (*flagsPtr & 1) {
            spu = (volatile u_short *)((u_char *)g_SndSpuRegs + spuOffset);
            value = *(u_short *)src0;
            spu[0] = value;
            value = *(u_short *)src2;
            spu[1] = value;
        }
        if (*flagsPtr & 4) {
            spu = (volatile u_short *)((u_char *)g_SndSpuRegs + spuOffset);
            value = *(u_short *)&g_SndVoiceRegsPitch[spuOffset];
            spu[2] = value;
        }
        if (*flagsPtr & 8) {
            spu = (volatile u_short *)((u_char *)g_SndSpuRegs + spuOffset);
            value = *(u_short *)&g_SndVoiceRegsAddr[spuOffset];
            spu[3] = value;
        }
        if (*flagsPtr & 0x10) {
            spu = (volatile u_short *)((u_char *)g_SndSpuRegs + spuOffset);
            value = *(u_short *)src8;
            spu[4] = value;
            value = *(u_short *)src10;
            spu[5] = value;
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
