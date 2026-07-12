#include "common.h"
#include "game/audio.h"

extern u8 D_8009DF20[];
extern u32 D_8009E59C[];
extern u16 D_8019CA68;
extern u32 D_801E4110;
extern volatile u32 D_801E416C;
extern u8 D_801E4BD7;
extern u8 D_801E4BDC;
extern u16 D_801E4BE8;
extern s16 D_801E4BEA;
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
        asm volatile(
            "lui %0,%%hi(D_801E4BEC)\n\t"
            "addiu %0,%0,%%lo(D_801E4BEC)"
            : "=r"(voiceOffsetPtr)
            : "r"(flags)
            : "memory");
        periodIndex = D_801E4BD7;
        tableIndex = D_801E4BDC;
        tableBase = D_801E416C;
        periodIndex <<= 4;
        periodIndex += tableIndex;
        periodIndex <<= 5;
        periodIndex += tableBase;
        asm volatile("lh %0,0(%1)" : "=r"(tableIndex) : "r"(voiceOffsetPtr) : "memory");
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
        asm volatile("lh %0,0(%1)" : "=r"(tableIndex) : "r"(voiceOffsetPtr) : "memory");
        periodIndex = *(u16 *)(periodIndex + 0x12);
        tableBase = D_8019CA68;
        tableIndex <<= 1;
        periodIndex += tableBase;
        *(u16 *)&D_8009DF20[0xA + tableIndex] = periodIndex;
    }
    g_SndVoiceFlags[D_801E4BEA] |= 0x30;

    (void)stackPad;
}
