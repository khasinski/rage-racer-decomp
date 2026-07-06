#include "common.h"
#include "psyq/snd.h"

extern s32 D_801E6D84;
extern s32 D_801E6D88;
extern s32 D_801E6D80;
extern s32 D_801E446C[];
extern s32 D_801E6CC4;
extern s16 D_801E6CA8[];
extern s16 D_80082F28[];

void func_80077C7C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);
void GameSetLoadedTableVolumeScale(s32 scale) asm("func_8005BD58");
s32 GameSetSoundToneTableEntry(s32 row, s32 bank, s32 value) asm("func_8005B040");
void GamePlaySoundSlotVoice(s32 slot, s32 tone, s32 vabSlot) asm("func_8005B2F0");

void func_8005B070(u16 *table) {
    register u16 *tableReg asm("$16") = table;
    register s32 bank asm("$18");
    register s32 row asm("$17");
    register s32 col asm("$5");
    register s32 bankOffset asm("$9");
    register s32 rowOffset asm("$7");
    register s32 rowBaseOffset asm("$8");
    register s32 rowBasePtr asm("$2");
    register s32 colOffset asm("$3");
    register s32 *base asm("$10") = D_801E446C;
    register s32 *secondBase asm("$11");
    register s32 *leftPtr asm("$3");
    register s32 *rightPtr asm("$6");
    register s32 step asm("$2");

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
                register s32 leftValue asm("$4");

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
            register s32 rowArg asm("$4");
            register s32 tone asm("$6");

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
    leftPtr = &D_801E6CC4;
    *leftPtr = step;
    step--;
    if ((u32)step >= 0x27FF) {
        *leftPtr = 0x2800;
    }
}

void func_8005B190(s32 left, s32 right) {
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

    D_801E6D84 = left;
    D_801E6D88 = right;
    SsUtSetReverbDepth((s16)left, (s16)right);
}

void func_8005B204(s32 type, s32 left, s32 right) {
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
        D_801E6D84 = left;
        D_801E6D88 = right;
        SsUtSetReverbType((s16)type);
        SsUtReverbOn();
        func_8005B190(left, right);
    } else {
        D_801E6D80 = 0;
        D_801E6D88 = 0;
        D_801E6D84 = 0;
    }
}

void GamePlaySoundSlotVoice(s32 slot, s32 tone, s32 vabSlot) {
    register s16 *base asm("$3") = D_80082F28;
    register s16 *row asm("$2");
    register s16 *entry asm("$3");

    row = (s16 *)((s32)base + (slot << 2));
    entry = (s16 *)((tone << 1) + (s32)row);
    func_80077C7C((s16)(slot + 0xE), D_801E6CA8[(s16)vabSlot], *entry, 0, 0x3C, 0, 0, 0);
}
