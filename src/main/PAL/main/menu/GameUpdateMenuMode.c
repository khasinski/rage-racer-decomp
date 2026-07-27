#include "common.h"
#include "game/state.h"
#include "game/menu.h"
#include "psyq/gpu.h"
#include "game/audio.h"
#include "psyq/snd.h"

extern s32 D_8009B348;
extern s32 D_8009B324;
extern s32 g_CarTable asm("D_8019C7C8");
extern s32 g_PlayerCarIndex asm("D_801E40D4");
extern s32 g_CarListCursor asm("D_801E4B88");

struct Entry_5ACA0 {
    u8 f0;
    u8 f1;
    u8 pad[6];
};
extern s32 g_MenuHintBarStep asm("D_8009B30C");
extern u8 g_MenuHintBarScript asm("D_80082A90");
extern s32 g_MenuHintBarProgress asm("D_8009B2FC");
extern s32 g_MenuHintButtonsVisible asm("D_8009B314");
extern u8 g_PadType asm("D_801E4369");


void func_80047024(void *, s32, s32, s32, s32, s32, s32, s32, s32);
s32 func_800487D8(void *, void *);
void func_800496F0(s32, s32);
void func_80046A2C(void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
void func_80047E60(s32);

/* Scene 8: the menu-mode per-frame dispatcher. Runs the two
 * g_MenuScreenDraw overlay slots and then g_MenuScreenUpdate[g_MenuScreen]. */
void GameUpdateMenuMode(void) asm("func_8005ACA0");
void GameUpdateMenuMode(void) {
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
    func_80047024(scratch, 0, 0, 0x140, 2, 0, 0, 0, 0xFF);

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

    func_800496F0(D_8009B324, ((struct Entry_5ACA0 *)g_CarTable)[(g_MenuScreen == 0xB) ? g_CarListCursor : g_PlayerCarIndex].f1);

    {
        /* This pin is load-bearing: removing it changes .text. */
        register s32 flag asm("$6");
        flag = g_MenuHintBarStep;
        if (flag == 0) {
            return;
        }
    }
    if (func_800487D8(&g_MenuHintBarScript, &g_MenuHintBarProgress) == 0) {
        return;
    }

    if (g_MenuHintButtonsVisible != 0) {
        if (g_PadType == 0x23) {
            scratch = (void *)((s32)scratch + 4);
            func_80046A2C(scratch, 0xC0, 0x1A1, 0x20, 0xC, 0x94, 0xF4, 0, 0, 0, 0x244, 1, 1, 0x3B);
            func_80046A2C(scratch, 0xF0, 0x1A1, 0x2C, 0xC, 0xB4, 0xF4, 0, 0, 0, 0x244, 1, 1, 0x3B);
        } else {
            scratch = (void *)((s32)scratch + 4);
            func_80046A2C(scratch, 0xC0, 0x1A1, 0x20, 0xC, 0x94, 0xE8, 0, 0, 0, 0x244, 1, 1, 0x3B);
            func_80046A2C(scratch, 0xF0, 0x1A1, 0x2C, 0xC, 0xB4, 0xE8, 0, 0, 0, 0x244, 1, 1, 0x3B);
        }
    }
    func_80047E60(g_MenuOverlayPattern);
}

extern s32 g_SeqVolumeFadeStep asm("D_801E6D9C");
void func_800731CC(void);
void SsSeqCalledTbyT(void) asm("func_80071018");
void func_8005E900(void);
void func_8005AF78(void) {
    if (g_SceneId == 0xC) {
        func_800731CC();
    } else {
        SsSeqCalledTbyT();
        if (g_SeqVolumeFadeStep != 0) {
            func_8005E900();
        }
    }
}

extern u8 D_801E8AFC;

s32 func_8007B014(void *arg0, s32 arg1);

s32 func_8005AFD4(void) {
    u8 *base;
    s32 value0;
    s32 value1;

    base = &D_801E8AFC;
    value1 = func_8007B014(base, 0);
    value1 = (value1 << 9) + (s32)base;
    value0 = *(s16 *)(value1 + 0x800);
    value1 = *(s16 *)(value1 + 0xC00);

    value0 = value0 < 0 ? -value0 : value0;
    value1 = value1 < 0 ? -value1 : value1;

    return (value0 << 16) | (s16)value1;
}

extern s16 g_SoundSlotTone[] asm("D_80082F28");

s32 GameSetSoundToneTableEntry(s32 arg0, s32 arg1, s32 arg2) {
    s16 *base = g_SoundSlotTone;
    s16 *row;
    s16 *entry;
    s32 old;

    row = (s16 *)((s32)base + (arg0 << 2));
    entry = (s16 *)((arg1 << 1) + (s32)row);
    old = *entry;

    if (arg2 >= 0) {
        *entry = arg2;
    }
    return old;
}

extern s32 g_ReverbDepthL asm("D_801E6D84");
extern s32 g_ReverbDepthR asm("D_801E6D88");
extern s32 D_801E6D80;
extern s32 g_EngineSoundCurves[] asm("D_801E446C");
extern s32 g_EngineSoundMaxRpm asm("D_801E6CC4");

void GameSetLoadedTableVolumeScale(s32 scale) asm("func_8005BD58");
s32 GameSetSoundToneTableEntry(s32 row, s32 bank, s32 value) asm("func_8005B040");
void GamePlaySoundSlotVoice(s32 slot, s32 tone, s32 vabSlot) asm("func_8005B2F0");

void GameLoadAudioParameterTable(u16 *table) asm("func_8005B070");
void GameLoadAudioParameterTable(u16 *table) {
    u16 *tableReg = table;
    s32 bank;
    s32 row;
    s32 col;
    s32 bankOffset;
    s32 rowOffset;
    s32 rowBaseOffset;
    s32 rowBasePtr;
    s32 colOffset;
    s32 *base = g_EngineSoundCurves;
    s32 *secondBase;
    s32 *leftPtr;
    s32 *rightPtr;
    s32 step;

    bank = 0;
    secondBase = base + 9;
    bankOffset = 0;
    do {
        row = 0;
        rowOffset = bankOffset;
        do {
            col = 0;
            rowBaseOffset = rowOffset;
            rightPtr = (s32 *)((s32)rowOffset + (s32)secondBase);
            do {
                s32 leftValue;

                leftValue = *tableReg++;
                colOffset = col << 2;
                col++;
                asm volatile("" : "=r"(col) : "0"(col));
                asm volatile("addu %0,%1,%2" : "=r"(rowBasePtr) : "r"(rowBaseOffset), "r"(base), "r"(colOffset));
                asm volatile("addu %0,%0,%1" : "=r"(colOffset) : "r"(rowBasePtr), "0"(colOffset));
                leftPtr = (s32 *)colOffset;
                *leftPtr = leftValue;
                *rightPtr = *tableReg++;
                rightPtr++;
            } while (col < 9);
            row++;
            rowOffset += 0x48;
        } while (row < 12);
        bank++;
        bankOffset += 0x360;
    } while (bank < 2);

    {
        /* This pin is load-bearing: removing it changes .text. */
        register s32 scale asm("$6");

        scale = *tableReg;
        tableReg++;
        asm volatile("" : "=r"(scale) : "0"(scale));
        bank = 0;
        GameSetLoadedTableVolumeScale(scale);
    }

    do {
        row = 0;
        do {
            s32 rowArg;
            s32 tone;

            tone = *tableReg;
            tableReg++;
            asm volatile("" : "=r"(tone) : "0"(tone));
            rowArg = row;
            row++;
            asm volatile("" : "=r"(row) : "0"(row));
            GameSetSoundToneTableEntry(rowArg, bank, tone);
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

void GameSetReverbDepth(s32 left, s32 right) asm("func_8005B190");
void GameSetReverbDepth(s32 left, s32 right) {
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

void GameSetReverbPreset(s32 type, s32 left, s32 right) asm("func_8005B204");
void GameSetReverbPreset(s32 type, s32 left, s32 right) {
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
        GameSetReverbDepth(left, right);
    } else {
        D_801E6D80 = 0;
        g_ReverbDepthR = 0;
        g_ReverbDepthL = 0;
    }
}

void GamePlaySoundSlotVoice(s32 slot, s32 tone, s32 vabSlot) {
    s16 *base = g_SoundSlotTone;
    s16 *row;
    s16 *entry;

    row = (s16 *)((s32)base + (slot << 2));
    entry = (s16 *)((tone << 1) + (s32)row);
    SsUtKeyOnV((s16)(slot + 0xE), g_VabIds[(s16)vabSlot], *entry, 0, 0x3C, 0, 0, 0);
}
