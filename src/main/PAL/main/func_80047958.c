#include "common.h"

typedef struct {
    u8 u;
    u8 v;
    u16 w;
} Glyph;

extern Glyph D_8007FA3C[];

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

void func_80047958(s32 x0, s16 y, u8 *str0, u8 color, u8 g, u8 b, u16 clut, s32 flags) {
    register u8 *str asm("$18");
    register s32 x asm("$17");
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
    ot = *(void **)0x1F800004;

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
            goto draw;
        case '-':
            idx = 0x25;
            goto draw;
        case '!':
            idx = 0x26;
            goto draw;
        case '?':
            idx = 0x27;
            goto draw;
        case '@':
            idx = 0x28;
            goto draw;
        case '/':
            idx = 0x2b;
            goto draw;
        case ',':
            idx = 0x2c;
            goto draw;
        case '"':
            idx = 0x2d;
            goto draw;
        case '\'':
            idx = 0x2e;
            goto draw;
        case ':':
            idx = 0x30;
            goto draw;
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
        w = fixed ? 8 : D_8007FA3C[idx].w;
        u0 = fixed ? (idx % 32) * 8 : D_8007FA3C[idx].u;
        v0 = fixed ? (idx / 32) * 16 + 24 : D_8007FA3C[idx].v;

        func_80046A2C(
            (u8 *)ot + 4,
            (s16)x,
            (s16)y,
            (s16)w,
            0x10,
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
