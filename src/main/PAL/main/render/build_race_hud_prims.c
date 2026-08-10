#include "common.h"
#include "game/race.h"
#include "game/render.h"


void BuildRaceHudPrims(s32 mode) {
    u8 *cursor;
    s32 col;
    s32 row;
    RenderBufferAddress rowOffset;

    if (mode != 0) {
        s32 bufferOffset;

        row = 0;
        rowOffset.value = 0;

nonzero_outer:
        col = 0;
        cursor = g_RaceHudSprite11U0;
        bufferOffset = 0;
nonzero_inner:
        {
            u8 *dst;
            s32 offset;
            RenderBufferAddress spriteAddress;
            RenderBufferAddress baseAddress;
            GameSpriteDescAddress descAddress;

            baseAddress.drawPacket = g_TachoNeedlePrim0PageA;
            dst = baseAddress.bytes;
            dst = bufferOffset + dst;
            offset = rowOffset.value + 0x2C;
            dst += offset;
            spriteAddress.bytes = dst;
            descAddress.descriptors = g_RaceHudSpriteDescsGp;
            descAddress.bytes += rowOffset.value;
            BuildSpriteFromDesc(spriteAddress.sprite, descAddress.descriptors);
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
        rowOffset.value += 0x14;
        if (row < 0xC) {
            goto nonzero_outer;
        }
    } else {
        s32 bufferOffset;

        row = 0;
        rowOffset.drawPacket = g_TachoNeedlePrim0PageA;
        bufferOffset = 0;

zero_outer:
        col = 0;
        cursor = rowOffset.bytes;
zero_inner:
        {
            RenderBufferAddress spriteAddress;
            GameSpriteDescAddress descAddress;

            spriteAddress.bytes = cursor + (bufferOffset + 0x2C);
            descAddress.descriptors = g_RaceHudSpriteDescsTimeTrial;
            descAddress.bytes += bufferOffset;
            BuildSpriteFromDesc(spriteAddress.sprite, descAddress.descriptors);
        }
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
