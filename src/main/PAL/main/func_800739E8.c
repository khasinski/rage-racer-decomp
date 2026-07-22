#include "common.h"
#include "psyq/spu.h"

extern u8 D_801E42F8;
extern u8 D_801E4BDF;
extern u8 D_8009E0B8[];
extern u8 D_8009E0BA[];
extern u8 D_8009E0BE[];
extern u8 D_8009E0D0[];
extern u8 D_8009E0D3[];

u8 func_800739E8(s32 unused) {
    volatile s32 stack[4];
    register s32 selected asm("$16");
    register u32 bestPitch asm("$11");
    register s32 candidates asm("$10");
    register s32 bestAge asm("$8");
    register s32 candidate asm("$9");
    register u32 threshold asm("$12");
    register u32 i asm("$7");
    register s32 currentPriority asm("$6");
    register s32 priorityCopy asm("$5");
    register u32 currentPitch asm("$4");
    register s32 offset asm("$3");
    register u32 voice asm("$2");
    register s32 initialCount asm("$2");
    s32 count;

    selected = 99;
    bestPitch = 0xFFFF;
    candidates = 0;
    bestAge = 0;
    candidate = 99;
    initialCount = D_801E42F8;
    threshold = D_801E4BDF;
    i = 0;

    if (initialCount != 0) {
        do {
            voice = i & 0xFF;
            offset = (voice << 1) + voice;
            offset = (offset << 2) + voice;
            offset <<= 2;

            if (D_8009E0D3[offset] != 0) {
                goto occupied;
            }
            if (*(u16 *)&D_8009E0BE[offset] == 0) {
                selected = i;
                goto found;
            }

occupied:
            voice = i & 0xFF;
            offset = (voice << 1) + voice;
            offset = (offset << 2) + voice;
            offset <<= 2;
            currentPriority = *(s16 *)&D_8009E0D0[offset];

            if (currentPriority < (s32)(threshold & 0xFFFF)) {
                priorityCopy = currentPriority;
                asm("" : "=r"(priorityCopy) : "0"(priorityCopy));
                threshold = priorityCopy;
                candidate = i;
                bestPitch = *(u16 *)&D_8009E0BE[offset];
                bestAge = *(u16 *)&D_8009E0BA[offset];
                candidates = 1;
            } else if (currentPriority == (s32)(threshold & 0xFFFF)) {
                currentPitch = *(u16 *)&D_8009E0BE[offset];
                candidates++;
                if (currentPitch < (bestPitch & 0xFFFF)) {
                    bestAge = *(u16 *)&D_8009E0BA[offset];
                    bestPitch = currentPitch;
                    candidate = i;
                } else if (currentPitch == (bestPitch & 0xFFFF)) {
                    register s32 ageRaw asm("$2");
                    register s32 age asm("$3");

                    ageRaw = *(s16 *)&D_8009E0BA[offset];
                    age = ageRaw;
                    asm("" : "=r"(age) : "0"(age));
                    if (bestAge < ageRaw) {
                        bestAge = age;
                        candidate = i;
                    }
                }
            }

            i++;
        } while ((i & 0xFF) < D_801E42F8);
    }

found:
    if ((selected & 0xFF) == 99) {
        register u32 candidateFlag asm("$2");

        candidateFlag = candidates & 0xFF;
        selected = candidate;
        if (candidateFlag == 0) {
            selected = D_801E42F8;
        }
    }

    count = D_801E42F8;
    if ((u32)(selected & 0xFF) < (u32)count) {
        i = 0;
        if (count != 0) {
            register u8 *agesBase asm("$5");

            agesBase = D_8009E0B8;
            do {
                register u32 ageIndex asm("$3");
                register s32 ageOffset asm("$2");
                register u32 age asm("$3");

                ageIndex = i & 0xFF;
                ageOffset = (ageIndex << 1) + ageIndex;
                ageOffset = (ageOffset << 2) + ageIndex;
                ageOffset <<= 2;
                i++;
                age = *(u16 *)&D_8009E0BA[ageOffset];
                *(u16 *)&agesBase[ageOffset + 2] = age + 1;
            } while ((i & 0xFF) < (u32)count);
        }

        {
            register u32 selectedIndex asm("$3");
            register s32 selectedOffset asm("$2");

            selectedIndex = selected & 0xFF;
            selectedOffset = (selectedIndex << 1) + selectedIndex;
            selectedOffset = (selectedOffset << 2) + selectedIndex;
            selectedOffset <<= 2;
            *(u16 *)&D_8009E0BA[selectedOffset] = 0;
            *(u16 *)&D_8009E0D0[selectedOffset] = D_801E4BDF;
            if (D_8009E0D3[selectedOffset] == 2) {
                register s32 noiseMode asm("$4");
                register u32 noiseMask asm("$5");

                noiseMask = 0xFFFFFF;
                asm("" : "=r"(noiseMask) : "0"(noiseMask));
                noiseMode = 0;
                SpuSetNoiseVoice(noiseMode, noiseMask);
            }
        }
    }

    (void)unused;
    (void)stack;
    return selected & 0xFF;
}
