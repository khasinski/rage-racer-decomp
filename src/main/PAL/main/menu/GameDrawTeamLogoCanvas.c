#include "common.h"

extern s32 D_8009B280;
extern u8 D_8009B284;

void func_80047024(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);

void func_8004A17C(s32 delta) {
    u8 *scratch;
    s32 value;
    s32 sum;
    s32 limit;
    s32 alpha;

    scratch = *(u8 **)0x1F800004;
    if (delta > 0) {
        value = D_8009B280;
        sum = delta + value;
        value = sum;
        D_8009B280 = value;
        if (0xFFFF < value) {
            D_8009B280 = 0xFFFF;
        }
    } else {
        value = D_8009B280;
        sum = delta + value;
        value = sum;
        D_8009B280 = value;
        if (value < 0) {
            D_8009B280 = 0;
        }
    }

    limit = 0x1E0;
    D_8009B284 = D_8009B280 >> 8;
    alpha = D_8009B284;
    func_80047024(scratch + 0x18, 0x48, 0, 0xF8, limit, alpha, alpha, alpha, 0x40);
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/menu/GameDrawTeamLogoCanvas", func_8004A248);

extern s32 D_8007F948;
extern s32 D_8009B298;
extern s32 D_8009B29C;

void GameRampTeamLogoCanvas(s32 arg0, s32 arg1) asm("func_8004B8B4");
void GameRampTeamLogoCanvas(s32 arg0, s32 arg1) {
    s32 temp;
    s32 y;

    if (arg0 > 0) {
        temp = arg0 + D_8009B298;
        D_8009B298 = temp;
        if (temp >= 0x101) {
            D_8009B298 = 0x100;
        }
    } else {
        temp = arg0 + D_8009B298;
        D_8009B298 = temp;
        if (temp < 0x40) {
            D_8009B298 = 0x40;
        }
    }

    if (arg1 > 0) {
        temp = arg1 + D_8009B29C;
        D_8009B29C = temp;
        if (temp >= 0x101) {
            D_8009B29C = 0x100;
        }
    } else {
        temp = arg1 + D_8009B29C;
        D_8009B29C = temp;
        if (temp < 0) {
            D_8009B29C = 0;
        }
    }

    y = D_8009B29C;
    temp = ((y << 4) + y) << 4;
    if (temp < 0) {
        temp += 0xFF;
    }
    D_8007F948 = 0x220 - (temp >> 8);
}

extern u32 g_TeamLogoCanvas[] asm("D_801E6F2C");

void GamePlaySoundCue(s32 cue) asm("func_8005D6EC");

void GameScrollTeamLogoUp(void) asm("func_8004B9B8");
void GameScrollTeamLogoUp(void) {
    register s32 i asm("$4");
    register u32 *stackPtr asm("$5");
    register u32 *base asm("$6");
    register u32 *cursor asm("$3");
    u32 saved[8];
    register u32 value asm("$2");

    GamePlaySoundCue(1);

    i = 0;
    stackPtr = saved;
    base = g_TeamLogoCanvas;
    cursor = base;
    do {
        value = *cursor;
        cursor++;
        i++;
        *stackPtr = value;
        stackPtr++;
    } while (i < 8);

    i = 0;
    cursor = base;
    do {
        value = cursor[8];
        i++;
        *cursor = value;
        cursor++;
    } while (i < 0x1F8);

    i = 0;
    stackPtr = saved;
    cursor = base;
    do {
        value = *stackPtr;
        stackPtr++;
        i++;
        cursor[0x1F8] = value;
        cursor++;
    } while (i < 8);
}

void GameScrollTeamLogoDown(void) asm("func_8004BA50");
void GameScrollTeamLogoDown(void) {
    s32 i;
    u32 *newPtr;
    u32 *stackPtr;
    u32 *base;
    u32 *cursor;
    u32 saved[8];
    u32 value;

    GamePlaySoundCue(1);

    i = 0;
    stackPtr = saved;
    base = g_TeamLogoCanvas;
    cursor = base;
    do {
        value = cursor[0x1F8];
        cursor++;
        i++;
        *stackPtr = value;
        stackPtr++;
    } while (i < 8);

    i = 0x1F7;
    newPtr = base + 0x1F7;
    cursor = newPtr;
    do {
        value = *cursor;
        i--;
        cursor[8] = value;
        cursor--;
    } while (i >= 0);

    i = 0;
    stackPtr = base;
    cursor = saved;
    do {
        value = *cursor;
        cursor++;
        i++;
        newPtr = stackPtr;
        *newPtr = value;
        stackPtr++;
    } while (8 > i);
}

void GameScrollTeamLogoLeft(void) asm("func_8004BAE4");
void GameScrollTeamLogoLeft(void) {
    register s32 row asm("$7");
    register u32 *savePtr asm("$4");
    register u32 *savePtr2 asm("$10");
    register u32 *rowBase asm("$8");
    register s32 offset asm("$9");
    register s32 col asm("$6");
    register u32 *base asm("$11");
    register u32 *base2 asm("$5");
    register u32 *addr asm("$4");
    register u32 *cursor asm("$3");
    u32 saved[64];
    register u32 value asm("$2");
    register u32 next asm("$3");

    GamePlaySoundCue(1);

    row = 0;
    savePtr = saved;
    base = g_TeamLogoCanvas;
    cursor = base;
    do {
        value = *cursor;
        cursor += 8;
        row++;
        value <<= 28;
        *savePtr = value;
        savePtr++;
    } while (row < 0x40);

    row = 0;
    savePtr2 = saved;
    rowBase = base;
    offset = 0;
    do {
        col = 0;
        base2 = base;
        do {
            addr = (u32 *)(offset + (s32)base2);
            base2++;
            value = addr[0];
            next = addr[1];
            value >>= 4;
            next <<= 28;
            value |= next;
            addr[0] = value;
            col++;
        } while (col < 7);

        value = *savePtr2;
        savePtr2++;
        offset += 0x20;
        next = rowBase[7];
        row++;
        next >>= 4;
        next |= value;
        rowBase[7] = next;
        rowBase += 8;
    } while (row < 0x40);
}

void GameScrollTeamLogoRight(void) asm("func_8004BBA8");
void GameScrollTeamLogoRight(void) {
    register s32 row asm("$7");
    register u32 *savePtr asm("$4");
    register u32 *savePtr2 asm("$10");
    register u32 *rowBase asm("$8");
    register s32 offset asm("$9");
    register s32 col asm("$6");
    register u32 *base asm("$11");
    register u32 *base2 asm("$5");
    register u32 *addr asm("$2");
    register u32 *cursor asm("$3");
    u32 saved[64];
    register u32 savedNibble asm("$2");
    register u32 value asm("$3");
    register u32 prev asm("$4");

    GamePlaySoundCue(1);

    row = 0;
    savePtr = saved;
    base = g_TeamLogoCanvas;
    cursor = base;
    do {
        savedNibble = cursor[7];
        cursor += 8;
        row++;
        savedNibble >>= 28;
        *savePtr = savedNibble;
        savePtr++;
    } while (row < 0x40);

    row = 0;
    rowBase = base;
    savePtr2 = saved;
    offset = 0;
    do {
        col = 7;
        base2 = base + 7;
        do {
            addr = (u32 *)(offset + (s32)base2);
            base2--;
            col--;
            value = addr[0];
            prev = addr[-1];
            value <<= 4;
            prev >>= 28;
            value |= prev;
            addr[0] = value;
        } while (col > 0);

        savedNibble = *savePtr2;
        savePtr2++;
        offset += 0x20;
        value = rowBase[0];
        row++;
        value <<= 4;
        value |= savedNibble;
        rowBase[0] = value;
        rowBase += 8;
    } while (row < 0x40);
}

void GameFlipTeamLogoVertical(void) asm("func_8004BC68");
void GameFlipTeamLogoVertical(void) {
    s32 i;
    s32 j;
    s32 mirror;
    u32 *base;

    GamePlaySoundCue(8);
    base = g_TeamLogoCanvas;
    i = 0;
    mirror = 0x3F;
    do {
        u8 *cursor;
        s32 leftOffset;
        s32 rightOffset;

        j = 0;
        leftOffset = i << 5;
        rightOffset = (mirror - i) << 5;
        cursor = (u8 *)base;
        do {
            u32 temp;
            u32 *left;
            u32 *right;

            left = (u32 *)(leftOffset + (s32)cursor);
            right = (u32 *)(rightOffset + (s32)cursor);
            cursor += 4;
            temp = *left;
            *left = *right;
            j++;
            *right = temp;
        } while (j < 8);
        i++;
    } while (i < 0x20);
}
