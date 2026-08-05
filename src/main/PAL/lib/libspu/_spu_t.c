#include <sys/types.h>

#include "common.h"
#include "psyq/spu.h"

#define VA_ROUNDED_SIZE(type) \
    (((sizeof(type) + sizeof(long) - 1) / sizeof(long)) * sizeof(long))
#define va_start(list, last) \
    (list = (void *)((u_char *)&(last) + VA_ROUNDED_SIZE(last)))
#define va_arg(list, type) \
    (list = (void *)((u_char *)list + VA_ROUNDED_SIZE(type)), \
     *(type *)((u_char *)list - VA_ROUNDED_SIZE(type)))

typedef void *va_list;

extern volatile u_short *g_SpuRegBase;
extern long g_SpuDmaTransferAddr;
extern long g_SpuDmaBlockCount;

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
        g_SpuTransferStartAddr = count >> _spu_mem_mode_unitM;
        g_SpuRegBase[0xD3] = g_SpuTransferStartAddr;
        break;

    case 1:
        g_SpuTransferIsRead = 0;
        i = 0;
        while (g_SpuRegBase[0xD3] != g_SpuTransferStartAddr) {
            if (++i > 0xF00) {
                return -2;
            }
        }

        control = g_SpuRegBase[0xD5];
        control &= ~0x30;
        control |= 0x20;
        g_SpuRegBase[0xD5] = control;
        break;

    case 0:
        g_SpuTransferIsRead = 1;
        i = 0;
        while (g_SpuRegBase[0xD3] != g_SpuTransferStartAddr) {
            if (++i > 0xF00) {
                return -2;
            }
        }

        control = g_SpuRegBase[0xD5];
        control &= ~0x30;
        control |= 0x30;
        g_SpuRegBase[0xD5] = control;
        break;

    case 3:
        if (g_SpuTransferIsRead == 1) {
            transferMode = 0x30;
        } else {
            transferMode = 0x20;
        }

        i = 0;
        while ((g_SpuRegBase[0xD5] & 0x30) != transferMode) {
            if (++i > 0xF00) {
                return -2;
            }
        }

        if (g_SpuTransferIsRead == 1) {
            *g_SpuDelayReg = (*g_SpuDelayReg & 0xF0FFFFFF) | 0x22000000;
        } else {
            *g_SpuDelayReg = (*g_SpuDelayReg & 0xF0FFFFFF) | 0x20000000;
        }

        count = va_arg(args, u_long);
        g_SpuDmaTransferAddr = count;
        count = va_arg(args, u_long);
        g_SpuDmaBlockCount = count / 64;
        g_SpuDmaBlockCount += (count % 64) ? 1 : 0;
        *g_SpuDmaMadr = g_SpuDmaTransferAddr;
        *g_SpuDmaBcr = (g_SpuDmaBlockCount << 16) | 0x10;

        if (g_SpuTransferIsRead == 1) {
            dmaControl = 0x01000200;
        } else {
            dmaControl = 0x01000201;
        }
        *g_SpuDmaChcr = dmaControl;
        break;
    }

    return 0;
}
