#include "common.h"
#include "game/audio.h"
#include "game/sound.h"
#include "psyq/snd.h"

extern s32 D_801E6CC8[];

void GameSetSoundSlotVoiceEnabled(s32 arg0, s32 arg1) {
    s32 *entry;
    register s32 *base asm("$2");
    register s32 offset asm("$3");

    if (arg1 != 0) {
        base = D_801E6CC8;
        offset = arg0 << 2;
        asm("" : "=r"(base), "=r"(offset) : "0"(base), "1"(offset));
        entry = (s32 *)(offset + (s32)base);
        if (*entry == 0) {
            GamePlaySoundSlotVoice(arg0, 0, 3);
            *entry = 1;
        }
    } else {
        base = D_801E6CC8;
        offset = arg0 << 2;
        asm("" : "=r"(base), "=r"(offset) : "0"(base), "1"(offset));
        entry = (s32 *)(offset + (s32)base);
        if (*entry != 0) {
            GameStopSoundSlotVoice(arg0);
            *entry = 0;
        }
    }
}

void GameSetSoundSlotVoicesEnabled(s32 arg0) {
    s32 i;

    for (i = 0; i < 6; i++) {
        if (i != 5) {
            GameSetSoundSlotVoiceEnabled(i, arg0);
        }
    }
}

void GameSetEffectVoicesEnabled(s32 arg0) {
    GameSetSoundSlotVoicesEnabled(arg0);
}

extern s32 D_801E6CBC;
extern s32 g_AudioSlotMask asm("D_801E6C9C");
extern s32 D_801E6CDC;
extern s32 D_801E6CE0;
extern s32 g_PanVoiceVolumeL asm("D_801E6CE4");
extern s32 D_801E6CE8;
extern s32 D_801E6CEC;
extern s32 D_801E6CF0;
extern s32 D_801E6CF4;
extern s32 D_801E6CF8;

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
        ptr = &g_AudioSlotMask;
        offset = 0;
        for (; i < 2; i++) {
            *(s32 *)((u8 *)&D_801E6D00[0].mode + offset) = neg;
            *(s32 *)((u8 *)&D_801E6D00[0].left + offset) = neg;
            *(s32 *)((u8 *)&D_801E6D00[0].right + offset) = neg;
            ptr[0x78 / 4] = 0;
            ptr = (s32 *)((u8 *)ptr + 0x18);
            *(s32 *)((u8 *)&D_801E6D00[0].volLeft + offset) = 0;
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
            *(s32 *)((u8 *)&D_801E6D30[0].state + offset) = neg;
            *(s32 *)((u8 *)&D_801E6D30[0].note + offset) = neg;
            *(s32 *)((u8 *)&D_801E6D30[0].tone + offset) = neg;
            *(s32 *)((u8 *)&D_801E6D30[0].pitch + offset) = value;
            *(s32 *)((u8 *)&D_801E6D30[0].volume + offset) = 0;
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
        g_PanVoiceVolumeL = value;
        D_801E6CF4 = value;
        D_801E6CF0 = value;
        value = 0x1E00;
        D_801E6CF8 = value;
        value = 1;
        g_EffectVolumeScale = eighty;
        D_801E6CEC = 0;
        D_801E6CE0 = eighty;
        g_AudioSlotMask = value;
    }
}

extern u8 D_8009B3B8[];
extern char D_8001267C[];
extern char D_80012694[];

s32 func_8007317C(s32 arg0);
s32 func_800730BC(s32 arg0, s32 arg1);
s32 func_80072C4C(s32 arg0, s32 arg1, s32 arg2);
void func_80063D9C(s32 arg0);

s32 GameInitSoundWithVab(s32 header, s32 body) {
    register s32 headerReg asm("$16") = header;
    register s32 bodyReg asm("$18") = body;
    register s16 *vabIdPtr asm("$17");
    register s32 currentVabId asm("$5");
    register s32 fail asm("$16");
    register s32 ret asm("$2");

    asm("" : "=r"(bodyReg) : "0"(bodyReg));
    SsSetTableSize(D_8009B3B8, 2, 1);
    SsSetTickMode(1);
    SsStartSoundTickMode1();
    SsSetVoiceCount(0xA);
    SsUtReverbOff();
    GameSetReverbPreset(2, 0, 0);
    GameResetSoundState();

    ret = func_80072C4C(headerReg, -1, 0x1000);
    vabIdPtr = g_VabIds;
    *vabIdPtr = ret;
    asm volatile("" : "=r"(ret) : "0"(ret));
    currentVabId = (s16)ret;
    fail = -1;
    if (currentVabId == fail) {
        GameDebugPrintf(D_8001267C);
        func_80063D9C(1);
    }

    ret = func_800730BC(bodyReg, currentVabId);
    *vabIdPtr = ret;
    if ((s16)ret == fail) {
        GameDebugPrintf(D_80012694);
        func_80063D9C(1);
    }

    func_8007317C(1);
    SsSetMVol(0x3FFF, 0x3FFF);
    SsSetReservedVoice(0);
    return 0;
}

s32 GameInitSoundRuntime(void) {
    SsSetTableSize(D_8009B3B8, 2, 1);
    SsSetTickMode(0x1000);
    SsStartSoundTickMode1();
    SsSetVoiceCount(0xA);
    SsUtReverbOff();
    GameSetReverbPreset(2, 0, 0);
    GameResetSoundState();
    SsSetMVol(0x3FFF, 0x3FFF);
    SsSetReservedVoice(0);
    GameInitSequenceAudio();
    return 0;
}
