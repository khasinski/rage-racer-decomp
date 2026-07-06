#include "common.h"
#include "psyq/gpu.h"

s32 func_800657E4(void);

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

void func_80065234(DrawEnv *arg0) {
    s32 mode;
    u32 value;

    D_800941E4(D_8001339C, arg0->clip.x, arg0->clip.y, arg0->clip.w, arg0->clip.h);
    D_800941E4(D_800133B4, arg0->ofs[0], arg0->ofs[1]);
    D_800941E4(D_800133C4, arg0->tw.x, arg0->tw.y, arg0->tw.w, arg0->tw.h);
    D_800941E4(D_800133DC, arg0->dtd);
    D_800941E4(D_800133E8, arg0->dfe);

    mode = func_800657E4();
    if (mode == 1) {
        goto high_mode;
    }

    mode = func_800657E4();
    if (mode != 2) {
        goto low_mode;
    }

high_mode:
    value = arg0->tpage;
    D_800941E4(D_80013374, (value >> 9) & 3, (value >> 7) & 3, (value << 6) & 0x7C0, (value << 3) & 0x300);
    return;

low_mode:
    value = arg0->tpage;
    D_800941E4(D_80013374, (value >> 7) & 3, (value >> 5) & 3, (value << 6) & 0x7C0, ((value << 4) & 0x100) + ((value >> 2) & 0x200));
}

void func_800653B4(DispEnv *arg0) {
    D_800941E4(D_800133F4, arg0->disp.x, arg0->disp.y, arg0->disp.w, arg0->disp.h);
    D_800941E4(D_80013410, arg0->screen.x, arg0->screen.y, arg0->screen.w, arg0->screen.h);
    D_800941E4(D_8001342C, arg0->isinter);
    D_800941E4(D_80013438, arg0->isrgb24);
}
