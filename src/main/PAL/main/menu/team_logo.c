#include "common.h"

typedef struct TeamLogoSample {
    u16 clut[2][16];
    u16 canvas[64][16];
} TeamLogoSample;

extern TeamLogoSample *g_TeamLogoSampleData asm("D_8019CA64");
extern u16 g_TeamLogoClut[] asm("D_801E444C");
extern u16 g_TeamLogoSwatches[] asm("D_801E444E");
extern u16 g_TeamLogoCanvas[] asm("D_801E6F2C");

static inline s32 TeamLogoParity(s32 value)
{
    return value & 1;
}

static inline s32 TeamLogoRowByteOffset(s32 row)
{
    return ((row * 32) + row) << 6;
}

static inline s32 TeamLogoParityByteOffset(s32 parity)
{
    return parity * 32;
}

static inline u16 *TeamLogoPaletteAddress(
    TeamLogoSample *samples, s32 row, s32 parity)
{
    u32 address;
    s32 parityOffset;

    address = TeamLogoRowByteOffset(row);
    address += (u32)samples;
    parityOffset = TeamLogoParityByteOffset(parity);
    parityOffset += address;
    return (u16 *)parityOffset;
}

static inline u16 *TeamLogoClutAddress(
    TeamLogoSample *samples, s32 row, s32 parity, s32 index)
{
    s32 byteOffset;
    u32 address;
    s32 parityOffset;

    byteOffset = index * 2;
    address = TeamLogoRowByteOffset(row);
    address += (u32)samples;
    parityOffset = TeamLogoParityByteOffset(parity);
    parityOffset += address;
    byteOffset += parityOffset;
    return (u16 *)byteOffset;
}

/* Builds g_TeamLogoCanvas and its CLUT from one sample character and one sample background. */
void ComposeSampleTeamLogo(s32 arg0, s32 arg1) asm("func_8001D338");
void ComposeSampleTeamLogo(s32 arg0, s32 arg1)
{
    s32 index;
    u16 *clutDst0;
    u16 *clutDst1;
    u16 *dst;
    u16 *src;
    u16 *src0;
    s32 row0;
    s32 row1;
    s32 adjusted;
    s32 outer;
    s32 j;
    u16 value;
    u16 fill;

    adjusted = arg1 + ((u32)arg1 >> 31);
    row1 = (adjusted >> 1) + 10;
    arg1 &= 1;
    index = 1;
    clutDst0 = g_TeamLogoSwatches;
    row0 = arg0 / 2;
    arg0 = TeamLogoParity(arg0);
    src = TeamLogoPaletteAddress(g_TeamLogoSampleData, row0, arg0) + 1;

    do {
        *clutDst0++ = *src++;
        index++;
    } while (index < 12);

    if (index < 16) {
        clutDst1 = &g_TeamLogoClut[index];
        src = TeamLogoClutAddress(
            g_TeamLogoSampleData, row1, arg1, index);

        do {
            *clutDst1++ = *src++;
            index++;
        } while (index < 16);
    }

    dst = g_TeamLogoCanvas;
    outer = 0;
    src0 = &g_TeamLogoSampleData[row0].canvas[0][0];
    src = &g_TeamLogoSampleData[row1].canvas[0][0];

    for (; outer < 64; outer++) {
        for (j = 0; j < 16; j++) {
            value = *src0;

            if ((value & 0x000F) == 0) {
                fill = *src & 0x000F;
                value |= fill;
            }
            if ((value & 0x00F0) == 0) {
                fill = *src & 0x00F0;
                value |= fill;
            }
            if ((value & 0x0F00) == 0) {
                fill = *src & 0x0F00;
                value |= fill;
            }
            if ((value & 0xF000) == 0) {
                fill = *src & 0xF000;
                value |= fill;
            }
            src0++;
            *dst++ = value;
            src++;
        }
    }
}

#include "psyq/gpu.h"

extern u8 D_8007D24C;
void LoadImage(Rect *rect, void *data) asm("func_80065B24");

void ClearTeamNameTexture(void) asm("func_8001D4E8");
void ClearTeamNameTexture(void) {
    s16 sp[4];
    sp[0] = 0x282;
    sp[1] = 0x37;
    sp[2] = 0xC;
    sp[3] = 8;
    LoadImage((Rect *)sp, &D_8007D24C);
}

extern u8 D_8007C7CC[];
void ClearTeamNameTexture(void) asm("func_8001D4E8");
void LoadImage(Rect *rect, void *data) asm("func_80065B24");

void UploadTeamNameTexture(u8 *str, s32 len) asm("func_8001D530");
void UploadTeamNameTexture(u8 *str, s32 len) {
    s16 sp[4];
    ClearTeamNameTexture();
    sp[0] = 0x288 - len;
    sp[1] = 0x37;
    sp[2] = 2;
    sp[3] = 8;
    while (len > 0) {
        LoadImage((Rect *)sp, &D_8007C7CC[*str++ << 5]);
        sp[0] += 2;
        len--;
    }
}
