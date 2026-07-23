#include "common.h"

#define VA_ROUNDED_SIZE(type) \
    (((sizeof(type) + sizeof(s32) - 1) / sizeof(s32)) * sizeof(s32))
#define va_start(list, last) \
    (list = (void *)((u8 *)&(last) + VA_ROUNDED_SIZE(last)))
#define va_arg(list, type) \
    (list = (void *)((u8 *)list + VA_ROUNDED_SIZE(type)), \
     *(type *)((u8 *)list - VA_ROUNDED_SIZE(type)))

typedef void *va_list;

extern u16 D_8009AB78;
extern volatile u16 *D_8009AB7C;
extern volatile u32 *D_8009AB80;
extern volatile u32 *D_8009AB84;
extern volatile u32 *D_8009AB88;
extern volatile u32 *D_8009AB90;
extern s32 D_8009ABA0;
extern s32 D_8009ABC8;
extern s32 D_8009ABCC;
extern s32 D_8009ABD0;

s32 func_80079600(s32 op, ...) {
    s32 dmaControl;
    s32 i;
    va_list args;
    u32 count;
    u16 transferMode;
    u16 control;

    va_start(args, op);

    switch (op) {
    case 2:
        count = va_arg(args, u32);
        D_8009AB78 = count >> D_8009ABA0;
        D_8009AB7C[0xD3] = D_8009AB78;
        break;

    case 1:
        D_8009ABC8 = 0;
        i = 0;
        while (D_8009AB7C[0xD3] != D_8009AB78) {
            if (++i > 0xF00) {
                return -2;
            }
        }

        control = D_8009AB7C[0xD5];
        control &= ~0x30;
        control |= 0x20;
        D_8009AB7C[0xD5] = control;
        break;

    case 0:
        D_8009ABC8 = 1;
        i = 0;
        while (D_8009AB7C[0xD3] != D_8009AB78) {
            if (++i > 0xF00) {
                return -2;
            }
        }

        control = D_8009AB7C[0xD5];
        control &= ~0x30;
        control |= 0x30;
        D_8009AB7C[0xD5] = control;
        break;

    case 3:
        if (D_8009ABC8 == 1) {
            transferMode = 0x30;
        } else {
            transferMode = 0x20;
        }

        i = 0;
        while ((D_8009AB7C[0xD5] & 0x30) != transferMode) {
            if (++i > 0xF00) {
                return -2;
            }
        }

        if (D_8009ABC8 == 1) {
            *D_8009AB90 = (*D_8009AB90 & 0xF0FFFFFF) | 0x22000000;
        } else {
            *D_8009AB90 = (*D_8009AB90 & 0xF0FFFFFF) | 0x20000000;
        }

        count = va_arg(args, u32);
        D_8009ABCC = count;
        count = va_arg(args, u32);
        D_8009ABD0 = count / 64;
        D_8009ABD0 += (count % 64) ? 1 : 0;
        *D_8009AB80 = D_8009ABCC;
        *D_8009AB84 = (D_8009ABD0 << 16) | 0x10;

        if (D_8009ABC8 == 1) {
            dmaControl = 0x01000200;
        } else {
            dmaControl = 0x01000201;
        }
        *D_8009AB88 = dmaControl;
        break;
    }

    return 0;
}
