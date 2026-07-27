#include "common.h"
#include "psyq/spu.h"

extern long D_8009E598;
extern u_long D_8009E59C[];
extern volatile u_short *D_8009A588;
extern u_char D_8009DF20[];
extern u_char D_8009DF24[];
extern u_char D_8009DF26[];
extern u_char D_8009E0A0[];
extern u_char D_8009E0B8[];
extern u_char D_8009E0BE[];
extern u_char D_8009E0D3[];
extern u_char D_8009E0D4[];
extern u_char D_8009E0E0[];
extern volatile u_short D_801F2A08;
extern volatile u_short D_801F2A0C;
extern volatile u_short D_8009E670;
extern volatile u_short D_8009E674;
extern volatile u_short D_8009E680;
extern volatile u_short D_8009E684;
extern u_char D_801E42F8;
extern volatile u_char D_801E4D88;

void SpuVmAutoVolTick(long voice) asm("func_80074ECC");
void SpuVmAutoPanTick(long voice) asm("func_800753CC");

void SsUtFlush(void) asm("func_80075FA4");
void SsUtFlush(void) {
    volatile long stack[4];
    register long i asm("$16");
    register long voiceOffset asm("$17");
    register long voiceIndex asm("$18");
    register long two asm("$19");
    register long oneSaved asm("$20");
    register u_long commonMask asm("$18");
    u_short keyOnLow;
    u_short keyOnHigh;
    u_short keyOffLow;
    u_short keyOffHigh;
    u_short noiseLow;
    u_short noiseHigh;

    {
        register long count asm("$4");
        register long historyWork asm("$2");
        register u_long *historyBase asm("$3");
        register u_long *history asm("$6");
        register u_long one asm("$8");
        register long voiceCount asm("$7");
        register u_short *pitchPtr asm("$4");
        register volatile u_short *spu asm("$5");

        i = 0;
        historyWork = D_8009E598;
        count = D_801E42F8;
        historyBase = D_8009E59C;
        historyWork = (historyWork + 1) & 0xF;
        asm("" : "=r"(historyWork) : "0"(historyWork));
        D_8009E598 = historyWork;
        historyWork = (long)&historyBase[historyWork];
        *(u_long *)historyWork = 0;
        if (count > 0) {
        history = (u_long *)historyWork;
        one = 1;
        voiceCount = count;
        pitchPtr = (u_short *)D_8009E0BE;
        spu = D_8009A588;
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

    if (D_801E4D88 == 0) {
        register u_long *historyScan asm("$3");

        commonMask = ~0U;
        i = 0;
        historyScan = D_8009E59C;
        do {
            commonMask &= *historyScan++;
            i++;
        } while (i < 15);

        {
            register long activeVoices asm("$2");

            activeVoices = D_801E42F8;
            asm("" : "=r"(activeVoices) : "0"(activeVoices));
        if (activeVoices > 0) {
            i = 0;
            oneSaved = 1;
            two = 2;
            voiceOffset = 0;
noiseLoop:
            if ((commonMask & (oneSaved << i)) != 0) {
                if (D_8009E0D3[voiceOffset] == two) {
                    SpuSetNoiseVoice(0, 0xFFFFFF);
                }
                D_8009E0D3[voiceOffset] = 0;
            }
            activeVoices = D_801E42F8;
            asm("" : "=r"(activeVoices) : "0"(activeVoices), "r"(i));
            i++;
            if (i < activeVoices) {
                voiceOffset += 0x34;
                goto noiseLoop;
            }
        }
        }
    }

    {
        register u_short mask asm("$2");
        register u_short activeMask asm("$3");

        i = 0;
        voiceIndex = 0;
        mask = D_801F2A08;
        activeMask = D_8009E670;
        mask = ~mask;
        activeMask &= mask;
        asm("" : "=r"(activeMask) : "0"(activeMask));
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
        register long voiceStep asm("$2");

        if (*(short *)&D_8009E0D4[voiceOffset] != 0) {
            SpuVmAutoVolTick(voiceIndex >> 16);
        }
        if (*(short *)&D_8009E0E0[voiceOffset] != 0) {
            SpuVmAutoPanTick(voiceIndex >> 16);
        }
        voiceOffset += 0x34;
        voiceStep = 0x10000;
        voiceIndex += voiceStep;
        i++;
    } while (i < 24);

    {
        register u_char *flagsPtr asm("$5");
        register u_char *src0 asm("$6");
        register u_char *src2 asm("$7");
        register u_char *src8 asm("$8");
        register u_char *src10 asm("$9");
        register long spuOffset asm("$4");
        register volatile u_short *spu asm("$2");
        register u_short value asm("$3");
        register u_char *srcBase asm("$2");

        spuOffset = 0;
        asm("" : : "r"(oneSaved), "r"(two), "r"(spuOffset));
        flagsPtr = D_8009E0A0;
        srcBase = D_8009DF20;
        src10 = srcBase + 10;
        src8 = srcBase + 8;
        src2 = srcBase + 2;
        src0 = srcBase;
        do {
        if (*flagsPtr & 1) {
            spu = (volatile u_short *)((u_char *)D_8009A588 + spuOffset);
            value = *(u_short *)src0;
            spu[0] = value;
            value = *(u_short *)src2;
            spu[1] = value;
        }
        if (*flagsPtr & 4) {
            spu = (volatile u_short *)((u_char *)D_8009A588 + spuOffset);
            value = *(u_short *)&D_8009DF24[spuOffset];
            spu[2] = value;
        }
        if (*flagsPtr & 8) {
            spu = (volatile u_short *)((u_char *)D_8009A588 + spuOffset);
            value = *(u_short *)&D_8009DF26[spuOffset];
            spu[3] = value;
        }
        if (*flagsPtr & 0x10) {
            spu = (volatile u_short *)((u_char *)D_8009A588 + spuOffset);
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
        } while ((long)flagsPtr < (long)D_8009E0B8);
    }

    {
    volatile u_short *spu = D_8009A588;
    keyOnLow = D_801F2A08;
    keyOnHigh = D_801F2A0C;
    keyOffLow = D_8009E670;
    keyOffHigh = D_8009E674;
    noiseLow = D_8009E680;
    noiseHigh = D_8009E684;
    D_801F2A08 = 0;
    D_801F2A0C = 0;
    D_8009E670 = 0;
    D_8009E674 = 0;
    spu[0xC6] = keyOnLow;
    spu[0xC7] = keyOnHigh;
    spu[0xC4] = keyOffLow;
    spu[0xC5] = keyOffHigh;
    spu[0xCC] = noiseLow;
    spu[0xCD] = noiseHigh;
    }

    asm(".globl func_80076060\nfunc_80076060 = SsUtFlush + 0xbc");
    (void)stack;
}
