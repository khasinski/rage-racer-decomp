#include "common.h"
#include "game/audio.h"
#include "psyq/snd.h"

extern s32 D_801E40D4;
extern s32 D_801E6C9C;
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
extern s32 D_800125FC[];

s32 func_80050FA8(s32 arg0);
void func_8007865C(s32 arg0);

void func_8005DC1C(void) {
    func_8007865C(0);
    SsSetVoiceCount(8);

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
        register s32 value asm("$2");

        value = -1;
        D_801E6CE8 = value;
        D_801E6CE4 = value;
        D_801E6CF4 = value;
        D_801E6CF0 = value;
        value = 0x1E00;
        D_801E6CEC = 0;
        D_801E6CF8 = value;
    }

    GameSetEffectVoicesEnabled(1);
    GameSetReverbPreset(2, 0, 0);
    GameSetLoadedTableVolumeScale(D_800125FC[func_80050FA8(D_801E40D4)]);
}
