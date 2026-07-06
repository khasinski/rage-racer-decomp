#include "common.h"
#include "psyq/gpu.h"

extern u8 D_8007C7CC[];
void func_8001D4E8(void);
void func_80065B24(Rect *arg0, void *arg1);

void func_8001D530(u8 *str, s32 len) {
    s16 sp[4];
    func_8001D4E8();
    sp[0] = 0x288 - len;
    sp[1] = 0x37;
    sp[2] = 2;
    sp[3] = 8;
    while (len > 0) {
        func_80065B24((Rect *)sp, &D_8007C7CC[*str++ << 5]);
        sp[0] += 2;
        len--;
    }
}
