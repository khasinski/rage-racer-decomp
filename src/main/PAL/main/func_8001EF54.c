#include "common.h"

void func_8001674C(const char *fmt);

extern char D_80010D34[];

typedef struct {
    u8 pad0[0x18];
    u16 field_18;
    u16 field_1A;
    u8 pad1[0xC];
    s32 field_28;
    u16 field_2C;
    u16 field_2E;
    u8 pad2[0x4];
    s32 field_34;
} Unk8001EF54;

void func_8001EF54(Unk8001EF54 *arg0) {
    volatile s32 timeout = 0x800000;
    s32 one;
    u16 x;

    if (arg0->field_34 == 0) {
        one = 1;
        do {
            timeout = timeout - 1;
            if (timeout == 0) {
                func_8001674C(D_80010D34);
                arg0->field_34 = one;
                arg0->field_28 = arg0->field_28 < 1U;
                x = ((Unk8001EF54 *)((u8 *)arg0 + (arg0->field_28 << 3)))->field_18;
                arg0->field_2C = x;
                arg0->field_2E = ((Unk8001EF54 *)((u8 *)arg0 + (arg0->field_28 << 3)))->field_1A;
            }
        } while (arg0->field_34 == 0);
    }

    arg0->field_34 = 0;
}
