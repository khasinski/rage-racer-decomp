#include "common.h"
#include "game/state.h"
#include "game/menu.h"
#include "psyq/gpu.h"
#include "game/audio.h"
#include "psyq/snd.h"
#include "game/car.h"
#include "game/sound.h"

struct Entry_5ACA0 {
    u8 f0;
    u8 f1;
    u8 pad[6];
};
extern u8 g_PadType;

void GameDrawSolidRectWide(void *, s32, s32, s32, s32, s32, s32, s32, s32) asm("DrawSolidRect");
s32 RunTimedDrawScript(void *, void *);
void GameDrawSpriteWide(void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32) asm("DrawSprite");
void DrawBitPatternOverlay(s32);

/* Scene 8: the menu-mode per-frame dispatcher. Runs the two
 * g_MenuScreenDraw overlay slots and then g_MenuScreenUpdate[g_MenuScreen]. */
void UpdateMenuMode(void) {
    void *scratch;
    s32 c0;
    s32 c1;

    c0 = g_AnimTimer;
    c1 = g_SceneTimer;
    scratch = *(void **)0x1F800004;
    c0 += 1;
    c1 += 1;
    g_AnimTimer = c0;
    g_SceneTimer = c1;
    if (c1 == 2) {
        SetDispMask(1);
    }
    GameDrawSolidRectWide(scratch, 0, 0, 0x140, 2, 0, 0, 0, 0xFF);

    if ((u32)(g_MenuScreen - 1) < 2) {
        *(s32 *)0x1F800064 = 1;
    } else {
        *(s32 *)0x1F800064 = 5;
    }

    if (g_MenuHandlerIndex > 0) {
        g_MenuScreenDraw[g_MenuHandlerIndex](0x14);
    }
    if (g_MenuHandlerIndex2 > 0) {
        D_8009B348 = g_MenuScreenDraw[g_MenuHandlerIndex2](-10);
    }
    g_MenuScreenUpdate[g_MenuScreen]();

    DrawCarSpecGraph(D_8009B324, ((struct Entry_5ACA0 *)g_CarTable)[(g_MenuScreen == 0xB) ? g_CarListCursor : g_PlayerCarIndex].f1);

    {
        register s32 flag asm("$6");
        flag = g_MenuHintBarStep;
        if (flag == 0) {
            return;
        }
    }
    if (RunTimedDrawScript(&g_MenuHintBarScript, &g_MenuHintBarProgress) == 0) {
        return;
    }

    if (g_MenuHintButtonsVisible != 0) {
        if (g_PadType == 0x23) {
            scratch = (void *)((u8 *)scratch + 4);
            GameDrawSpriteWide(scratch, 0xC0, 0x1A1, 0x20, 0xC, 0x94, 0xF4, 0, 0, 0, 0x244, 1, 1, 0x3B);
            GameDrawSpriteWide(scratch, 0xF0, 0x1A1, 0x2C, 0xC, 0xB4, 0xF4, 0, 0, 0, 0x244, 1, 1, 0x3B);
        } else {
            scratch = (void *)((u8 *)scratch + 4);
            GameDrawSpriteWide(scratch, 0xC0, 0x1A1, 0x20, 0xC, 0x94, 0xE8, 0, 0, 0, 0x244, 1, 1, 0x3B);
            GameDrawSpriteWide(scratch, 0xF0, 0x1A1, 0x2C, 0xC, 0xB4, 0xE8, 0, 0, 0, 0x244, 1, 1, 0x3B);
        }
    }
    DrawBitPatternOverlay(g_MenuOverlayPattern);
}

void SpuVmDamperStep(void);
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

extern u8 D_801E8AFC;

s32 SpuTransferStatus(void *arg0, s32 arg1);

s32 IsSpuTransferDone(void) {
    u8 *base;
    s32 value0;
    s32 value1;

    base = &D_801E8AFC;
    value1 = SpuTransferStatus(base, 0);
    value1 = (value1 << 9) + (u8 *)base;
    value0 = *(s16 *)(value1 + 0x800);
    value1 = *(s16 *)(value1 + 0xC00);

    value0 = value0 < 0 ? -value0 : value0;
    value1 = value1 < 0 ? -value1 : value1;

    return (value0 << 16) | (s16)value1;
}

s32 SetSoundToneTableEntry(s32 arg0, s32 arg1, s32 arg2) {
    s16 *base = g_SoundSlotTone;
    s16 *row;
    s16 *entry;
    s32 old;

    row = (s16 *)((s32)base + (arg0 * 4));
    entry = (s16 *)((arg1 * 2) + (s32)row);
    old = *entry;

    if (arg2 >= 0) {
        *entry = arg2;
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

    {
        register s32 scale asm("$6");

        scale = *tableReg;
        tableReg++;
        bank = 0;
        SetLoadedTableVolumeScale(scale);
    }

    do {
        row = 0;
        do {
            s32 rowArg;
            s32 tone;

            tone = *tableReg;
            tableReg++;
            rowArg = row;
            row++;
            SetSoundToneTableEntry(rowArg, bank, tone);
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
        D_801E6D80 = type;
        g_ReverbDepthL = left;
        g_ReverbDepthR = right;
        SsUtSetReverbType((s16)type);
        SsUtReverbOn();
        SetReverbDepth(left, right);
    } else {
        D_801E6D80 = 0;
        g_ReverbDepthR = 0;
        g_ReverbDepthL = 0;
    }
}

void PlaySoundSlotVoice(s32 slot, s32 tone, s32 vabSlot) {
    s16 *base = g_SoundSlotTone;
    s16 *row;
    s16 *entry;

    row = (s16 *)((s32)base + (slot * 4));
    entry = (s16 *)((tone * 2) + (s32)row);
    SsUtKeyOnV((s16)(slot + 0xE), g_VabIds[(s16)vabSlot], *entry, 0, 0x3C, 0, 0, 0);
}
