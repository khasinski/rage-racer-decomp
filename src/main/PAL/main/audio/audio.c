#include "common.h"
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
                *(s32 *)((u8 *)&g_EffectVoices[0].state + offset) = neg;
                *(s32 *)((u8 *)&g_EffectVoices[0].note + offset) = neg;
                *(s32 *)((u8 *)&g_EffectVoices[0].tone + offset) = neg;
                *(s32 *)((u8 *)&g_EffectVoices[0].pitch + offset) = value;
                *(s32 *)((u8 *)&g_EffectVoices[0].volume + offset) = 0;
                offset += 0x14;
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
        DebugPrintf(g_MsgVabOpenHeadError);
        BiosExit(1);
    }

    *vabIdPtr = SsVabTransBody((u_char *)body, vabId);
    if (*vabIdPtr == -1) {
        DebugPrintf(g_MsgVabTransBodyError);
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

#include "common.h"
#include "game/audio.h"
#include "psyq/snd.h"


s32 StartAudioSlotLoad(s32 slot, s32 header, s32 body, s32 table) {
    s16 vabId;

    if (slot == 3) {
        return (s16)StartVabTransferWithTable(header, body, (u16 *)table);
    }
    if (slot == 1 || slot == 6) {
        return (s16)OpenVabSequenceSlot(slot, header, body, table);
    }

    g_AudioLoadSlot = slot;
    g_VabIds[slot] = SsVabOpenHeadSticky((u_char *)header, -1, g_VabSpuAddress[slot]);
    /* Reading the slot back is what keeps the opened id in a register: the
       store is a halfword, so the reload is folded into a sign-extend of the
       call result and that value is still there to hand to SsVabTransBody. */
    vabId = g_VabIds[slot];
    if (vabId == -1) {
        DebugPrintf(g_MsgVabOpenHeadError);
        BiosExit(1);
    }

    g_VabIds[slot] = SsVabTransBody((u_char *)body, vabId);
    if (g_VabIds[slot] == -1) {
        DebugPrintf(g_MsgVabTransBodyError);
        BiosExit(1);
    }

    g_VabTransferDone = SsVabTransCompleted(0);
    return g_VabTransferDone;
}

s32 PollAudioSlotLoad(void) {
    s32 completed;
    register s32 *flagsPtr asm("$4");
    register s32 slot asm("$5");
    s32 one;
    s32 value;
    s32 bit;

    completed = SsVabTransCompleted(0);
    g_VabTransferDone = (s16)completed;

    if ((s16)completed != 0) {
        flagsPtr = &g_AudioSlotMask;
        one = 1;
        slot = g_AudioLoadSlot;
        value = *flagsPtr;
        bit = (s16)(one << slot);
        *flagsPtr = bit | value;

        if (slot == 0) {
            g_SoundCueBank = one;
        } else if (slot == one) {
            g_SoundCueBank = slot;
        } else {
            value = 2;
            if ((slot == value) || (slot == 3)) {
                g_SoundCueBank = value;
            }
        }
    }

    return (s16)g_VabTransferDone;
}

s32 CloseVabOnlyAudioSlot(s32 slot) {
    s32 *flagsPtr = &g_AudioSlotMask;
    s32 bit = 1;
    s32 flags = *flagsPtr;
    s32 zeroArg = 0;
    s32 ret;
    s16 *ids;

    bit <<= slot;

    if (!(bit & flags)) {
    ret = 0;
    } else {
    *flagsPtr = bit ^ flags;
    SsUtSetReverbDepth(zeroArg, 0);
    _SsVmInit(0);
    /* g_VabIds sits 0xC bytes past the slot mask; deriving it from flagsPtr
       (rather than naming the symbol) is what the retail code does. */
    ids = (s16 *)((u8 *)flagsPtr + 0xC);
    SsVabClose(ids[slot]);
    ret = 1;
    }
    return ret;
}

s32 CloseLoadedAudioSlots(void) {
    SpuVmDamperStep();
    if (CloseAudioSlot(1) == 0) {
        return 0;
    }
    if (CloseVabOnlyAudioSlot(2) == 0) {
        return 0;
    }
    if (CloseVabOnlyAudioSlot(3) == 0) {
        return 0;
    }
}

s32 StartVabTransferWithTable(s32 header, s32 body, u16 *table) {
    /* $18 (s2) is the one thing this shape cannot reach on its own. The slot
       pointer and `table` both want a callee-saved register, both have three
       references, and gcc's priority is refs/live-length: 3/24 for the pointer
       against 3/23 for `table`, so `table` is allocated first and takes s2.
       Retail has the pointer in s2, which needs the pointer to win. Nothing in
       the C decides that here -- the two live ranges are fixed by the call
       sequence, and every shape tried (pointer vs array vs global, local copies
       of every parameter, declaration order, the check reading the pointer or
       the global or a second local) leaves 23 against 24 unchanged. */
    register s16 *vabIdPtr asm("$18") = &g_VabIds[3];
    s16 vabId;

    g_AudioLoadSlot = 3;
    *vabIdPtr = SsVabOpenHeadSticky((u_char *)header, -1, g_VabSpuAddress[3]);
    vabId = *vabIdPtr;
    if (vabId == -1) {
        DebugPrintf(g_MsgVabOpenHeadError);
        BiosExit(1);
    }

    *vabIdPtr = SsVabTransBody((u_char *)body, vabId);
    if (*vabIdPtr == -1) {
        DebugPrintf(g_MsgVabTransBodyError);
        BiosExit(1);
    }

    if (table != 0) {
        LoadAudioParameterTable(table);
    }

    g_ExtraVabLoaded = 1;
    g_VabTransferDone = SsVabTransCompleted(0);
    return g_VabTransferDone;
}

s32 LoadExtraVabSlotWithTable(s32 header, s32 body, s32 table) {
    /* Same allocation tie as StartVabTransferWithTable: see the note there. */
    register s16 *vabIdPtr asm("$18") = &g_VabIds[3];
    s16 vabId;
    s32 flags;

    *vabIdPtr = SsVabOpenHeadSticky((u_char *)header, -1, 0x6A000);
    vabId = *vabIdPtr;
    if (vabId == -1) {
        DebugPrintf(g_MsgVabOpenHeadError);
        BiosExit(1);
    }

    *vabIdPtr = SsVabTransBody((u_char *)body, vabId);
    if (*vabIdPtr == -1) {
        DebugPrintf(g_MsgVabTransBodyError);
        BiosExit(1);
    }

    SsVabTransCompleted(1);
    if (table != 0) {
        LoadAudioParameterTable((u16 *)table);
    }

    flags = g_AudioSlotMask;
    g_ExtraVabLoaded = 1;
    g_AudioSlotMask = flags | 0x20;
    return 0;
}

void CloseExtraVabSlot(void) {
    s32 liveSlot;
    s32 *flagsPtr = &g_AudioSlotMask;
    s32 flags = *flagsPtr;
    s32 newFlags;

    if (flags & 0x20) {
        newFlags = flags ^ 0x20;
        *flagsPtr = newFlags;
        SsUtReverbOff();
        SsUtSetReverbDepth(0x28, 0x28);
        SsUtKeyOffV((s16)liveSlot);
        SsVabClose(g_VabIds[5]);
    }
}

void ShutdownSoundSystem(void) {
    s32 i;
    s32 *flag = &g_AudioSlotMask;

    if (*flag != 0) {
        *flag = 0;
        SsUtReverbOff();
        SsUtSetReverbType(0);
        SsUtSetReverbDepth(0, 0);
        i = 0;
        while (i < 24) {
            SsUtKeyOffV((s16)i);
            i++;
        }
        VSync(2);
        SsVabClose(g_VabIds[4]);
        SsVabClose(g_VabIds[5]);
        SsStopSoundTick();
        SsQuit();
    }
}

#include "common.h"
#include "game/audio.h"

void SetEffectVolumeScale(s32 scale) {
    if (scale >= 0) {
        if (scale >= 0x81) {
            scale = 0x80;
        }
    } else {
        scale = 0;
    }
    g_SoundScale.scale = scale;
}

void SetLoadedTableVolumeScale(s32 scale) {
    if (scale >= 0) {
        if (scale >= 0x81) {
            scale = 0x80;
        }
    } else {
        scale = 0;
    }
    g_SoundSlotVolumeScale = scale;
}

void SetSequenceVolumeSetting(s32 setting) {
    u32 adjusted;
    s32 value;

    value = setting;
    if (value >= 0) {
        adjusted = setting;
        adjusted++;
        adjusted--;
        setting = adjusted;
        if (setting >= 0x10) {
            setting = 0xF;
        }
    } else {
        setting = 0;
    }

    value = setting;
    SetCdVolumeSetting(setting);
    SetSequenceVolumeScale(value);
}

/* Set the effect master volume scale (g_SoundScale.scale) from a
 * 0..15 level, mapping it onto the 0..0x80 fixed-point scale used by the
 * effect-voice volume math. */
void SetEffectVolumeSetting(s32 level) {
    if (level >= 0) {
        if (level >= 0x10) {
            level = 0xF;
        }
    } else {
        level = 0;
    }
    g_SoundScale.scale = (level << 7) / 15;
}

void SetStereoOutput(void) { g_StereoOutput = 1; SetCdMixPreset(0); SsSetStereo(); }

void SetMonoOutput(void) { g_StereoOutput = 0; SetCdMixPreset(1); SsSetMono(); }

u32 GetLoadedAudioStep(void) {
    return g_EngineSoundMaxRpm;
}

s32 GetActiveAudioSlots(void) {
    return g_AudioSlotMask;
}

#include "common.h"
#include "game/audio.h"
#include "psyq/snd.h"
#include "game/sound.h"

void SetPanVoiceTargetVolume(s32 left, s32 right) {
    if (left >= 0) {
        if (left > 0x80) {
            left = 0x80;
        }
    } else {
        left = 0;
    }

    if (right >= 0) {
        if (right > 0x80) {
            right = 0x80;
        }
    } else {
        right = 0;
    }

    if (g_StereoOutput != 0) {
        g_PanVoiceVolumeL = left;
        g_PanVoiceVolumeR = right;
    } else {
        s32 temp = (left + right) / 2;

        g_PanVoiceVolumeL = temp;
        g_PanVoiceVolumeR = temp;
    }
}

void ApplyPanVoiceVolume(void) {
    s32 values[2];
    s32 changed;
    s32 i;
    s32 *dst;
    s32 *src;
    s32 raw;
    s32 loopValue;
    s32 scale;
    s32 left;
    register s32 right asm("$6");
    register s32 voice asm("$4");
    s32 zeroArg;

    changed = 0;
    i = 0;
    dst = values;
    src = &g_PanVoiceVolumeL;
    do {
        loopValue = *src;
        if (loopValue < 2) {
            *dst = 0;
        } else {
            *dst = loopValue;
            changed = 1;
        }
        dst++;
        i++;
        src++;
    } while (i < 2);

    if (changed != 0) {
        raw = values[0];
        scale = g_SoundScale.scale;
        left = raw * scale;
        raw = values[1];
        if (left < 0) {
            left += 0x7F;
        }
        raw *= scale;
        if (raw < 0) {
            raw += 0x7F;
        }
        left >>= 7;
        right = raw >> 7;

        if (left >= 0) {
            if (left >= 0x81) {
                left = 0x80;
            }
        } else {
            left = 0;
        }

        if (right >= 0) {
            if (right >= 0x81) {
                right = 0x80;
            }
        } else {
            right = 0;
        }

        SsUtSetVVol(0x15, left, right);
        if (g_PanVoiceActive == 0) {
            right = 0xF;
            voice = 0x15;
            asm volatile("" : : "r"(voice));
            raw = 0x3C;
            left = g_VabIds[0];
            zeroArg = 0;
            SsUtKeyOnV(voice, left, right, zeroArg, raw, 0, 0, 0);
        }
    } else if (g_PanVoiceActive != 0) {
        SsUtKeyOffV(0x15);
    }

    g_PanVoiceActive = changed;
}

void StartIndexedEffectVoice(s32 baseTone) {
    SsUtKeyOnV(0x14, g_VabIds[0], (s16)baseTone, 0, 0x3C, 0, 0, 0);
}

void StopIndexedEffectVoice(void) {
    SsUtKeyOffV(0x14);
}

void SetIndexedEffectVoice(s32 index, s32 phase, s32 volume) {
    if (index >= -1) {
        if (index >= 3) {
            index = 2;
        }
    } else {
        index = -1;
    }

    if (volume >= 0) {
        if (volume >= 0x80) {
            volume = 0x7F;
        }
    } else {
        volume = 0;
    }

    g_IndexedEffectIndex = index;
    if (index >= 0) {
        g_IndexedEffectVolume = volume;
        g_IndexedEffectPitch = phase;
    }
}

void UpdateIndexedEffectVoice(void) {
    s32 base;
    s32 center;
    s32 fine;
    register s32 index asm("$3");
    register s32 raw asm("$2");
    s32 product;
    s32 scale;
    register s32 left asm("$5");
    register s32 right asm("$6");
    register s32 voice asm("$4");

    raw = g_IndexedEffectIndexPrev;
    switch (0) { default:
    if (raw < 0) {
        index = g_IndexedEffectIndex;
        if (index < 0) {
            break;
        }
            raw = (index * 3) << 2;
            StartIndexedEffectVoice(INDEXED_EFFECT(raw).tone);
            } else {
        index = g_IndexedEffectIndex;
        if (index < 0) {
            StopIndexedEffectVoice();
        } else if (index != raw) {
            raw = (index * 3) << 2;
            StartIndexedEffectVoice(INDEXED_EFFECT(raw).tone);
        }
    }

    raw = g_IndexedEffectIndex;
    if (raw >= 0) {
        index = (raw * 3) << 2;
        product = g_IndexedEffectVolume * INDEXED_EFFECT(index).volume;
        raw = g_IndexedEffectPitch;
        base = INDEXED_EFFECT(index).tone;
        center = raw >> 7;
        fine = raw & 0x7F;
        if (product < 0) {
            product += 0x7F;
        }
        raw = product >> 7;
        scale = g_SoundScale.scale;
        raw *= scale;
        left = raw;
        if (raw < 0) {
            left = raw + 0x7F;
        }
        left >>= 7;
        right = left;

        if (right >= 0) {
            if (right >= 0x81) {
                left = 0x80;
            }
        } else {
            left = 0;
        }

        if (right >= 0) {
            if (right >= 0x81) {
                right = 0x80;
            }
        } else {
            right = 0;
        }

        SsUtSetVVol(0x14, left, right);
        voice = 0x14;
        left = 0;
        asm volatile("" : : "r"(voice), "r"(left));
        right = (s16)base;
        raw = (s16)center;
        SsUtChangePitch(voice, left, right, 0x3C, 0, raw, fine);
    }

    }
    g_IndexedEffectIndexPrev = g_IndexedEffectIndex;
}

extern s32 D_800126D0[] asm("g_SoundModes");
extern u8 D_801E6D00[] asm("g_MusicChannels");

/* Byte-offset view of g_MusicChannels (see game/sound.h): the retail code
 * keeps i * 0x18 in a register rather than indexing. */
#define CHANNEL(byteOffset) (*(MusicChannel *)((u8 *)g_MusicChannels + (byteOffset)))

/* Byte-offset view of the sound-mode table: the retail code keeps
 * (mode * 3) << 3 in a register rather than indexing, so the scaled offset is
 * passed in. The comparison sites below index normally and were previously
 * spelled as word offsets off a s32 * -- D_800126D0 + 14 is entry 2, slot 0. */
#define MODE(byteOffset) (*(SoundModeEntry *)((u8 *)D_800126D0 + (byteOffset)))

extern SoundModeEntry g_SoundModes[];

void SetStereoSoundCue(s32 cue, s32 left, s32 right) {
    s32 offset;
    s32 count;
    s32 i;
    s32 loopTableOffset;
    s32 average;
    /* Load-bearing: removing this $v0 pin changes four linked words. */
    register s32 scaledLeft asm("$2");
    s32 scaledRight;
    s32 entryOffset;
    s32 *table;
    s32 currentA;
    s32 currentB;
    s32 matchValue;
    s32 flag;
    s32 *base;
    SoundModeEntry *entry;

    if (cue >= 0) {
        if (cue >= 4) {
            cue = 3;
        }
    } else {
        cue = 0;
    }

    if (left >= 0) {
        if (left >= 0x80) {
            left = 0x7F;
        }
    } else {
        left = 0;
    }

    if (right >= 0) {
        if (right >= 0x80) {
            right = 0x7F;
        }
    } else {
        right = 0;
    }

    if ((left <= 0) && (right <= 0)) {
        switch (0) { default:
        left = g_MusicChannels[0].left;
        right = 0;
        if (left < 0) {
            if (g_MusicChannels[1].left < 0) {
                return;
            }
        }

        if ((u32)cue < 2) {
            if (left == g_SoundModes[0].slots[0].left) {
                currentB = g_MusicChannels[1].left;
                if (currentB == g_SoundModes[0].slots[1].left) {
                    goto found_match;
                }
            }
            if (left == g_SoundModes[1].slots[0].left) {
                currentB = g_MusicChannels[1].left;
                matchValue = g_SoundModes[1].slots[1].left;
                break;
            }
        } else {
            if (left == g_SoundModes[2].slots[0].left) {
                currentB = g_MusicChannels[1].left;
                if (currentB == g_SoundModes[2].slots[1].left) {
                    goto found_match;
                }
            }
            if (left == g_SoundModes[3].slots[0].left) {
                currentB = g_MusicChannels[1].left;
                matchValue = g_SoundModes[3].slots[1].left;
                break;
            }
        }
        goto after_match;
        }
        if (!(currentB != matchValue)) {
found_match:
        right = 1;
        }
after_match:

        if (right != 0) {
            /* Load-bearing: removing this $v0 pin changes 139 linked words. */
            s32 resetLoad;
            s32 resetCount;
            s32 inactiveValue;
            s32 activeValue;

            resetLoad = MODE((cue * 3) << 3).count;
            i = 0;
            if (resetLoad <= i) {
                return;
            }
            inactiveValue = -1;
            activeValue = 1;
            resetCount = resetLoad;
            do {
                offset = i * 0x18;
                CHANNEL(offset).left = inactiveValue;
                CHANNEL(offset).right = inactiveValue;
                CHANNEL(offset).mode = activeValue;
                table = (s32 *)((u8 *)&g_AudioSlotMask + offset);
                *(s32 *)((s32)table + 0x78) = 0;
                CHANNEL(offset).volLeft = 0;
                i++;
            } while (i < resetCount);
        }
        return;
    }

    currentA = g_MusicChannels[0].left;
    if (currentA == MODE((cue * 3) << 3).slots[0].left) {
        currentB = g_MusicChannels[1].left;
        if (currentB == MODE((cue * 3) << 3).slots[1].left) {
            g_MusicChannels[0].mode = 2;
        } else {
            g_MusicChannels[0].mode = 0;
        }
    } else {
        g_MusicChannels[0].mode = 0;
    }

    i = 0;
    loopTableOffset = (cue * 3) << 3;
    cue = MODE(loopTableOffset).count;
    if (cue <= i) {
        return;
    }

    average = (left + right) / 2;
    count = cue;
    /* Load-bearing: removal changes five linked preheader words. */
    asm("" : "=r"(count) : "0"(count));
    base = D_800126D0;
    entryOffset = loopTableOffset;
    entry = (SoundModeEntry *)((s32)base + entryOffset);
    cue = 0;
    do {
        if (i != 0) {
            /* Asymmetric on purpose: retail writes channel[i].mode through the
             * indexed form but reads channel[0].mode through its own symbol.
             * Spelling the read as g_MusicChannels[0].mode costs an
             * instruction, with or without RAW() on either side. */
            CHANNEL(cue).mode = D_801E6D08;
        }

        flag = g_StereoOutput;
        CHANNEL(cue).left = entry->slots[0].left;
        CHANNEL(cue).right = entry->slots[0].right;
        if (flag != 0) {
            currentB = MODE(entryOffset).factor;
            scaledLeft = left * currentB;
            if (scaledLeft < 0) {
                scaledLeft += 0x7F;
            }
            scaledLeft >>= 7;
            *(volatile s32 *)&CHANNEL(cue).volLeft = scaledLeft;
            scaledRight = right * currentB;
            entry = (SoundModeEntry *)((u8 *)entry + 8);
            if (scaledRight < 0) {
                scaledRight += 0x7F;
            }
            scaledRight >>= 7;
            *(volatile s32 *)&CHANNEL(cue).volRight = scaledRight;
            i++;
        } else {
            if ((scaledLeft = average * MODE(entryOffset).factor) < 0) {
                currentB = scaledLeft + 0x7F;
            } else {
                currentB = scaledLeft;
            }
            currentB >>= 7;
            *(volatile s32 *)&CHANNEL(cue).volLeft = currentB;
            *(volatile s32 *)&CHANNEL(cue).volRight = currentB;
            /* Load-bearing: removal changes eight linked scheduler words. */
            asm volatile("");
            entry = (SoundModeEntry *)((s32)entry + 8);
            i++;
        }
        cue += 0x18;
    } while (i < count);
}

#define UPDATE_BASIC_EFFECT_VOLUME()                                  \
    raw = *(s32 *)((u8 *)&g_MusicChannels[0].volLeft + offset);                              \
    scale = g_SoundScale.scale;                                                \
    left = raw * scale;                                                \
    raw = *(s32 *)((u8 *)&g_MusicChannels[0].volRight + offset);                              \
    voice = i + 8;                                                     \
    if (left < 0) {                                                    \
        left += 0x7F;                                                  \
    }                                                                 \
    raw *= scale;                                                      \
    if (raw < 0) {                                                     \
        raw += 0x7F;                                                   \
    }                                                                 \
    left >>= 7;                                                        \
    right = raw >> 7;                                                  \
    if (left >= 0) {                                                   \
        if (left >= 0x81) {                                            \
            left = 0x80;                                               \
        }                                                             \
    } else {                                                          \
        left = 0;                                                      \
    }                                                                 \
    if (right >= 0) {                                                  \
        if (right >= 0x81) {                                           \
            right = 0x80;                                              \
        }                                                             \
    } else {                                                          \
        right = 0;                                                     \
    }                                                                 \
    SsUtSetVVol((s16)voice, (s16)left, (s16)right);                   \
    *state = neg

#define START_BASIC_EFFECT_VOLUME()                                   \
    raw = *(s32 *)((u8 *)&g_MusicChannels[0].volLeft + offset);                              \
    scale = g_SoundScale.scale;                                                \
    left = raw * scale;                                                \
    raw = i + 8;                                                       \
    asm("" : "=r"(raw) : "0"(raw));                                    \
    voice = raw;                                                       \
    raw = *(s32 *)((u8 *)&g_MusicChannels[0].volRight + offset);                              \
    if (left < 0) {                                                    \
        left += 0x7F;                                                  \
    }                                                                 \
    raw *= scale;                                                      \
    if (raw < 0) {                                                     \
        raw += 0x7F;                                                   \
    }                                                                 \
    left >>= 7;                                                        \
    right = raw >> 7;                                                  \
    if (left >= 0) {                                                   \
        if (left >= 0x81) {                                            \
            left = 0x80;                                               \
        }                                                             \
    } else {                                                          \
        left = 0;                                                      \
    }                                                                 \
    if (right >= 0) {                                                  \
        if (right >= 0x81) {                                           \
            right = 0x80;                                              \
        }                                                             \
    } else {                                                          \
        right = 0;                                                     \
    }                                                                 \
    SsUtSetVVol((s16)voice, (s16)left, (s16)right);                   \
    *state = neg

void UpdateBasicEffectVoices(void) {
    s32 offset;
    s32 *state;
    s32 i;
    s32 voicePacked;
    s32 neg;
    s32 raw;
    s32 scale;
    s32 voice;
    s32 left;
    s32 right;

    i = 0;
    neg = -1;
    state = &g_MusicChannels[0].mode;
    voicePacked = 0x80000;
    offset = 0;
    do {
        switch (*state) {
        case 0:
            SsUtKeyOnV(voicePacked >> 16, g_VabIds[0],
                          *(s16 *)((u8 *)&g_MusicChannels[0].left + offset),
                          *(s16 *)((u8 *)&g_MusicChannels[0].right + offset), 0x3C, 0, 0, 0);
            START_BASIC_EFFECT_VOLUME();
            break;
        case 2:
            UPDATE_BASIC_EFFECT_VOLUME();
            break;
        case 1:
            SsUtKeyOffV(voicePacked >> 16);
            *state = neg;
            break;
        }
        state = (s32 *)((u8 *)state + 0x18);
        voicePacked += 0x10000;
        i++;
        offset += 0x18;
    } while (i < 2);
}

#include "common.h"
#include "game/audio.h"
#include "psyq/snd.h"
#include "game/sound.h"

extern s32 D_801E6D30 asm("g_EffectVoices");

void SetPitchedSoundCue(s32 bank, s32 pitch, s32 volume) {
    s32 count;
    s32 loopCount;
    s32 i;
    s32 off;
    /* Load-bearing GCC 2.6.3 register roles for the table loops. */
    register s32 tblOff asm("$3");
    register s32 loopTblOff asm("$8");
    s32 compareOff;
    s32 scaled;
    s32 scaleValue;
    s32 cueValue;
    s32 toneValue;
    s32 hasActiveVoice;
    s32 ok;
    s32 bankIndex;
    s32 active;
    s32 inactive;
    s32 defaultPitch;
    register const EffectCueRow *p asm("$7");
    const s32 *tableBase;
    s32 *stateBase;

    if (bank >= 0) {
        if (bank >= 3) {
            bank = 2;
        }
    } else {
        bank = 0;
    }
    if (volume >= 0) {
        if (volume >= 0x80) {
            volume = 0x7F;
        }
    } else {
        volume = 0;
    }

    switch (bank) {
    case 0:
        if (volume <= 0) {
            s32 resetIndex;

            resetIndex = 0;
            if ((D_801E6D30 >= 0) || (D_801E6D44 >= 0)) {
                count = g_EffectCueTable[0];
                if (count > 0) {
                    active = 1;
                    inactive = -1;
                    defaultPitch = 0x1E00;
                    volume = count;
                    off = 0;
                    do {
                        *(s32 *)((u8 *)g_EffectVoiceState + off) = active;
                        *(s32 *)((u8 *)&D_801E6D30 + off) = inactive;
                        *(s32 *)((u8 *)g_EffectVoiceTone + off) = inactive;
                        *(s32 *)((u8 *)g_EffectVoicePitch + off) = defaultPitch;
                        *(s32 *)((u8 *)g_EffectVoiceVolume + off) = 0;
                        off += 0x14;
                        resetIndex++;
                    } while (resetIndex < volume);
                }
            }
        } else {
            if ((D_801E6D30 == D_80012738) &&
                (D_801E6D44 == D_80012740)) {
                *g_EffectVoiceState = 2;
            } else {
                *g_EffectVoiceState = 0;
            }
            bankIndex = bank * 3;
            tblOff = bankIndex * 8;
            count = *(s32 *)((u8 *)g_EffectCueTable + tblOff);
            i = 0;
            if (count > i) {
                stateBase = g_EffectVoiceState;
                loopCount = count;
                tableBase = g_EffectCueTable;
                loopTblOff = tblOff;
                p = (const EffectCueRow *)((u8 *)tableBase + loopTblOff);
                off = 0;
                do {
                    if (i != 0) {
                        *(s32 *)((u8 *)g_EffectVoiceState + off) = *stateBase;
                    }
                    scaleValue = *(s32 *)((u8 *)&D_80012734 + loopTblOff);
                    scaled = volume * scaleValue;
                    cueValue = p->cue;
                    *(s32 *)((u8 *)&D_801E6D30 + off) = cueValue;
                    toneValue = p->tone;
                    *(s32 *)((u8 *)g_EffectVoicePitch + off) = pitch;
                    *(s32 *)((u8 *)g_EffectVoiceTone + off) = toneValue;
                    p = (const EffectCueRow *)((u8 *)p + 8);
                    if (scaled < 0) {
                        scaled += 0x7F;
                    }
                    *(s32 *)((u8 *)g_EffectVoiceVolume + off) = scaled >> 7;
                    i++;
                    off += 0x14;
                } while (i < loopCount);
            }
        }
        break;

    case 1:
    case 2:
        if (volume <= 0) {
            hasActiveVoice = D_801E6D58 >= 0;
            ok = 0;
            if (hasActiveVoice || (D_801E6D6C >= 0)) {
                if (bank == 1) {
                    if (D_801E6D58 == D_80012750) {
                        ok = D_801E6D6C == D_80012758;
                    }
                } else if ((bank == 2) && (D_801E6D58 == D_80012768) &&
                           (D_801E6D6C == D_80012770)) {
                    ok = 1;
                }
                if (ok != 0) {
                    count = g_EffectCueTable[bank * 6];
                    i = 0;
                    if (count > i) {
                        active = 1;
                        inactive = -1;
                        defaultPitch = 0x1E00;
                        volume = count;
                        off = 0x28;
                        do {
                            *(s32 *)((u8 *)g_EffectVoiceState + off) = active;
                            *(s32 *)((u8 *)&D_801E6D30 + off) = inactive;
                            *(s32 *)((u8 *)g_EffectVoiceTone + off) = inactive;
                            *(s32 *)((u8 *)g_EffectVoicePitch + off) = defaultPitch;
                            *(s32 *)((u8 *)g_EffectVoiceVolume + off) = 0;
                            off += 0x14;
                            i++;
                        } while (i < volume);
                    }
                }
            }
        } else {
            compareOff = bank * 0x18;
            if ((D_801E6D58 ==
                 *(s32 *)((u8 *)&D_80012738 + compareOff)) &&
                (D_801E6D6C ==
                 *(s32 *)((u8 *)&D_80012740 + compareOff))) {
                D_801E6D60 = 2;
                tblOff = bank * 2;
            } else {
                D_801E6D60 = 0;
                tblOff = bank * 2;
            }
            tblOff = (tblOff + bank) * 8;
            bankIndex = bank * 3;
            tblOff = bankIndex * 8;
            count = *(s32 *)((u8 *)g_EffectCueTable + tblOff);
            i = 0;
            if (count > i) {
                stateBase = &D_801E6D60;
                loopCount = count;
                tableBase = g_EffectCueTable;
                loopTblOff = tblOff;
                p = (const EffectCueRow *)((u8 *)tableBase + loopTblOff);
                off = 0x28;
                do {
                    if (i != 0) {
                        *(s32 *)((u8 *)g_EffectVoiceState + off) = *stateBase;
                    }
                    scaleValue = *(s32 *)((u8 *)&D_80012734 + loopTblOff);
                    scaled = volume * scaleValue;
                    cueValue = p->cue;
                    *(s32 *)((u8 *)&D_801E6D30 + off) = cueValue;
                    toneValue = p->tone;
                    *(s32 *)((u8 *)g_EffectVoicePitch + off) = pitch;
                    *(s32 *)((u8 *)g_EffectVoiceTone + off) = toneValue;
                    p = (const EffectCueRow *)((u8 *)p + 8);
                    if (scaled < 0) {
                        scaled += 0x7F;
                    }
                    *(s32 *)((u8 *)g_EffectVoiceVolume + off) = scaled >> 7;
                    i++;
                    off += 0x14;
                } while (i < loopCount);
            }
        }
        break;
    }
}

/* Loop over the 4 effect voices (indices 10..13). `voice` is kept in the
 * compiler's scaled (<<16) representation of the short voice number, exactly
 * as GCC materialises it, and read back with `voice >> 16`. */
#define VOLPITCH()                                                    \
    svArg = voiceCopy;                                                \
                               \
    prod = *(s32 *)(g_EffectVoiceVolume + offset) * g_SoundScale.scale;                \
    left = prod;                                                      \
    if (prod < 0) {                                                   \
        left = prod + 0x7F;                                           \
    }                                                                 \
    left >>= 7;                                                       \
    right = left;                                                     \
    if (right >= 0) {                                                 \
        if (right >= 0x81) {                                          \
            left = 0x80;                                              \
        }                                                             \
    } else {                                                          \
        left = 0;                                                     \
    }                                                                 \
    if (right >= 0) {                                                 \
        if (right >= 0x81) {                                          \
            right = 0x80;                                             \
        }                                                             \
    } else {                                                          \
        right = 0;                                                    \
    }                                                                 \
    SsUtSetVVol((s16)svArg, left, right);                             \
    SsUtChangePitch(voice >> 16, 0, *f0Ptr, 0x3C, 0,                  \
                    (s16)(*pitchPtr >> 7), *(u16 *)pitchPtr & 0x7F);  \
    *statePtr = neg

void UpdateEffectVoiceStates(void) {
    s32 *statePtr;
    s32 *pitchPtr;
    s16 *f0Ptr;
    s32 offset;
    s32 voiceCopy;
    s32 neg;
    s32 svArg;
    register s32 left asm("$5");
    s32 right;
    s32 prod;
    s32 voice;
    s32 state;

    neg = -1;
    statePtr = g_EffectVoiceState;
    voice = 10 << 16;
    voiceCopy = 10;
    pitchPtr = statePtr + 1;
    f0Ptr = (s16 *)statePtr - 4;
    offset = 0;
    do {
        state = *statePtr;
        switch (state) {
        case 0:
            SsUtKeyOnV(voice >> 16, g_VabIds[0], *f0Ptr,
                          *(s16 *)(g_EffectVoiceTone + offset), 0x3C, 0, 0, 0);
            VOLPITCH();
            break;
        case 2:
            VOLPITCH();
            break;
        case 1:
            SsUtKeyOffV(voice >> 16);
            *statePtr = neg;
            break;
        }
        statePtr = (s32 *)((u8 *)statePtr + 0x14);
        voice += 1 << 16;
        voiceCopy++;
        pitchPtr = (s32 *)((u8 *)pitchPtr + 0x14);
        f0Ptr = (s16 *)((u8 *)f0Ptr + 0x14);
        offset += 0x14;
    } while ((s32)statePtr < (s32)&g_EffectVoiceState[20]);
}


/* `note` is what every caller passes as the MIDI note, always 0x3C. */
s32 StartSoundCueVoice(s32 cue, s32 note, s32 volL, s32 volR) {
    const s32 *voiceBits;
    s32 busy[6];
    s32 tone2;
    s32 vab;
    s32 prog;
    s32 tone;
    s32 baseVol;
    s32 scale;
    s32 scaled;
    s32 result;
    s32 i;

    tone = 0;
    voiceBits = g_SpecialVoiceBits;
    tone2 = 1;
    if (g_SoundCueBank == 1) {
        tone2 = g_SoundCueParams[cue][4];
        vab = g_SoundCueParams[cue][1];
        prog = g_SoundCueParams[cue][2];
        tone = g_SoundCueParams[cue][3];
        baseVol = g_SoundCueParams[cue][0];
    } else {
        vab = 0;
        if (g_SoundCueBank == 2) {
            tone2 = g_SoundCueParams2[cue][4];
            vab = g_SoundCueParams2[cue][1];
            prog = g_SoundCueParams2[cue][2];
            tone = g_SoundCueParams2[cue][3];
            baseVol = g_SoundCueParams2[cue][0];
        } else {
            prog = cue;
            baseVol = 0x80;
        }
    }

    scaled = volL * baseVol;
    if (scaled < 0) {
        scaled += 0x7F;
    }
    scale = g_SoundScale.scale;
    scaled = (scaled >> 7) * scale;
    if (scaled < 0) {
        scaled += 0x7F;
    }
    volL = scaled >> 7;
    scaled = volR * baseVol;
    if (scaled < 0) {
        scaled += 0x7F;
    }
    scaled = (scaled >> 7) * scale;
    if (scaled < 0) {
        scaled += 0x7F;
    }
    volR = scaled >> 7;

    if (g_SoundCueBank == 1) {
        i = 0;
        do {
            busy[i] = SpuGetKeyStatus(voiceBits[i]);
            i++;
        } while (i < 6);
        i = 0;
        do {
            if (busy[i] == 0) {
                result = (s16)SsUtKeyOnV((s16)(i + 0x12), g_SoundScale.values[vab],
                                         (s16)prog, (s16)tone, 0x3C, 0,
                                         (s16)volL, (s16)volR);
                g_SpecialCueVoiceA = result;
                busy[i] = 1;
                break;
            }
            i++;
        } while (i < 6);
        i = 0;
        do {
            if (busy[i] == 0) {
                result = (s16)SsUtKeyOnV((s16)(i + 0x12), g_SoundScale.values[vab],
                                         (s16)prog, (s16)tone2, 0x3C, 0,
                                         (s16)volL, (s16)volR);
                g_SpecialCueVoiceB = result;
                busy[i] = 1;
                break;
            }
            i++;
        } while (i < 6);
    } else {
        result = (s16)SsUtKeyOn(g_SoundScale.values[vab], prog, tone, 0x3C, 0,
                                volL, volR);
        result = (s16)SsUtKeyOn(g_SoundScale.values[(g_SpecialCueVoiceA = result, vab)],
                                prog, tone2, 0x3C, 0, volL, volR);
        g_SpecialCueVoiceB = result;
    }

    if (result < 0) {
        DebugPrintf(D_80012778);
        return -1;
    }
    return result;
}


s32 StartSingleSpecialCue(s32 cue, s32 volume) {
    s32 result = -1;
    s32 *handle;
    s32 value;
    s32 offset;
    s32 tone;
    s32 pitch;
    s32 scaled;
    s32 scaleValue;
    s32 current;

    handle = &g_SpecialCueVoiceA;
    g_SpecialCueVoiceB = result;
    *handle = result;
    current = g_ActiveSpecialCue;

    if (current != cue) {
        scaled = volume * g_SoundCueParams[cue][0];
        offset = g_SoundCueParams[cue][1];
        tone = g_SoundCueParams[cue][2];
        pitch = g_SoundCueParams[cue][3];
        if (scaled < 0) {
            scaled += 0x7F;
        }

        result = g_SoundScale.scale;
        value = scaled >> 7;
        value *= result;
        result = value;
        if (result < 0) {
            result += 0x7F;
        }

        scaleValue = g_SoundScale.values[offset];
        tone = (s16)tone;
        pitch = (s16)pitch;
        result = (s16)SsUtKeyOnV(
            0x13,
            scaleValue,
            tone,
            pitch,
            0x3C,
            0,
            (s32)((u32)result << 9) >> 16,
            (s32)((u32)result << 9) >> 16);
        *handle = result;
    }

    g_ActiveSpecialCue = cue;
    return result;
}

s32 StartSpecialCueVoice(s32 cue, s32 volumeLeft, s32 volumeRight) {
    s32 id;
    s32 pan;
    s32 prog;
    s32 tone;
    s32 sx;
    s32 sy;
    s32 result;
    s32 baseVol;
    s32 scale;
    s32 vx;
    s32 vy;
    s32 nextTone;

    id = cue;
    sy = volumeRight;
    baseVol = g_SoundCueParams2[id][0];
    pan = g_SoundCueParams2[id][1];
    prog = g_SoundCueParams2[id][2];
    tone = g_SoundCueParams2[id][3];

    vx = baseVol * volumeLeft;
    if (vx < 0) {
        vx += 0x7F;
    }
    scale = g_SoundScale.scale;
    sx = (vx >> 7) * scale;
    if (sx < 0) {
        sx += 0x7F;
    }
    vy = baseVol * sy;
    if (vy < 0) {
        vy += 0x7F;
    }
    sx >>= 7;
    volumeRight = (vy >> 7) * scale;
    if (volumeRight < 0) {
        volumeRight += 0x7F;
    }
    sy = volumeRight >> 7;

    if ((SpuGetKeyStatus(D_80011C84) == 0) || (id == 0x3D) || (id == 0x2B)) {
        result = (s16)SsUtKeyOnV(
            0x16,
            g_VabIds[pan],
            (s16)prog,
            (s16)tone,
            0x3C,
            0,
            (s16)sx,
            (s16)sy);
        nextTone = tone + 1;
        nextTone = (s32)((u32)nextTone << 16) >> 16;
        result = (s16)SsUtKeyOnV(
            0x17,
            g_VabIds[(g_SpecialCueVoiceA = result, pan)],
            (s16)prog,
            nextTone,
            0x3C,
            0,
            (s16)sx,
            (s16)sy);
        g_SpecialCueVoiceB = result;
    }

    return result;
}

void PlaySoundCue(s32 cue) {
    if (g_SoundCueBank == 1) {
        if (cue >= 0) {
            if (cue >= 0x1E) {
                cue = 0x1D;
            }
        } else {
            cue = 0;
        }

        if ((u32)(cue - 0xF) < 3U) {
            if (cue != g_LastSpecialCueRequest) {
                g_LastSpecialCueRequest = cue;
                StartSingleSpecialCue(cue, 0x80);
            }
            return;
        }
        StartSoundCueVoice(cue, 0x3C, 0x80, 0x80);
        return;
    }

    if (g_SoundCueBank == 2) {
        if (cue >= 0) {
            if (cue >= 0x46) {
                cue = 0x45;
            }
        } else {
            cue = 0;
        }

        if ((u32)(cue - 0xF) < 3U) {
            if (cue != g_LastSpecialCueRequest) {
                g_LastSpecialCueRequest = cue;
                StartSingleSpecialCue(cue, 0x80);
            }
            return;
        }
        if (cue < 0x19) {
            StartSoundCueVoice(cue, 0x3C, 0x80, 0x80);
            return;
        }
        StartSpecialCueVoice(cue, 0x80, 0x80);
    }
}


/* Sets one engine-sound slot: scales `volume` by the global effect scale,
 * pushes it to the slot's voice, then re-pitches that voice to the tone at
 * g_SoundSlotTone[slot][toneIndex]. */
void SetSoundSlotTone(s32 slot, s32 bend, s32 volume, s32 toneIndex, u16 vabSlot) {
    s32 voice;
    register s32 left asm("$5");
    s32 right;
    s32 prod;
    s32 vab;
    s32 voiceCopy;

    prod = volume * g_SoundScale.scale;
    voice = slot + 0xE;
    voiceCopy = voice;
    vab = vabSlot;
    left = prod;
    if (prod < 0) {
        left = prod + 0x7F;
    }
    left >>= 7;
    right = left;
    if (right >= 0) {
        if (right >= 0x81) {
            left = 0x80;
        }
    } else {
        left = 0;
    }
    if (right >= 0) {
        if (right >= 0x81) {
            right = 0x80;
        }
    } else {
        right = 0;
    }
    SsUtSetVVol((s16)voiceCopy, left, right);
    voice = slot + 0xE;
    SsUtPitchBend((s16)voice, g_VabIds[(s16)vab], g_SoundSlotTone[slot][toneIndex], 0x3C, (s16)bend);
}

#include "common.h"
#include "game/audio.h"
#include "game/sound.h"
#include "psyq/snd.h"
#include "game/car.h"

s32 InterpolateAudioParameter(s32 parameter, s32 position, s32 bank) {
    s32 value = position;
    s32 index;
    s32 index_offset;
    s32 *base = (s32 *)g_EngineSoundCurves;
    s32 row_offset;
    s32 bank_offset;
    s32 *base_minus;
    s32 *entry;
    s32 *scan;
    s32 *lower_position;
    s32 lower_value_base;
    s32 lower_value_indexed;
    s32 lower_value_address;
    s32 upper_value_base;
    s32 upper_value_banked;
    s32 upper_value_address;
    s32 lower_position_value;
    s32 lower_value_value;
    s32 upper_value_value;
    s32 numerator;
    s32 denominator;
    s32 raw_result;
    s32 result;

    index = 1;
    row_offset = (parameter * 9) << 3;
    bank_offset = (((bank * 7) * 4) - bank) << 5;
    bank = row_offset + bank_offset;
    entry = (s32 *)((s32)base + bank);
    scan = entry + 1;
    while (index < 9) {
        raw_result = *scan;
        if (value < raw_result) {
            break;
        }
        index++;
        scan++;
    }

    base_minus = base - 1;
    index_offset = index * 4;
    lower_position = (s32 *)(index_offset + (s32)base_minus);
    lower_position = (s32 *)((u8 *)lower_position + bank);
    lower_value_base = (s32)(base + 8);
    lower_value_indexed = index_offset + lower_value_base;
    lower_value_address = lower_value_indexed + bank;
    upper_value_base = (s32)(base + 9);
    upper_value_banked = bank + upper_value_base;
    upper_value_address = index_offset + upper_value_banked;
    lower_value_value = *(s32 *)lower_value_address;
    upper_value_value = *(s32 *)upper_value_address;
    lower_position_value = *lower_position;
    numerator =
        (upper_value_value - lower_value_value) *
        (value - lower_position_value);
    denominator =
        *(s32 *)(index_offset + (s32)entry) - lower_position_value;
    raw_result = numerator / denominator + lower_value_value;

    if (raw_result >= 0) {
        result = raw_result;
        if (result >= 0x80) {
            result = 0x7F;
        }
    } else {
        result = 0;
    }

    return result;
}

void UpdateLoadedAudioVoices(s32 value, s32 bank) {
    s32 odd_parameter;
    s32 index;
    s32 second;
    s32 scaled;
    s32 *scale_base;
    s32 *slot;
    s32 *slot_base;
    s32 first;

    value = ((value * 5) << 11) / *(scale_base = &g_EngineSoundMaxRpm);

    if (bank != g_EngineSoundBank) {
        index = 0;
        slot = scale_base + 1;
        do {
            if (*slot++ != 0 &&
                g_SoundSlotTone[index][0] != g_SoundSlotTone[index][1]) {
                PlaySoundSlotVoice(index, bank, 3);
            }
            index++;
        } while (index < 6);
        g_EngineSoundBank = bank;
    }

    index = 0;
    odd_parameter = 1;
    scale_base = (slot_base = g_SoundSlotActive);
    slot = scale_base;
    do {
        if (*slot != 0) {
            first = InterpolateAudioParameter(index * 2, value, bank);
            second = InterpolateAudioParameter(odd_parameter, value, bank);
            scaled = second * slot_base[6];
            if (scaled < 0) {
                scaled += 0x7F;
            }
            SetSoundSlotTone(index, first, scaled >> 7, bank, 3);
        }
        odd_parameter += 2;
        index++;
        slot++;
    } while (index < 6);

    g_EngineSoundPosition = value;
    ApplyPanVoiceVolume();
    UpdateBasicEffectVoices();
    UpdateIndexedEffectVoice();
    UpdateEffectVoiceStates();
}

void SetDefaultReverbDepth(void) {
    SetReverbDepth(0x28, 0x28);
}

void InitSequenceAudio(void) {
    _SsVmInit(0);
    SsSetVoiceCount(0x12);
    SetReverbDepth(0x28, 0x28);
    g_ReverbFadeStep = 0;
    RefreshSequenceVolumeScale();
}

void InitEffectVoiceRuntime(void) {
    _SsVmInit(0);
    SsSetVoiceCount(8);

    {
        s32 i;
        s32 neg;
        register s32 *ptr asm("$5");
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
        s32 offset;

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
        s32 value;

        value = -1;
        g_PanVoiceVolumeR = value;
        g_PanVoiceVolumeL = value;
        g_IndexedEffectIndexPrev = value;
        g_IndexedEffectIndex = value;
        value = 0x1E00;
        g_PanVoiceActive = 0;
        g_IndexedEffectPitch = value;
    }

    SetEffectVoicesEnabled(1);
    SetReverbPreset(2, 0, 0);
    SetLoadedTableVolumeScale(g_CarSoundVolumeScales[GetOwnedCarAssetIndex(g_PlayerCarIndex)]);
}

void RestoreReverbDepth(s32 enabled) {
    if (enabled != 0) {
        SetReverbDepth(g_ReverbDepthL, g_ReverbDepthR);
    } else {
        SetReverbDepth(0, 0);
    }
}

void ForcePanVoiceEnabled(s32 enabled) {
    s32 values[2];
    s32 i;
    s32 *src;
    s32 *dst;
    s32 scale;
    s32 raw;
    register s32 voice asm("$4");
    s32 left;
    register s32 right asm("$6");
    s32 zeroArg;
    register s32 unused asm("$16");

    i = 0;
    dst = values;
    src = &g_PanVoiceVolumeL;
    do {
        if (*src < 2) {
            *dst = 0;
        } else {
            *dst = *src;
        }
        dst++;
        i++;
        src++;
    } while (i < 2);

    if (enabled != 0) {
        raw = values[0];
        scale = g_SoundScale.scale;
        left = raw * scale;
        raw = values[1];
        if (left < 0) {
            left += 0x7F;
        }
        unused = 0;
        asm volatile("" : : "r"(unused));
        raw *= scale;
        left >>= 7;
        if (raw < 0) {
            raw += 0x7F;
        }
        right = raw >> 7;

        if (left >= 0) {
            if (left >= 0x81) {
                left = 0x80;
            }
        } else {
            left = 0;
        }

        if (right >= 0) {
            if (right >= 0x81) {
                right = 0x80;
            }
        } else {
            right = 0;
        }

        SsUtSetVVol(0x15, left, right);
        voice = 0x15;
        right = 0xF;
        asm volatile("" : : "r"(voice), "r"(right));
        raw = 0x3C;
        left = g_VabIds[0];
        zeroArg = 0;
        SsUtKeyOnV(voice, left, right, zeroArg, raw, 0, 0, 0);
    } else {
        SsUtKeyOffV(0x15);
    }
}

#include "common.h"
#include "game/audio.h"
#include "game/sound.h"
#include "psyq/snd.h"

void ForceBasicEffectVoicesEnabled(s32 enabled) {
    s32 offset;
    s32 voicePacked;
    s32 voice;
    s32 i;
    s32 unused;
    s32 raw;
    s32 scale;
    s32 left;
    s32 right;
    register s32 voiceArg asm("$4");
    s32 zeroArg;

    unused = 0;
    i = 0;
    voicePacked = 0x80000;
    voice = 8;
    offset = 0;
    do {
        if (enabled != 0) {
            voiceArg = voicePacked >> 16;
            raw = 0x3C;
            left = g_VabIds[0];
            right = *(s16 *)((u8 *)&g_MusicChannels[0].left + offset);
            zeroArg = 0;
            SsUtKeyOnV(voiceArg, left, right, zeroArg, raw, 0, 0, 0);
            asm volatile("" : : "r"(unused));

            raw = *(s32 *)((u8 *)&g_MusicChannels[0].volLeft + offset);
            scale = g_SoundScale.scale;
            left = raw * scale;
            raw = *(s32 *)((u8 *)&g_MusicChannels[0].volRight + offset);
            voiceArg = voice;
            if (left < 0) {
                left += 0x7F;
            }
            raw *= scale;
            if (raw < 0) {
                raw += 0x7F;
            }
            left >>= 7;
            right = raw >> 7;

            if (left >= 0) {
                if (left >= 0x81) {
                    left = 0x80;
                }
            } else {
                left = 0;
            }

            if (right >= 0) {
                if (right >= 0x81) {
                    right = 0x80;
                }
            } else {
                right = 0;
            }

            SsUtSetVVol((s16)voiceArg, left, right);
        } else {
            SsUtKeyOffV(voicePacked >> 16);
        }

        voicePacked += 0x10000;
        voice++;
        i++;
        offset += 0x18;
    } while (i < 2);
}

void ForceIndexedEffectVoiceEnabled(s32 enabled) {
    s32 base;
    register s32 center;
    s32 fine;
    register s32 index asm("$3");
    register s32 raw asm("$2");
    s32 product;
    s32 scale;
    register s32 left asm("$5");
    register s32 right asm("$6");
    register s32 voice asm("$4");

    if (enabled != 0) {
        index = g_IndexedEffectIndexPrev;
        if (index < 0) {
            return;
        }
        raw = (index * 3) << 2;
        StartIndexedEffectVoice(INDEXED_EFFECT(raw).tone);
    } else {
        StopIndexedEffectVoice();
    }

    raw = g_IndexedEffectIndexPrev;
    if (raw >= 0) {
        index = (raw * 3) << 2;
        product = g_IndexedEffectVolume * INDEXED_EFFECT(index).volume;
        raw = g_IndexedEffectPitch;
        base = INDEXED_EFFECT(index).tone;
        center = raw >> 7;
        fine = raw & 0x7F;
        if (product < 0) {
            product += 0x7F;
        }
        raw = product >> 7;
        scale = g_SoundScale.scale;
        raw *= scale;
        left = raw;
        if (raw < 0) {
            left = raw + 0x7F;
        }
        left >>= 7;
        right = left;

        if (right >= 0) {
            if (right >= 0x81) {
                left = 0x80;
            }
        } else {
            left = 0;
        }

        if (right >= 0) {
            if (right >= 0x81) {
                right = 0x80;
            }
        } else {
            right = 0;
        }

        SsUtSetVVol(0x14, left, right);
        voice = 0x14;
        left = 0;
        asm volatile("" : : "r"(voice), "r"(left));
        right = (s16)base;
        raw = (s16)center;
        SsUtChangePitch(voice, left, right, 0x3C, 0, raw, fine);
    }
}

void ForcePitchEffectVoicesEnabled(s32 enabled) {
    s32 voicePacked;
    s32 voice;
    s32 pitchBase;
    s32 toneBase;
    s32 offset;
    s32 state;
    register s32 raw asm("$2");
    s32 scale;
    s32 left;
    register s32 right asm("$6");
    register s32 voiceArg asm("$4");
    s32 keyTone;

    state = enabled;
    voicePacked = 0xA0000;
    voice = 0xA;
    pitchBase = (s32)&g_EffectVoices[0].pitch;
    toneBase = pitchBase - 0xC;
    offset = 0;
    do {
        if (state != 0) {
            voiceArg = voicePacked >> 16;
            left = g_VabIds[0];
            right = *(s16 *)toneBase;
            keyTone = *(s16 *)((u8 *)&g_EffectVoices[0].tone + offset);
            raw = 0x3C;
            SsUtKeyOnV(voiceArg, left, right, keyTone, raw, 0, 0, 0);

            scale = *(s32 *)((u8 *)&g_EffectVoices[0].volume + offset);
            asm volatile("" : : "r"(scale));
            raw = g_SoundScale.scale;
            raw = scale * raw;
            voiceArg = voice;
            left = raw;
            if (raw < 0) {
                left = raw + 0x7F;
            }
            left >>= 7;
            right = left;

            if (right >= 0) {
                if (right >= 0x81) {
                    left = 0x80;
                }
            } else {
                left = 0;
            }

            if (right >= 0) {
                if (right >= 0x81) {
                    right = 0x80;
                }
            } else {
                right = 0;
            }

            SsUtSetVVol((s16)voiceArg, left, right);

            right = *(s16 *)toneBase;
            voiceArg = voicePacked >> 16;
            SsUtChangePitch(voiceArg, 0, right, 0x3C, 0,
                            (*(s32 *)pitchBase << 9) >> 16,
                            *(u16 *)pitchBase & 0x7F);
        } else {
            SsUtKeyOffV(voicePacked >> 16);
        }

        voicePacked += 0x10000;
        voice++;
        pitchBase += 0x14;
        toneBase += 0x14;
        offset += 0x14;
    } while (pitchBase < (s32)&g_ReverbFadeStep);
}

/* Mid-function labels the retail build exports (see docs/ASM_AND_GTE_POLICY.md,
 * "symbol labels"). They define symbols only and emit no code. */
asm(".globl func_8005E078\n"
    "func_8005E078 = ForceIndexedEffectVoiceEnabled + 0x20\n"
    ".globl func_8005E1B8\n"
    "func_8005E1B8 = ForceIndexedEffectVoiceEnabled + 0x160\n"
    ".globl func_8005E200\n"
    "func_8005E200 = ForcePitchEffectVoicesEnabled + 0x30\n"
    ".globl func_8005E314\n"
    "func_8005E314 = ForcePitchEffectVoicesEnabled + 0x144");

void ForceSoundSlotVoicePlayback(s32 enabled) {
    s32 saved = enabled;
    s32 i;
    register s32 *base asm("$18");
    s32 *active;
    s32 odd;
    s32 first;
    s32 second;
    s32 factor;
    s32 scaled;
    s32 callSlot;
    s32 callBend;
    s32 callTone;

    SetSoundSlotVoicesEnabled(enabled);

    i = 0;
    if (saved != 0) {
        base = g_SoundSlotActive;
        active = base;
        saved = 0;
        do {
            if (*base++ != 0 && g_SoundSlotTone[i][0] != g_SoundSlotTone[i][1]) {
                PlaySoundSlotVoice(i, active[-3], 3);
            }
            i++;
            saved += 4;
        } while (i < 6);

        i = 0;
        odd = 1;
        base = g_SoundSlotActive;
        active = base;
        do {
            if (*active != 0) {
                first = InterpolateAudioParameter(i * 2, base[-4], base[-3]);
                second = InterpolateAudioParameter(odd, base[-4], base[-3]);
                factor = base[6];
                scaled = second * factor;
                if (scaled < 0) {
                    scaled += 0x7F;
                }
                callSlot = i;
                callBend = first;
                scaled >>= 7;
                callTone = base[-3];
                SetSoundSlotTone(callSlot, callBend, scaled, callTone, 3);
            }
            odd += 2;
            i++;
            active++;
        } while (i < 6);
    }
}

void ForceAllEffectVoicesEnabled(s32 enabled) {
    ForcePanVoiceEnabled(enabled);
    ForceBasicEffectVoicesEnabled(enabled);
    ForceIndexedEffectVoiceEnabled(enabled);
    ForcePitchEffectVoicesEnabled(enabled);
    ForceSoundSlotVoicePlayback(enabled);
}

#include "common.h"
#include "psyq/snd.h"


s32 OpenVabSequenceSlot(s32 slot, s32 header, s32 body, s32 seq) {
    s16 vabId;
    /* $5 only matters for this second read. The first one is a cross-block
       value whose only use is SsVabTransBody's second argument, so it lands in
       a1 on its own; this one lives and dies inside one block, has no argument
       use to prefer a1, and local-alloc hands it v0. Retail keeps both in a1. */
    register s32 vabIdAgain asm("$5");

    g_AudioLoadSlot = slot;
    g_VabIds[slot] = SsVabOpenHeadSticky((u_char *)header, -1, g_VabSpuAddress[slot]);
    vabId = g_VabIds[slot];
    if (vabId == -1) {
        DebugPrintf(g_MsgSeqVabOpenHeadError);
        BiosExit(1);
    }

    g_VabIds[slot] = SsVabTransBody((u_char *)body, vabId);
    vabIdAgain = g_VabIds[slot];
    if (vabIdAgain == -1) {
        DebugPrintf(g_MsgSeqVabTransBodyError);
        BiosExit(1);
    }

    /* g_SeqHandle is a word (sym.bss size 0x4); the header calls it s16
       because every other reader wants the low half. */
    *(s32 *)&g_SeqHandle = (s16)SsSeqOpen(seq);
    g_SeqVolumeFadeStep = 0;
    g_VabTransferDone = SsVabTransCompleted(0);
    return g_VabTransferDone;
}

s32 CloseAudioSlot(s32 slot) {
    s32 *flagsPtr = &g_AudioSlotMask;
    s32 bit = 1;
    s32 flags = *flagsPtr;
    s32 ret;
    s16 *ids;

    bit <<= slot;
    if ((bit & flags) == 0) {
        ret = 0;
    } else {
        *flagsPtr = bit ^ flags;
        SsUtSetReverbDepth(0, 0);
        _SsVmInit(0);
        SsSeqCloseWrapper(g_SeqHandle);
        /* g_VabIds sits 0xC bytes past the slot mask; deriving it from flagsPtr
           (rather than naming the symbol) is what the retail code does. */
        ids = (s16 *)((u8 *)flagsPtr + 0xC);
        SsVabClose(ids[slot]);
        ret = 1;
    }
    return ret;
}

void StartVabSlotVoice(s32 voice, s32 unused, s16 vabSlot) {
    s32 voiceOffset = voice * 4;

    SsUtKeyOnV((s16)voice, g_VabIds[(s16)vabSlot], *(s16 *)((u8 *)g_VabSlotVoiceTone + voiceOffset), 0, 0x3C, 0, 0, 0);
}

void StopDirectVoice(s32 voice) {
    SsUtKeyOffV((s16)voice);
}
