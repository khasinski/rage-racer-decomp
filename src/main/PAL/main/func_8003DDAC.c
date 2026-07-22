#include "common.h"

extern s32 D_801E428C;
extern s32 D_801E40E4;
extern u16 D_8007E33C[];
extern u16 D_8007E334[];
extern u8 D_8007E300[];
extern u8 D_8007E2F4[];

void func_8001A530(void *arg0, s32 arg1);
void func_8001A4C0(void *arg0, s32 arg1);
void func_80069568(void *arg0, void *arg1);
void func_80017794(void *arg0, void *arg1, void *arg2);
void func_80029E50(void *arg0, s32 arg1);
s32 func_800632B0(void);

void func_8003DDAC(s32 arg0, s32 arg1) {
    s16 sp10[16];
    s16 sp30[16];
    register s32 frame asm("$23") = arg0;
    register s32 update asm("$20") = arg1;
    register u16 *dst asm("$16");
    register u16 *delta asm("$22");
    register u16 *deltaBase asm("$2");
    register s16 *work asm("$18") = sp30;
    register u16 *base asm("$21");
    register s32 offset asm("$17");
    register s32 end asm("$19");
    register s32 start asm("$2");
    register s32 loopIndex asm("$5");
    register s32 dstOffset asm("$2");
    s32 limit;
    register s32 active asm("$3");
    register s32 activeValue asm("$2");
    register s32 frameMask asm("$2");

    activeValue = D_801E428C;
    active = activeValue & 3;
    active = active != 0;
    if (active) {
        start = 1;
        end = 4;
    } else {
        asm("addu %0,$0,$0" : "=r"(start));
        end = 1;
    }

    loopIndex = start;
    asm("" : "=r"(loopIndex) : "0"(loopIndex));
    if (loopIndex < end) {
        deltaBase = D_8007E33C;
        delta = &deltaBase[active];
        asm("" : "=r"(delta) : "0"(delta));
        work = sp30;
        base = D_8007E334;
        asm("" : "=r"(base) : "0"(base));
        dstOffset = loopIndex << 1;
        asm("addu %0,%1,%2" : "=r"(dst) : "r"(dstOffset), "r"(base));
        offset = loopIndex * 0x10;

        do {
            if (update != 0) {
                *dst += *delta;
            }
            *dst &= 0xFFF;

            func_8001A530(sp10, *(s32 *)(D_8007E300 + offset));
            func_80069568((void *)0x1F800028, sp10);
            func_8001A4C0(work, *(s16 *)dst);
            func_80069568(sp10, work);
            func_80017794((void *)0x1F80011C, D_8007E2F4 + offset, work);

            *(s32 *)0x1F800084 = 0;
            limit = 1;
            if (D_801E40E4 >= 0x3F) {
                limit = 0x3E;
            }
            func_80029E50((void *)0x1F800000, limit);

            dst++;
            offset += 0x10;
        } while ((s32)dst < ((end << 1) + (s32)base));
    }

    frameMask = frame & 0x1FF;
    if ((frameMask == 0) && (update != 0)) {
        D_8007E33C[0] = func_800632B0() & 0x1F;
        D_8007E33C[1] = func_800632B0() & 0x3F;
    }
}
