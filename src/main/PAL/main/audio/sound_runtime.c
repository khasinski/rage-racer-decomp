#include "common.h"
#include <stdio.h>
#include "game/audio.h"
#include "game/sound.h"
#include "psyq/snd.h"
#include "psyq/kernel.h"
#include "game/cd.h"

void SetSoundSlotVoiceEnabled(s32 slot, s32 enabled) {
    s32 *entry;

    if (enabled != 0) {
        s32 *base = g_SoundSlotActive;
        entry = (s32 *)((slot * 4) + (u8 *)base);
        if (*entry == 0) {
            PlaySoundSlotVoice(slot, 0, 3);
            *entry = 1;
        }
    } else {
        s32 *base = g_SoundSlotActive;
        entry = (s32 *)((slot * 4) + (s32)base);
        if (*entry != 0) {
            StopSoundSlotVoice(slot);
            *entry = 0;
        }
    }
}

void SetSoundSlotVoicesEnabled(s32 enabled) {
    s32 i;

    for (i = 0; i < 6; i++) {
        if (i != 5) {
            SetSoundSlotVoiceEnabled(i, enabled);
        }
    }
}

void SetEffectVoicesEnabled(s32 enabled) {
    SetSoundSlotVoicesEnabled(enabled);
}

void ResetSoundState(void) {
    s32 i;

    {
        s32 *ptr;

        i = 5;
        ptr = &g_SoundSlotActive5;
        for (; i >= 0; i--) {
            *ptr-- = 0;
        }
    }

    {
        s32 neg;
        s32 *ptr;

        i = 0;
        neg = -1;
        ptr = &g_AudioSlotMask;
        for (; i < 2; i++) {
            g_MusicChannels[i].mode = neg;
            g_MusicChannels[i].left.value = neg;
            g_MusicChannels[i].right.value = neg;
            ptr[0x78 / 4] = 0;
            ptr = (s32 *)((u8 *)ptr + 0x18);
            g_MusicChannels[i].volLeft = 0;
        }
    }

    {
        s32 offset;

        {
            s32 i;
            s32 neg;
            s32 value;

            i = 0;
            neg = -1;
            value = 0x1E00;
            offset = 0;
            for (; i < 4; i++) {
                ((EffectVoice *)((u8 *)g_EffectVoices + offset))->state = neg;
                ((EffectVoice *)((u8 *)g_EffectVoices + offset))->note = neg;
                ((EffectVoice *)((u8 *)g_EffectVoices + offset))->tone = neg;
                ((EffectVoice *)((u8 *)g_EffectVoices + offset))->pitch = value;
                ((EffectVoice *)((u8 *)g_EffectVoices + offset))->volume = 0;
                offset += sizeof(EffectVoice);
            }
        }

        {
            s32 value;

            offset = 0x80;
            value = -1;
            g_EngineSoundBank = value;
            g_PanVoiceVolumeR = value;
            g_PanVoiceVolumeL = value;
            g_IndexedEffectIndexPrev = value;
            g_IndexedEffectIndex = value;
            value = 0x1E00;
            g_IndexedEffectPitch = value;
            value = 1;
            g_SoundScale.scale = offset;
            g_PanVoiceActive = 0;
            g_SoundSlotVolumeScale = offset;
            g_AudioSlotMask = value;
        }
    }
}

s32 InitSoundWithVab(s32 header, s32 body) {
    s16 *vabIdPtr = g_VabIds;
    s16 vabId;

    SsSetTableSize(g_SndTableArea, 2, 1);
    SsSetTickMode(1);
    SsStartSoundTickMode1();
    SsSetVoiceCount(0xA);
    SsUtReverbOff();
    SetReverbPreset(2, 0, 0);
    ResetSoundState();

    *vabIdPtr = SsVabOpenHeadSticky((u_char *)header, -1, 0x1000);
    vabId = *vabIdPtr;
    if (vabId == -1) {
        printf(g_MsgVabOpenHeadError);
        BiosExit(1);
    }

    *vabIdPtr = SsVabTransBody((u_char *)body, vabId);
    if (*vabIdPtr == -1) {
        printf(g_MsgVabTransBodyError);
        BiosExit(1);
    }

    SsVabTransCompleted(1);
    SsSetMVol(0x3FFF, 0x3FFF);
    SsSetReservedVoice(0);
    return 0;
}

s32 InitSoundRuntime(void) {
    SsSetTableSize(g_SndTableArea, 2, 1);
    SsSetTickMode(0x1000);
    SsStartSoundTickMode1();
    SsSetVoiceCount(0xA);
    SsUtReverbOff();
    SetReverbPreset(2, 0, 0);
    ResetSoundState();
    SsSetMVol(0x3FFF, 0x3FFF);
    SsSetReservedVoice(0);
    InitSequenceAudio();
    return 0;
}
