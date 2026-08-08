#include "common.h"
#include "game/audio.h"
#include "game/car.h"
#include "game/menu.h"
#include "game/scratchpad.h"
#include "game/sound.h"
#include "game/state.h"
#include "psyq/gpu.h"
#include "psyq/snd.h"

void TickSequenceAudio(void) {
    if (g_SceneId == 0xC) {
        SpuVmDamperStep();
    } else {
        SsSeqCalledTbyT();
        if (g_SeqVolumeFadeStep != 0) {
            UpdateSequenceFadeOut();
        }
    }
}

extern u8 g_ReplayFrameBuffer;


s32 IsSpuTransferDone(void) {
    u8 *base;
    s32 value0;
    s32 value1;

    base = &g_ReplayFrameBuffer;
    value1 = SpuTransferStatus(base, 0);
    value1 = (value1 << 9) + (s32)base;
    value0 = *(s16 *)(value1 + 0x800);
    value1 = *(s16 *)(value1 + 0xC00);

    value0 = value0 < 0 ? -value0 : value0;
    value1 = value1 < 0 ? -value1 : value1;

    return (value0 << 16) | (s16)value1;
}

/* Reads one tone out of the 2x6 g_SoundSlotTone grid, and writes it too when
 * `tone` is not negative. Returns what was there before. */
s32 SetSoundToneTableEntry(s32 slot, s32 vabSlot, s32 tone) {
    s16 *base = &g_SoundSlotTone[0][0];
    s16 *row;
    s16 *entry;
    s32 old;

    row = (s16 *)((s32)base + (slot * 4));
    entry = (s16 *)((vabSlot * 2) + (s32)row);
    old = *entry;

    if (tone >= 0) {
        *entry = tone;
    }
    return old;
}

void LoadAudioParameterTable(u16 *table) {
    u16 *tableReg = table;
    s32 bank;
    s32 row;
    s32 col;
    s32 step;
    s32 *leftPtr;
    s32 tableValue;

    bank = 0;
    do {
        row = 0;
        do {
            col = 0;
            do {
                s32 leftValue;

                leftValue = *tableReg++;
                g_EngineSoundCurves[bank][row].left[col] = leftValue;
                g_EngineSoundCurves[bank][row].right[col] = *tableReg++;
                col++;
            } while (col < 9);
            row++;
        } while (row < 12);
        bank++;
    } while (bank < 2);

    tableValue = *tableReg;
    tableReg++;
    bank = 0;
    SetLoadedTableVolumeScale(tableValue);

    do {
        row = 0;
        do {
            s32 rowArg;

            tableValue = *tableReg;
            tableReg++;
            rowArg = row;
            row++;
            SetSoundToneTableEntry(rowArg, bank, tableValue);
        } while (row < 6);
        bank++;
    } while (bank < 2);

    step = *tableReg;
    leftPtr = &g_EngineSoundMaxRpm;
    *leftPtr = step;
    step--;
    if ((u32)step >= 0x27FF) {
        *leftPtr = 0x2800;
    }
}

void SetReverbDepth(s32 left, s32 right) {
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

    g_ReverbDepthL = left;
    g_ReverbDepthR = right;
    SsUtSetReverbDepth((s16)left, (s16)right);
}

void SetReverbPreset(s32 type, s32 left, s32 right) {
    s32 tempLeft;
    s32 tempRight;

    if (left >= 0) {
        tempLeft = left;
        if (tempLeft >= 0x80) {
            tempLeft = 0x7F;
        }
    } else {
        tempLeft = 0;
    }

    if ((left = tempLeft, right) >= 0) {
        tempRight = right;
        if (tempRight >= 0x80) {
            tempRight = 0x7F;
        }
    } else {
        tempRight = 0;
    }
    right = tempRight;

    SsUtReverbOff();

    if ((u32)(type - 1) < 9) {
        g_ReverbType = type;
        g_ReverbDepthL = left;
        g_ReverbDepthR = right;
        SsUtSetReverbType((s16)type);
        SsUtReverbOn();
        SetReverbDepth(left, right);
    } else {
        g_ReverbType = 0;
        g_ReverbDepthR = 0;
        g_ReverbDepthL = 0;
    }
}

void PlaySoundSlotVoice(s32 slot, s32 tone, s32 vabSlot) {
    s16 *base = &g_SoundSlotTone[0][0];
    s16 *row;
    s16 *entry;

    row = (s16 *)((s32)base + (slot * 4));
    entry = (s16 *)((tone * 2) + (s32)row);
    SsUtKeyOnV((s16)(slot + 0xE), g_VabIds[(s16)vabSlot], *entry, 0, 0x3C, 0, 0, 0);
}
