#include "common.h"
#include "game/scratchpad.h"
#include "game/state.h"

typedef struct Glyph {
    u8 u;
    u8 v;
    u16 w;
} Glyph;

extern Glyph D_8007F984[];

void GameDrawSpriteWide(
    void *ot,
    s32 x0,
    s32 y0,
    s32 x1,
    s32 y1,
    s32 u0,
    s32 v0,
    s32 r,
    s32 g,
    s32 b,
    s32 clut,
    s32 sh,
    s32 st,
    s32 flags) asm("DrawSprite");
void *QueueDrawModePrim(void *ot, void *prim, s32 arg2);

void DrawSmallText(s32 x0, s16 y, u8 *str0, u8 color, u8 g, u8 b, u16 clut, s32 flags) {
    u8 *str;
    s32 x;
    u8 fl = flags;
    void *ot;
    s32 fixed;
    s32 idx;
    s32 u0;
    s32 v0;
    s32 w;
    u8 c;
    s32 c2;

    str = str0;
    x = x0;
    ot = SCRATCH_OT_BASE_AS(void);

    while (*str) {
        fixed = flags & 0x80;
        c = *str;
        str++;
        switch (c) {
        case ' ':
            x += 6;
            continue;
        case '/':
            idx = 0x24;
            break;
        case '.':
            idx = 0x25;
            break;
        case ',':
            idx = 0x26;
            break;
        case '"':
            idx = 0x27;
            break;
        case '\'':
            idx = 0x28;
            break;
        case '-':
            idx = 0x29;
            break;
        case 0x81:
            c2 = *str;
            str++;
            switch (c2) {
            case 0x9b:
                idx = 0x2a;
                break;
            case 0xa0:
                idx = 0x2b;
                break;
            case 0x7e:
                idx = 0x2c;
                break;
            case 0xa2:
                idx = 0x2d;
                break;
            default:
                continue;
            }
            break;
        default:
            if (c < '0') {
                continue;
            }
            if (c < ':') {
                idx = c - '0';
            } else if (c < 'A') {
                continue;
            } else if (c < '[') {
                idx = c - '7';
            } else {
                continue;
            }
        }

        w = fixed ? 6 : D_8007F984[idx].w;
        u0 = fixed ? (idx % 42) * 6 : D_8007F984[idx].u;
        v0 = fixed ? (idx / 42) * 12 : D_8007F984[idx].v;

        GameDrawSpriteWide(
            (u8 *)ot + 4,
            (s16)x,
            (s16)y,
            (s16)w,
            0xc,
            (s16)u0,
            (s16)v0,
            (u8)color,
            g,
            b,
            clut,
            0,
            1,
            0x80);
        {
            s32 nx;
            nx = x + w;
            asm("" : "=r"(nx) : "0"(nx));
            x = nx;
        }
    }

    SCRATCH_PRIM_CURSOR_AS(void) =
        QueueDrawModePrim((u8 *)ot + 4, SCRATCH_PRIM_CURSOR_AS(void), (fl & 0x7f) + 27);
}

extern Glyph D_8007FA3C[];

void GameDrawSpriteWide(
    void *ot,
    s32 x0,
    s32 y0,
    s32 x1,
    s32 y1,
    s32 u0,
    s32 v0,
    s32 r,
    s32 g,
    s32 b,
    s32 clut,
    s32 sh,
    s32 st,
    s32 flags);
void *QueueDrawModePrim(void *ot, void *prim, s32 arg2);

void DrawLargeText(s32 x0, s16 y, u8 *str0, u8 color, u8 g, u8 b, u16 clut, s32 flags) {
    u8 *str;
    s32 x;
    u8 fl = flags;
    s32 fixed;
    void *ot;
    s32 idx;
    s32 u0;
    s32 v0;
    s32 w;
    u8 c;

    str = str0;
    x = x0;
    ot = SCRATCH_OT_BASE_AS(void);

    while (*str) {
        fixed = flags & 0x80;
        c = *str;
        str++;
        switch (c) {
        case ' ':
            x += 8;
            continue;
        case '.':
            idx = 0x24;
            break;
        case '-':
            idx = 0x25;
            break;
        case '!':
            idx = 0x26;
            break;
        case '?':
            idx = 0x27;
            break;
        case '@':
            idx = 0x28;
            break;
        case '/':
            idx = 0x2b;
            break;
        case ',':
            idx = 0x2c;
            break;
        case '"':
            idx = 0x2d;
            break;
        case '\'':
            idx = 0x2e;
            break;
        case ':':
            idx = 0x30;
            break;
        default:
            if (c < '0') {
                continue;
            }
            if (c < ':') {
                idx = c - '0';
            } else if (c < 'A') {
                continue;
            } else if (c < '[') {
                idx = c - '7';
            } else {
                continue;
            }
        }

        w = fixed ? 8 : D_8007FA3C[idx].w;
        u0 = fixed ? (idx % 32) * 8 : D_8007FA3C[idx].u;
        v0 = fixed ? (idx / 32) * 16 + 24 : D_8007FA3C[idx].v;

        GameDrawSpriteWide(
            (u8 *)ot + 4,
            (s16)x,
            (s16)y,
            (s16)w,
            0x10,
            (s16)u0,
            (s16)v0,
            (u8)color,
            g,
            b,
            clut,
            0,
            1,
            0x80);
        {
            s32 nx;
            nx = x + w;
            asm("" : "=r"(nx) : "0"(nx));
            x = nx;
        }
    }

    SCRATCH_PRIM_CURSOR_AS(void) =
        QueueDrawModePrim((u8 *)ot + 4, SCRATCH_PRIM_CURSOR_AS(void), (fl & 0x7f) + 27);
}

void GameDrawSpriteWide(
    void *ot,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 u,
    s32 v,
    s32 r,
    s32 g,
    s32 b,
    s32 clut,
    s32 arg11,
    s32 arg12,
    s32 flags);
void *QueueDrawModePrim(void *ot, void *prim, s32 count);

s32 GameDrawNumber(
    s32 x,
    s16 y,
    s32 flags,
    u32 value,
    u8 r,
    u8 g,
    u8 b,
    u16 clut,
    u8 primitiveCount);
s32 GameDrawNumber(
    s32 x,
    s16 y,
    s32 flags,
    u32 value,
    u8 r,
    u8 g,
    u8 b,
    u16 clut,
    u8 primitiveCount) {
    u8 digits[11];
    u16 drawVValue;
    void *ot;
    s32 width;
    s32 height;
    s32 v;
    s32 i;
    s32 digit;
    s32 drawn;
    s32 drawWidth;
    s32 drawHeight;
    s32 newHeight;
    s32 drawV;
    s32 small;
    s32 nextX;

    i = 9;
    if (flags & 8) {
        ot = (u8 *)SCRATCH_OT_BASE_AS(void) + 4;
    } else {
        ot = SCRATCH_OT_BASE_AS(void);
    }

    height = 16;
    if (flags & 4) {
        width = 8;
        v = 0xDC;
    } else {
        small = flags & 1;
        width = small ? 8 : 6;
        height = small ? 16 : 12;
        v = (-small) & 0x18;
    }

    for (digit = 0; digit <= i; digit++) {
        digits[i - digit] = value % 10;
        value /= 10;
    }

    digits[10] = 0xFF;
    i = 0;
    while ((digits[i] == 0) && (digits[i + 1] != 0xFF)) {
        digits[i] = ' ';
        i++;
    }

    drawn = 0;
    if (flags & 2) {
        i = 0;
    }

    if (digits[i] != 0xFF) {
        drawWidth = width;
        drawHeight = newHeight = height;
        drawV = v;
        while (digits[i] != 0xFF) {
            drawVValue = drawV;
            if (digits[i] == ' ') {
                nextX = x + width;
                if (nextX != drawWidth) {
                    x = nextX;
                } else {
                    x = nextX;
                }
                i++;
                continue;
            }

            GameDrawSpriteWide(
                ot,
                (s16)x,
                y,
                drawWidth,
                drawHeight,
                digits[i] * drawWidth,
                drawVValue,
                r,
                g,
                b,
                clut,
                0,
                1,
                0x80);
            i++;
            digit = width;
            drawn++;
            /* The identical arms are what keeps nextX alive past the add:
             * with a plain `x = nextX;` combine folds the pair into
             * `addu s1,s1,s3` and retail has `addu v0,s1,s3` / `move s1,v0`.
             * The loop note that used to wrap this bought nothing on top. */
            nextX = x + digit;
            if (nextX) {
                x = nextX;
            } else {
                x = nextX;
            }
        }
    }

    SCRATCH_PRIM_CURSOR_AS(void) =
        QueueDrawModePrim(ot, SCRATCH_PRIM_CURSOR_AS(void), primitiveCount + 27);
    return drawn;
}

extern u8 D_8007F6E8[];
extern s32 D_8007FB00;

void *QueueDrawModePrim(void *arg0, void *arg1, s32 arg2);

void DrawBitPatternOverlay(s32 arg0) {
    void *ot = SCRATCH_OT_BASE_AS(void);
    u8 *row = D_8007F6E8;
    s32 y;
    s32 outer;
    s32 one;
    s32 x;
    s32 bit;
    s32 offset;

    if (arg0 == 0) {
        return;
    }

    if (arg0 < 0) {
        if ((g_AnimTimer % 6U) == 0) {
            D_8007FB00 += 8;
        }

        offset = D_8007FB00;
        offset += (s32)D_8007F6E8;
        if (*(u8 *)(offset + 7) != 0) {
            D_8007FB00 = 0x10;
        }
        row = (u8 *)(D_8007FB00 + (s32)D_8007F6E8);
    } else {
        row += (arg0 - 1) * 8;
    }

    y = 0x150;
    outer = 0;
    one = 1;
    do {
        x = 0x22;
        bit = 0;
        do {
            if (((*row << bit) & 0x80) != 0) {
                GameDrawSpriteWide(
                    (u8 *)ot + 4,
                    (s16)x,
                    (s16)y,
                    4,
                    8,
                    0xFC,
                    0,
                    0,
                    0,
                    0,
                    0x244,
                    one,
                    one,
                    0x80);
            }
            bit++;
            x += 4;
        } while (bit < 8);
        y += 8;
        outer++;
        row++;
    } while (outer < 6);

    outer = 0;
    x = 1;
    bit = 0x4C0000;
    do {
        GameDrawSpriteWide(
            (u8 *)ot + 4,
            (s16)(bit >> 16),
            0x33,
            4,
            8,
            0xFC,
            0,
            0,
            0,
            0,
            0x244,
            x,
            x,
            0x80);
        bit += 0x50000;
        outer++;
    } while (outer < 0x10);

    SCRATCH_PRIM_CURSOR_AS(void) = QueueDrawModePrim((u8 *)ot + 4, SCRATCH_PRIM_CURSOR_AS(void), 0x39);
}
