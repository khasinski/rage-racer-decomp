#include "common.h"
extern s32 *D_801E4FAC;
extern s32 D_801E419C, D_801F17B0, D_8019CE0C;
extern u8 D_80010E30[], D_80010E34[], D_80010E38[], D_80010E40[];
void func_80016EA0(s32 id, void *dst, void *src, s32 arg3);
void func_800632F0(void *dst, void *fmt, s32 val);
void func_800206B8(u8 *s0) {
    u8 sp[16];
    if (D_801E4FAC[4] > 0x3B9AC9FF) {
        D_801E4FAC[4] = 0x3B9AC9FF;
    }
    func_80016EA0(0x10, s0 + 128, D_80010E30, 0x7812);
    func_800632F0(sp, D_80010E34, D_801F17B0);
    func_80016EA0(0x12, s0 + 140, sp, 0x7812);
    func_80016EA0(0x10, s0 + 160, D_80010E38, 0x7812);
    func_800632F0(sp, D_80010E34, D_801E4FAC[4]);
    func_80016EA0(0x12, s0 + 172, sp, 0x7812);
    if (D_801E419C != 0) {
        func_80016EA0(0x10, s0 + 192, D_80010E40, 0x7812);
        func_800632F0(sp, D_80010E34, D_8019CE0C);
        func_80016EA0(0x12, s0 + 204, sp, 0x7812);
    }
}
