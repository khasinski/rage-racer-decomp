#include "common.h"

s32 GetGraphType(void) asm("func_800657E4");

extern char D_80013374[];
extern char D_8001339C[];
extern char D_800133B4[];
extern char D_800133C4[];
extern char D_800133DC[];
extern char D_800133E8[];
extern char D_800133F4[];
extern char D_80013410[];
extern char D_8001342C[];
extern char D_80013438[];
extern void (*D_800941E4)(char *, ...);

typedef struct {
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
    s16 u0;
    s16 v0;
    s16 x2;
    s16 y2;
    s16 x3;
    s16 y3;
    u16 mode;
    u8 b0;
    u8 b1;
} UnkFunc80065234;

typedef struct {
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
    s16 x2;
    s16 y2;
    s16 x3;
    s16 y3;
    u8 b0;
    u8 b1;
} UnkFunc800653B4;

void DumpDrawEnv(UnkFunc80065234 *arg0) asm("func_80065234");
void DumpDispEnv(UnkFunc800653B4 *arg0) asm("func_800653B4");

void DumpDrawEnv(UnkFunc80065234 *arg0) {
    s32 mode;
    register u32 value asm("$2");

    D_800941E4(D_8001339C, arg0->x0, arg0->y0, arg0->x1, arg0->y1);
    D_800941E4(D_800133B4, arg0->u0, arg0->v0);
    D_800941E4(D_800133C4, arg0->x2, arg0->y2, arg0->x3, arg0->y3);
    D_800941E4(D_800133DC, arg0->b0);
    D_800941E4(D_800133E8, arg0->b1);

    mode = GetGraphType();
    if (mode == 1) {
        goto high_mode;
    }

    mode = GetGraphType();
    if (mode != 2) {
        goto low_mode;
    }

high_mode:
    value = arg0->mode;
    D_800941E4(D_80013374, (value >> 9) & 3, (value >> 7) & 3, (value << 6) & 0x7C0, (value << 3) & 0x300);
    return;

low_mode:
    value = arg0->mode;
    D_800941E4(D_80013374, (value >> 7) & 3, (value >> 5) & 3, (value << 6) & 0x7C0, ((value << 4) & 0x100) + ((value >> 2) & 0x200));
}

void DumpDispEnv(UnkFunc800653B4 *arg0) {
    D_800941E4(D_800133F4, arg0->x0, arg0->y0, arg0->x1, arg0->y1);
    D_800941E4(D_80013410, arg0->x2, arg0->y2, arg0->x3, arg0->y3);
    D_800941E4(D_8001342C, arg0->b0);
    D_800941E4(D_80013438, arg0->b1);
}
