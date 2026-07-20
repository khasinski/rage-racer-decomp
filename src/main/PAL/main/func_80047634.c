#include "common.h"

typedef struct {
    u8 u;
    u8 v;
    u16 w;
} Glyph;

extern Glyph D_8007F984[];

void func_80046A2C(
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
void *func_80017390(void *ot, void *prim, s32 arg2);

void func_80047634(s32 x0, s16 y, u8 *str0, u8 color, u8 g, u8 b, u16 clut, s32 flags) {
    register u8 *str asm("$16");
    register s32 x asm("$18");
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
    ot = *(void **)0x1F800004;

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
            goto draw;
        case '.':
            idx = 0x25;
            goto draw;
        case ',':
            idx = 0x26;
            goto draw;
        case '"':
            idx = 0x27;
            goto draw;
        case '\'':
            idx = 0x28;
            goto draw;
        case '-':
            idx = 0x29;
            goto draw;
        case 0x81:
            c2 = *str;
            str++;
            switch (c2) {
            case 0x9b:
                idx = 0x2a;
                goto draw;
            case 0xa0:
                idx = 0x2b;
                goto draw;
            case 0x7e:
                idx = 0x2c;
                goto draw;
            case 0xa2:
                idx = 0x2d;
                goto draw;
            }
            continue;
        }

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

    draw:
        w = fixed ? 6 : D_8007F984[idx].w;
        u0 = fixed ? (idx % 42) * 6 : D_8007F984[idx].u;
        v0 = fixed ? (idx / 42) * 12 : D_8007F984[idx].v;

        func_80046A2C(
            (u8 *)ot + 4,
            (s16)x,
            (s16)y,
            (s16)w,
            0xc,
            (s16)u0,
            (s16)v0,
            color & 0xff,
            g,
            b,
            clut,
            0,
            1,
            0x80);
        {
            register s32 nx asm("$2");
            nx = x + w;
            asm("" : "=r"(nx) : "0"(nx));
            x = nx;
        }
    }

    *(void **)0x1F800000 =
        func_80017390((u8 *)ot + 4, *(void **)0x1F800000, (fl & 0x7f) + 27);
}
