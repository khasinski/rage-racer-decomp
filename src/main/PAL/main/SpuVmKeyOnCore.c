#include "common.h"
#include "game/audio.h"

extern u8 D_8009DF20[];
extern u8 D_8009E0BC[];
extern volatile u16 D_8009E670;
extern volatile u16 D_8009E674;
extern volatile u16 D_801F2A08;
extern volatile u16 D_801F2A0C;
extern u8 D_801E42F8;
extern volatile u8 *D_8009A588;

void func_80074818(s32 voice, u16 note, u16 fine, u16 left, u16 right) {
    register u32 rawVoice asm("$7");
    register u32 index asm("$2");
    register u32 lowMask asm("$8");
    register u32 highMask asm("$6");
    register u32 i asm("$5");
    register u32 count asm("$4");
    register u32 voiceIndex asm("$3");
    u8 stackPad[8];

    rawVoice = voice;
    voice = rawVoice & 0xFF;
    index = voice << 4;
    *(u16 *)&D_8009DF20[index + 2] = fine;
    g_SndVoiceFlags[voice] |= 3;
    *(u16 *)&D_8009DF20[index] = note;

    if ((u32)voice < 16) {
        index = 1;
        lowMask = index << voice;
        highMask = 0;
    } else {
        lowMask = 0;
        voiceIndex = voice - 16;
        index = 1;
        highMask = index << voiceIndex;
    }

    voiceIndex = rawVoice & 0xFF;
    index = (((voiceIndex << 1) + voiceIndex) << 2) + voiceIndex;
    index <<= 2;
    count = D_801E42F8;
    i = 0;
    *(u16 *)&D_8009E0BC[index] = 10;

    if (count != 0) {
        do {
            voiceIndex = (u16)i;
            index = (((voiceIndex << 1) + voiceIndex) << 2) + voiceIndex;
            index <<= 2;
            D_8009E0BC[index + 0x17] &= 1;
            i++;
            voiceIndex = D_801E42F8;
        } while ((u16)i < voiceIndex);
    }

    voiceIndex = rawVoice & 0xFF;
    index = (((voiceIndex << 1) + voiceIndex) << 2) + voiceIndex;
    index <<= 2;
    D_8009E0BC[index + 0x17] = 2;
    voiceIndex = D_8009E670;
    count = D_8009E674;
    *(u16 *)&D_8009E0BC[index - 2] = 0;
    index = D_801F2A08;
    __asm__("or %0,%1,%2" : "=r"(voiceIndex) : "r"(lowMask), "r"(voiceIndex));
    __asm__("or %0,%1,%2" : "=r"(count) : "r"(highMask), "r"(count));
    D_8009E670 = voiceIndex;
    __asm__ volatile("" ::: "memory");
    index &= ~voiceIndex;
    D_8009E674 = count;
    __asm__ volatile("" ::: "memory");
    count = ~count;
    D_801F2A08 = index;
    index = D_801F2A0C;
    voiceIndex = (s32)D_8009A588;
    index &= count;
    D_801F2A0C = index;
    *(u16 *)(voiceIndex + 0x194) = lowMask;
    *(u16 *)(voiceIndex + 0x196) = highMask;

    (void)stackPad;
}
