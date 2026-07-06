#include "common.h"

void func_80064FA8(u8 *arg0);
void func_80064E90(u8 *arg0, s32 arg1);
void func_80064EB8(u8 *arg0, s32 arg1);

void func_80032FF0(u8 *prim, u8 *src) {
    func_80064FA8(prim);

    *(u16 *)(prim + 0x08) = *(u16 *)(src + 0x00);
    *(u16 *)(prim + 0x0A) = *(u16 *)(src + 0x02);
    *(u16 *)(prim + 0x10) = *(u16 *)(src + 0x04);
    *(u16 *)(prim + 0x12) = *(u16 *)(src + 0x06);
    *(u8 *)(prim + 0x0C) = *(u8 *)(src + 0x08);
    *(u8 *)(prim + 0x0D) = *(u8 *)(src + 0x0A);
    *(u16 *)(prim + 0x0E) = *(u16 *)(src + 0x0C);
    func_80064E90(prim, *(s32 *)(src + 0x10));
    func_80064EB8(prim, 1);
}
