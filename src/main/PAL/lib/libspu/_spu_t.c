#include <sys/types.h>

#include "common.h"

#define VA_ROUNDED_SIZE(type) \
    (((sizeof(type) + sizeof(long) - 1) / sizeof(long)) * sizeof(long))
#define va_start(list, last) \
    (list = (void *)((u_char *)&(last) + VA_ROUNDED_SIZE(last)))
#define va_arg(list, type) \
    (list = (void *)((u_char *)list + VA_ROUNDED_SIZE(type)), \
     *(type *)((u_char *)list - VA_ROUNDED_SIZE(type)))

typedef void *va_list;

extern u_short D_8009AB78;
extern volatile u_short *D_8009AB7C;
extern volatile u_long *D_8009AB80;
extern volatile u_long *D_8009AB84;
extern volatile u_long *D_8009AB88;
extern volatile u_long *D_8009AB90;
extern long D_8009ABA0;
extern long D_8009ABC8;
extern long D_8009ABCC;
extern long D_8009ABD0;

long _spu_t(long op, ...) asm("func_80079600");
long _spu_t(long op, ...) {
    long dmaControl;
    long i;
    va_list args;
    u_long count;
    u_short transferMode;
    u_short control;

    va_start(args, op);

    switch (op) {
    case 2:
        count = va_arg(args, u_long);
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

        count = va_arg(args, u_long);
        D_8009ABCC = count;
        count = va_arg(args, u_long);
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
