#include "common.h"
#include "game/audio.h"
#include "game/sound.h"
#include "psyq/snd.h"

extern s32 g_SoundSlotActive[] asm("D_801E6CC8");

void GameSetSoundSlotVoiceEnabled(s32 arg0, s32 arg1) {
    s32 *entry;

    if (arg1 != 0) {
        s32 *base = g_SoundSlotActive;
        entry = (s32 *)((arg0 << 2) + (s32)base);
        if (*entry == 0) {
            GamePlaySoundSlotVoice(arg0, 0, 3);
            *entry = 1;
        }
    } else {
        s32 *base = g_SoundSlotActive;
        entry = (s32 *)((arg0 << 2) + (s32)base);
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

extern s32 g_EngineSoundBank asm("D_801E6CBC");
extern s32 g_AudioSlotMask asm("D_801E6C9C");
extern s32 g_SoundSlotActive5 asm("D_801E6CDC");
extern s32 g_SoundSlotVolumeScale asm("D_801E6CE0");
extern s32 g_PanVoiceVolumeL asm("D_801E6CE4");
extern s32 g_PanVoiceVolumeR asm("D_801E6CE8");
extern s32 g_PanVoiceActive asm("D_801E6CEC");
extern s32 g_IndexedEffectIndex asm("D_801E6CF0");
extern s32 g_IndexedEffectIndexPrev asm("D_801E6CF4");
extern s32 g_IndexedEffectPitch asm("D_801E6CF8");

void GameResetSoundState(void) {
    {
        /* This pin is load-bearing: removing it changes .text. */
        register s32 i asm("$4");
        s32 *ptr;

        i = 5;
        ptr = &g_SoundSlotActive5;
        for (; i >= 0; i--) {
            *ptr-- = 0;
        }
    }

    {
        /* This pin is load-bearing: removing it changes .text. */
        register s32 i asm("$4");
        s32 neg;
        s32 *ptr;
        s32 offset;

        i = 0;
        neg = -1;
        ptr = &g_AudioSlotMask;
        offset = 0;
        for (; i < 2; i++) {
            *(s32 *)((u8 *)&g_MusicChannels[0].mode + offset) = neg;
            *(s32 *)((u8 *)&g_MusicChannels[0].left + offset) = neg;
            *(s32 *)((u8 *)&g_MusicChannels[0].right + offset) = neg;
            ptr[0x78 / 4] = 0;
            ptr = (s32 *)((u8 *)ptr + 0x18);
            *(s32 *)((u8 *)&g_MusicChannels[0].volLeft + offset) = 0;
            offset += 0x18;
        }
    }

    {
        s32 i;
        s32 neg;
        s32 value;
        /* This pin is load-bearing: removing it changes .text. */
        register s32 offset asm("$3");

        i = 0;
        neg = -1;
        value = 0x1E00;
        offset = 0;
        for (; i < 4; i++) {
            *(s32 *)((u8 *)&g_EffectVoices[0].state + offset) = neg;
            *(s32 *)((u8 *)&g_EffectVoices[0].note + offset) = neg;
            *(s32 *)((u8 *)&g_EffectVoices[0].tone + offset) = neg;
            *(s32 *)((u8 *)&g_EffectVoices[0].pitch + offset) = value;
            *(s32 *)((u8 *)&g_EffectVoices[0].volume + offset) = 0;
            offset += 0x14;
        }
    }

    {
        /* This pin is load-bearing: removing it changes .text. */
        register s32 eighty asm("$3");
        s32 value;

        eighty = 0x80;
        value = -1;
        g_EngineSoundBank = value;
        g_PanVoiceVolumeR = value;
        g_PanVoiceVolumeL = value;
        g_IndexedEffectIndexPrev = value;
        g_IndexedEffectIndex = value;
        value = 0x1E00;
        g_IndexedEffectPitch = value;
        value = 1;
        g_EffectVolumeScale = eighty;
        g_PanVoiceActive = 0;
        g_SoundSlotVolumeScale = eighty;
        g_AudioSlotMask = value;
    }
}

extern u8 g_SndTableArea[] asm("D_8009B3B8");
extern char g_MsgVabOpenHeadError[] asm("D_8001267C");
extern char g_MsgVabTransBodyError[] asm("D_80012694");

s32 func_8007317C(s32 arg0);
s32 func_800730BC(s32 arg0, s32 arg1);
s32 func_80072C4C(s32 arg0, s32 arg1, s32 arg2);
void func_80063D9C(s32 arg0);

s32 GameInitSoundWithVab(s32 header, s32 body) {
    s32 headerReg = header;
    s32 bodyReg = body;
    s16 *vabIdPtr;
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 currentVabId asm("$5");
    s32 fail;
    register s32 ret asm("$2");

    SsSetTableSize(g_SndTableArea, 2, 1);
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
        GameDebugPrintf(g_MsgVabOpenHeadError);
        func_80063D9C(1);
    }

    ret = func_800730BC(bodyReg, currentVabId);
    *vabIdPtr = ret;
    if ((s16)ret == fail) {
        GameDebugPrintf(g_MsgVabTransBodyError);
        func_80063D9C(1);
    }

    func_8007317C(1);
    SsSetMVol(0x3FFF, 0x3FFF);
    SsSetReservedVoice(0);
    return 0;
}

s32 GameInitSoundRuntime(void) {
    SsSetTableSize(g_SndTableArea, 2, 1);
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
