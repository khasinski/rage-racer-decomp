#include "psyq/kernel.h"

extern volatile u32 *D_8009A4F4;
extern void (*D_8009A4F8[])(void);
extern u32 *D_8009A518;
extern u8 D_80013BA8[];
extern u8 D_80013BC4[];

void func_8001674C(u8 *arg0, ...);

void intrDMADispatcher(void) {
    u32 pending;
    u32 pendingTemp;
    s32 i;
    void (**handler)(void);
    u32 lowMask;
    u32 one;
    register void (**handlerBase)(void) asm("$21");
    register u8 *fmt asm("$4");

    pendingTemp = *D_8009A4F4;
    pending = (pendingTemp >> 0x18) & 0x7F;
    if (pending != 0) {
        one = 1;
        lowMask = 0xFFFFFF;
        handlerBase = D_8009A4F8;
        do {
            i = 0;
            if (pending != 0) {
                handler = handlerBase;
                while ((pending != 0) && (i < 7)) {
                    if (pending & 1) {
                        register volatile u32 *bits asm("$4");
                        register u32 value asm("$2");
                        register s32 shift asm("$2");

                        bits = D_8009A4F4;
                        shift = i + 0x18;
                        value = one << shift;
                        value |= lowMask;
                        value &= *bits;
                        *bits = value;
                        if (*handler != 0) {
                            (*handler)();
                        }
                    }
                    handler++;
                    pending >>= 1;
                    i++;
                }
            }

            pendingTemp = *D_8009A4F4;
            pending = (pendingTemp >> 0x18) & 0x7F;
        } while (pending != 0);
    }

    if (((*D_8009A4F4 & 0xFF000000) == 0x80000000) || ((*D_8009A4F4 & 0x8000) != 0)) {
        fmt = D_80013BA8;
        func_8001674C(fmt, *D_8009A4F4);
        for (i = 0; i < 7; i++) {
            func_8001674C(D_80013BC4, i, D_8009A518[i * 4]);
        }
    }
}
