#include "common.h"
#include "game/race.h"
#include "game/render.h"


void BuildRaceHudPrims(s32 mode) {
    u8 *cursor;
    s32 col;
    s32 row;
    s32 rowOffset;

    if (mode != 0) {
        s32 bufferOffset;

        row = 0;
        rowOffset = 0;

nonzero_outer:
        col = 0;
        cursor = g_RaceHudSprite11U0;
        bufferOffset = 0;
nonzero_inner:
        {
            u8 *dst;
            s32 offset;

            dst = g_TachoNeedlePrim0PageA;
            dst = bufferOffset + dst;
            offset = rowOffset + 0x2C;
            dst += offset;
            BuildSpriteFromDesc(dst, g_RaceHudSpriteDescsGp + rowOffset);
        }
        if (g_GrandPrixClass == 5 && row == 0xB) {
            cursor[0] += 0xE8;
        }
        cursor += 0x237E8;
        bufferOffset += 0x237E8;
        col++;
        if (col < 2) {
            goto nonzero_inner;
        }
        row++;
        rowOffset += 0x14;
        if (row < 0xC) {
            goto nonzero_outer;
        }
    } else {
        s32 bufferOffset;

        row = 0;
        rowOffset = (s32)g_TachoNeedlePrim0PageA;
        bufferOffset = 0;

zero_outer:
        col = 0;
        cursor = (u8 *)rowOffset;
zero_inner:
        BuildSpriteFromDesc(cursor + (bufferOffset + 0x2C), g_RaceHudSpriteDescsTimeTrial + bufferOffset);
        cursor += 0x237E8;
        col++;
        if (col < 2) {
            goto zero_inner;
        }
        row++;
        bufferOffset += 0x14;
        if (row < 0xB) {
            goto zero_outer;
        }
    }
}
