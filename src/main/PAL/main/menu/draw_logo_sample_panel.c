#include "common.h"
void DrawRectOutline(void *ot, s32 x0, s32 y0, s32 x1, s32 y1, s32 arg5, s32 color, s32 arg7,
                   s32 arg8);

extern s32 D_8007FB24;
extern s32 D_8007FB28;
extern s32 D_8009B28C;
extern u8 g_TeamNameLength;
extern u8 g_TeamNameChars[];
extern u16 g_TeamLogoSwatches[];

void DrawSprite(void *ot, s16 x0, s16 y0, s16 x1, u16 y1, u8 u0, u8 v0, u8 r, u8 g,
                   u8 b, u16 clutX, s32 shadeTex, s32 semiTrans, u32 flags);
void DrawSolidRect(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u8 r, u8 g, u8 b, u8 alpha);
s32 rsin(s32 angle);

void DrawLogoSprite() asm("DrawSprite");
void DrawLogoRect() asm("DrawSolidRect");

void DrawLogoSamplePanel(s32 arg0, s32 arg1);
void DrawLogoSamplePanel(s32 arg0, s32 arg1) {
    void *ot = *(void **)0x1F800004;
    s32 idx;
    u32 t;
    s16 y;
    s32 i;
    s32 xoff;
    s32 xc = 0xA2;

    if (arg0 == 0) {
        D_8007FB24 = 0;
        return;
    }
    if (arg0 < 0) {
        D_8007FB24 += arg0;
        if (D_8007FB24 < 0) {
            D_8007FB24 = 0;
        }
    }
    idx = D_8007FB24;
    if (idx >= 0) {
        if (idx >= 6) {
            idx = 5;
        }
        t = (u32) - (idx * 960) >> 5;
        y = t + 494;
        DrawSprite(ot, 0xDA, y, 8, 0x10, (arg1 / 10) << 3, 0x18, 0, 0, 0, 0x244, 1, 1, 0x3B);
        DrawSprite(ot, 0xE2, y, 8, 0x10, (arg1 % 10) << 3, 0x18, 0, 0, 0, 0x244, 1, 1, 0x3B);
        DrawSprite(ot, 0xA2, y, 4, 0x10, 0x78, 0xCC, 0, 0, 0, 0x244, 1, 1, 0x3A);
        DrawSprite(ot, 0xA8, y, 0x34, 0x10, 0x7C, 0xCC, 0, 0, 0, 0x244, 1, 1, 0x3A);
        DrawSprite(ot, 0xEA, y, 4, 0x10, 0xB0, 0xCC, 0, 0, 0, 0x244, 1, 1, 0x3A);

        for (i = 0, xoff = -23; i < 15; i++) {
            DrawSolidRect(ot, (s16)(xc + xoff), (s16)(t + 528), 8, 0x10,
                          *(u8 *)&g_TeamLogoSwatches[i] << 3, (g_TeamLogoSwatches[i] >> 2) & 0xf8,
                          (g_TeamLogoSwatches[i] >> 7) & 0xf8, 0xFF);
            xoff += 8;
        }

        DrawRectOutline(ot, (s16)(xc - 24), (s16)((u16)y + 32), 0x7A, 0x14, 0xB4, 0xB4, 0xB4, 0xFF);
    }
    if (arg0 > 0) {
        D_8007FB24 += arg0;
        if (D_8007FB24 >= 6) {
            D_8007FB24 = 5;
        }
    }
}

void DrawTeamNameEntry(s32 arg0, s32 arg1) {
    s16 temp_a2;
    s16 temp_s2;
    s16 var_a2;
    s16 var_s1;
    s32 temp_s7;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 var_a0;
    s32 i;
    s32 rowI;
    u16 rowX;
    u16 xFirst;
    u16 xSecond;
    s32 animationStep;
    s32 var_v0;
    s32 colour;
    u32 temp_s0;
    u8 temp_v1;

    temp_s7 = *(s32 *)0x1F800004;
    if (arg0 == 0) {
        D_8007FB28 = 0;
        return;
    }
    if (arg0 < 0) {
        temp_v0 = arg0 + D_8007FB28;
        D_8007FB28 = temp_v0;
        if (temp_v0 < 0) {
            D_8007FB28 = 0;
        }
    }
    if ((D_8007FB28 >= 0x19) && ((u8)g_TeamNameLength < 6U)) {
        DrawLogoSprite(temp_s7, (g_TeamNameLength * 0xC) + 0x53, 0x7D, 0xC, 0x18, 0xF4, 0x28, 0,
                       0, 0, 0x244, 1, 1, 0x39);
    }
    animationStep = D_8007FB28 - 0xE;
    if (animationStep >= 0) {
        if (animationStep >= 0xC) {
            animationStep = 0xB;
        }
        var_s1 = ((u32)-(animationStep * 64) >> 5) + 0xFB;
        var_a0 = D_8009B28C;
        if (D_8009B28C < 0) {
            var_a0 = D_8009B28C + 0xFFF;
        }
        var_v0 = rsin(D_8009B28C - ((var_a0 >> 0xC) << 0xC));
        if (var_v0 < 0) {
            var_v0 += 0x3F;
        }
        colour = var_v0;
        colour >>= 6;
        colour -= 0x41;
        DrawLogoRect(temp_s7 + 4, (s16)(((arg1 % 11) * 0xC) + 0x54),
                     (s16)(var_s1 + ((arg1 / 11) * 0x18)), 0xB,
                     (s32)(s16)(animationStep * 2), 0, colour & 0xFF, 0, 0xFF);
        D_8009B28C += 0x60;
    }
    animationStep = D_8007FB28 - 0x11;
    if (animationStep >= 0) {
        if (animationStep >= 9) {
            animationStep = 8;
        }
        temp_a2 = ((u32)-(animationStep * 64) >> 5) + 0xF9;
        if (arg1 < 0xA) {
            DrawLogoSprite(temp_s7, (s16)(((arg1 % 11) * 0xC) + 0x56),
                           (s16)(temp_a2 + ((arg1 / 11) * 0x18)), 8,
                           (s32)(s16)(animationStep * 2), (s32)(s16)((arg1 % 32) * 8),
                           (s32)(s16)((arg1 / 32) * 16 + 0x18), 0, 0, 0, 0x244, 1, 1, 0x5B);
        } else if (arg1 >= 0xB) {
            DrawLogoSprite(temp_s7, (s16)(((arg1 % 11) * 0xC) + 0x56),
                           (s16)(temp_a2 + ((arg1 / 11) * 0x18)), 8,
                           (s32)(s16)(animationStep * 2),
                           (s32)(s16)(((arg1 - 1) % 32) * 8),
                           (s32)(s16)(((arg1 - 1) / 32) * 16 + 0x18), 0, 0, 0, 0x244, 1, 1,
                           0x5B);
        }
        rowI = 0;
        do {
            rowX = (rowI * 0xC) + 0x56;
            DrawLogoSprite(temp_s7 + 4, (rowX << 16) >> 16, temp_a2, 8,
                           (s32)(s16)(animationStep * 2), (s32)(s16)(rowI * 8), 0x18, 0, 0, 0,
                           0x244, 1, 1, 0x3B);
            rowI += 1;
        } while (rowI < 0xA);
        i = 0;
        do {
            rowX = (i * 0xC) + 0x56;
            DrawLogoSprite(temp_s7 + 4, (rowX << 16) >> 16, (s16)(temp_a2 + 0x18), 8,
                           (s32)(s16)(animationStep * 2), (s32)(s16)((i * 8) + 0x50), 0x18, 0,
                           0, 0, 0x244, 1, 1, 0x3B);
            i += 1;
        } while (i < 0xB);
        i = 0;
        do {
            rowX = (i * 0xC) + 0x56;
            DrawLogoSprite(temp_s7 + 4, (rowX << 16) >> 16, (s16)(temp_a2 + 0x30), 8,
                           (s32)(s16)(animationStep * 2), (s32)(s16)((i * 8) + 0xA8), 0x18, 0,
                           0, 0, 0x244, 1, 1, 0x3B);
            i += 1;
        } while (i < 0xB);
        i = 0;
        do {
            rowX = (i * 0xC) + 0x56;
            DrawLogoSprite(temp_s7 + 4, (rowX << 16) >> 16, (s16)(temp_a2 + 0x48), 8,
                           (s32)(s16)(animationStep * 2), (s32)(s16)(i * 8), 0x28, 0, 0, 0,
                           0x244, 1, 1, 0x3B);
            i += 1;
        } while (i < 0xB);
    }
    animationStep = D_8007FB28 - 0x13;
    if (animationStep >= 0) {
        if (animationStep >= 7) {
            animationStep = 6;
        }
        temp_s0 = (u32)-(animationStep * 64) >> 5;
        temp_s2 = animationStep * 2;
        DrawLogoSprite(temp_s7 + 4, 0xDA, (s16)(temp_s0 + 0xF4), 0x1E, (s32)temp_s2, 0xAC,
                       0xE8, 0, 0, 0, 0x244, 1, 1, 0x3A);
        DrawLogoSprite(temp_s7 + 4, 0xDA, (s16)(temp_s0 + 0x10B), 0x20, (s32)temp_s2, 0xAC,
                       0xF4, 0, 0, 0, 0x244, 1, 1, 0x3A);
        DrawLogoSprite(temp_s7 + 4, 0xDA, (s16)(temp_s0 + 0x117), 0x14, (s32)temp_s2, 0xCC,
                       0xF4, 0, 0, 0, 0x244, 1, 1, 0x3A);
    }
    animationStep = D_8007FB28 - 0x11;
    if (animationStep >= 0) {
        if (animationStep >= 9) {
            animationStep = 8;
        }
        i = 0;
        var_a2 = ((u32)-(animationStep * 0x178) >> 5) + 0xF9;
        if (i < (s32)g_TeamNameLength) {
            do {
                temp_v1 = g_TeamNameChars[i];
                if (temp_v1 < 0xAU) {
                    xFirst = (i * 0xC) + 0x56;
                    DrawLogoSprite(temp_s7, (xFirst << 16) >> 16, var_a2, 8,
                                   (s32)(s16)(animationStep * 2),
                                   (s32)(s16)((g_TeamNameChars[i] & 0x1F) * 8),
                                   (s32)(s16)((g_TeamNameChars[i] >> 5) * 16 + 0x18), 0, 0, 0,
                                   0x244, 1, 1, 0x3B);
                } else if (temp_v1 >= 0xBU) {
                    xSecond = (i * 0xC) + 0x56;
                    DrawLogoSprite(temp_s7, (xSecond << 16) >> 16, var_a2, 8,
                                   (s32)(s16)(animationStep * 2),
                                   (s32)(s16)(((g_TeamNameChars[i] - 1) % 32) * 8),
                                   (s32)(s16)(((g_TeamNameChars[i] - 1) / 32) * 16 + 0x18), 0, 0, 0,
                                   0x244, 1, 1, 0x3B);
                }
                i += 1;
            } while (i < (s32)g_TeamNameLength);
        }
    }
    if (arg0 > 0) {
        temp_v0_2 = arg0 + D_8007FB28;
        D_8007FB28 = temp_v0_2;
        if (temp_v0_2 >= 0x1A) {
            D_8007FB28 = 0x19;
        }
    }
}
