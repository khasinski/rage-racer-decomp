#include "common.h"
#include "game/audio.h"

extern s32 D_801E6CBC;
extern s32 D_801E6C9C;
extern s32 D_801E6CA4;
extern s32 D_801E6CDC;
extern s32 D_801E6CE0;
extern s32 D_801E6CE4;
extern s32 D_801E6CE8;
extern s32 D_801E6CEC;
extern s32 D_801E6CF0;
extern s32 D_801E6CF4;
extern s32 D_801E6CF8;
extern u8 D_801E6D00[];
extern u8 D_801E6D04[];
extern u8 D_801E6D08[];
extern u8 D_801E6D10[];
extern u8 D_801E6D30[];
extern u8 D_801E6D34[];
extern u8 D_801E6D38[];
extern u8 D_801E6D3C[];
extern u8 D_801E6D40[];

void GameResetSoundState(void) {
    {
        register s32 i asm("$4");
        register s32 *ptr asm("$2");

        i = 5;
        ptr = &D_801E6CDC;
        for (; i >= 0; i--) {
            *ptr-- = 0;
        }
    }

    {
        register s32 i asm("$4");
        register s32 neg asm("$6");
        register s32 *ptr asm("$5");
        register s32 offset asm("$3");

        i = 0;
        neg = -1;
        ptr = &D_801E6C9C;
        offset = 0;
        for (; i < 2; i++) {
            *(s32 *)(D_801E6D08 + offset) = neg;
            *(s32 *)(D_801E6D00 + offset) = neg;
            *(s32 *)(D_801E6D04 + offset) = neg;
            ptr[0x78 / 4] = 0;
            ptr = (s32 *)((u8 *)ptr + 0x18);
            *(s32 *)(D_801E6D10 + offset) = 0;
            offset += 0x18;
        }
    }

    {
        register s32 i asm("$4");
        register s32 neg asm("$5");
        register s32 value asm("$6");
        register s32 offset asm("$3");

        i = 0;
        neg = -1;
        value = 0x1E00;
        offset = 0;
        for (; i < 4; i++) {
            *(s32 *)(D_801E6D38 + offset) = neg;
            *(s32 *)(D_801E6D30 + offset) = neg;
            *(s32 *)(D_801E6D34 + offset) = neg;
            *(s32 *)(D_801E6D3C + offset) = value;
            *(s32 *)(D_801E6D40 + offset) = 0;
            offset += 0x14;
        }
    }

    {
        register s32 eighty asm("$3");
        register s32 value asm("$2");

        eighty = 0x80;
        value = -1;
        D_801E6CBC = value;
        D_801E6CE8 = value;
        D_801E6CE4 = value;
        D_801E6CF4 = value;
        D_801E6CF0 = value;
        value = 0x1E00;
        D_801E6CF8 = value;
        value = 1;
        D_801E6CA4 = eighty;
        D_801E6CEC = 0;
        D_801E6CE0 = eighty;
        D_801E6C9C = value;
    }
}
