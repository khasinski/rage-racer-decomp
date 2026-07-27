#include "common.h"
#include "psyq/spu.h"

extern s32 D_8009E598;
extern u32 D_8009E59C[];
extern volatile u16 *D_8009A588;
extern u8 D_8009DF20[];
extern u8 D_8009DF24[];
extern u8 D_8009DF26[];
extern u8 D_8009E0A0[];
extern u8 D_8009E0B8[];
extern u8 D_8009E0BE[];
extern u8 D_8009E0D3[];
extern u8 D_8009E0D4[];
extern u8 D_8009E0E0[];
extern volatile u16 D_801F2A08;
extern volatile u16 D_801F2A0C;
extern volatile u16 D_8009E670;
extern volatile u16 D_8009E674;
extern volatile u16 D_8009E680;
extern volatile u16 D_8009E684;
extern u8 D_801E42F8;
extern volatile u8 D_801E4D88;

void SpuVmAutoVolTick(s32 voice) asm("func_80074ECC");
void SpuVmAutoPanTick(s32 voice) asm("func_800753CC");

void SsUtFlush(void) asm("func_80075FA4");
void SsUtFlush(void) {
    volatile s32 stack[4];
    register s32 i asm("$16");
    register s32 voiceOffset asm("$17");
    register s32 voiceIndex asm("$18");
    register s32 two asm("$19");
    register s32 oneSaved asm("$20");
    register u32 commonMask asm("$18");
    u16 keyOnLow;
    u16 keyOnHigh;
    u16 keyOffLow;
    u16 keyOffHigh;
    u16 noiseLow;
    u16 noiseHigh;

    {
        register s32 count asm("$4");
        register s32 historyWork asm("$2");
        register u32 *historyBase asm("$3");
        register u32 *history asm("$6");
        register u32 one asm("$8");
        register s32 voiceCount asm("$7");
        register u16 *pitchPtr asm("$4");
        register volatile u16 *spu asm("$5");

        i = 0;
        historyWork = D_8009E598;
        count = D_801E42F8;
        historyBase = D_8009E59C;
        historyWork = (historyWork + 1) & 0xF;
        asm("" : "=r"(historyWork) : "0"(historyWork));
        D_8009E598 = historyWork;
        historyWork = (s32)&historyBase[historyWork];
        *(u32 *)historyWork = 0;
        if (count > 0) {
        history = (u32 *)historyWork;
        one = 1;
        voiceCount = count;
        pitchPtr = (u16 *)D_8009E0BE;
        spu = D_8009A588;
        do {
            *pitchPtr = spu[6];
            if (*pitchPtr == 0) {
                *history |= one << i;
            }
            i++;
            pitchPtr = (u16 *)((u8 *)pitchPtr + 0x34);
            spu += 8;
        } while (i < voiceCount);
        }
    }

    if (D_801E4D88 == 0) {
        register u32 *historyScan asm("$3");

        commonMask = ~0U;
        i = 0;
        historyScan = D_8009E59C;
        do {
            commonMask &= *historyScan++;
            i++;
        } while (i < 15);

        {
            register s32 activeVoices asm("$2");

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
        register u16 mask asm("$2");
        register u16 activeMask asm("$3");

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
        register s32 voiceStep asm("$2");

        if (*(s16 *)&D_8009E0D4[voiceOffset] != 0) {
            SpuVmAutoVolTick(voiceIndex >> 16);
        }
        if (*(s16 *)&D_8009E0E0[voiceOffset] != 0) {
            SpuVmAutoPanTick(voiceIndex >> 16);
        }
        voiceOffset += 0x34;
        voiceStep = 0x10000;
        voiceIndex += voiceStep;
        i++;
    } while (i < 24);

    {
        register u8 *flagsPtr asm("$5");
        register u8 *src0 asm("$6");
        register u8 *src2 asm("$7");
        register u8 *src8 asm("$8");
        register u8 *src10 asm("$9");
        register s32 spuOffset asm("$4");
        register volatile u16 *spu asm("$2");
        register u16 value asm("$3");
        register u8 *srcBase asm("$2");

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
            spu = (volatile u16 *)((u8 *)D_8009A588 + spuOffset);
            value = *(u16 *)src0;
            spu[0] = value;
            value = *(u16 *)src2;
            spu[1] = value;
        }
        if (*flagsPtr & 4) {
            spu = (volatile u16 *)((u8 *)D_8009A588 + spuOffset);
            value = *(u16 *)&D_8009DF24[spuOffset];
            spu[2] = value;
        }
        if (*flagsPtr & 8) {
            spu = (volatile u16 *)((u8 *)D_8009A588 + spuOffset);
            value = *(u16 *)&D_8009DF26[spuOffset];
            spu[3] = value;
        }
        if (*flagsPtr & 0x10) {
            spu = (volatile u16 *)((u8 *)D_8009A588 + spuOffset);
            value = *(u16 *)src8;
            spu[4] = value;
            value = *(u16 *)src10;
            spu[5] = value;
        }
        *flagsPtr = 0;
        flagsPtr++;
        src2 += 16;
        src10 += 16;
        src8 += 16;
        spuOffset += 16;
        src0 += 16;
        } while ((s32)flagsPtr < (s32)D_8009E0B8);
    }

    {
    volatile u16 *spu = D_8009A588;
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
