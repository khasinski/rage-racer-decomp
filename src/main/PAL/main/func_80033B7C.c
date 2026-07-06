#include "common.h"

extern u8 *D_8019C900;

void func_80064F80(u8 *prim);
void func_80064EB8(u8 *prim, s32 enabled);
void func_80064DDC(void *ot, void *prim);

u8 *func_80033B7C(u8 *prim, s32 x, s32 y, s32 code, u16 arg4) {
    register u8 *out asm("$16") = prim;
    register s32 xReg asm("$18") = x;
    register s32 yReg asm("$19") = y;
    register s32 codeReg asm("$17");
    register s32 arg4Reg asm("$20");

    asm("" : "=r"(xReg), "=r"(yReg) : "0"(xReg), "1"(yReg) : "$17");
    codeReg = code;
    arg4Reg = arg4;
    func_80064F80(out);
    func_80064EB8(out, 1);

    codeReg <<= 3;
    out[0xC] = codeReg;
    out[0xD] = 0x10;

    {
        register u8 *ot asm("$4") = D_8019C900;
        register u8 *oldPrim asm("$5") = out;

        *(s16 *)&out[0x8] = xReg;
        *(s16 *)&out[0xA] = yReg;
        *(s16 *)&out[0xE] = arg4Reg;
        out += 0x10;
        func_80064DDC(ot + 0xCC, oldPrim);
    }

    return out;
}
