#include <sys/types.h>

#include "common.h"

extern volatile u_short *D_8009AB7C;
extern u_short D_8009AB78;
extern long D_8009AB74;
extern u_char D_80013EC0[];
extern u_char D_80013EE0[];
extern u_char D_80013EF4[];


void _spu_writeByIO(u_short *addr, u_long size) asm("func_80078F4C");
void _spu_writeByIO(u_short *addr, u_long size) {
    volatile long di, dj;
    register u_short stat0 asm("$5");
    register u_short *paddr asm("$18") = addr;
    u_short saved;
    register u_short ctrl asm("$4");
    register u_short cmasked asm("$2");
    long chunk;
    long k;

#define SPU_DELAY()                           \
    for (dj = 13, di = 0; di < 240; di++) {   \
        dj = dj * 3;                          \
    }

    stat0 = D_8009AB7C[0xD7];
    D_8009AB7C[0xD3] = D_8009AB78;
    saved = stat0 & 0x7ff;
    SPU_DELAY();

    if (size != 0) {
        do {
            chunk = (size < 65) ? size : 64;
            for (k = 0; k < chunk; k += 2) {
                D_8009AB7C[0xD4] = *paddr++;
            }
            ctrl = D_8009AB7C[0xD5];
            cmasked = ctrl & 0xffcf;
            asm("" : "=r"(cmasked) : "0"(cmasked));
            ctrl = cmasked | 0x10;
            D_8009AB7C[0xD5] = ctrl;

            SPU_DELAY();

            D_8009AB74 = 0;
            while (D_8009AB7C[0xD7] & 0x400) {
                if (++D_8009AB74 >= 5001) {
                    GameDebugPrintf(D_80013EC0, D_80013EE0);
                    break;
                }
            }

            SPU_DELAY();
            SPU_DELAY();

            size -= chunk;
        } while (size != 0);
    }

    ctrl = D_8009AB7C[0xD5];
    D_8009AB7C[0xD5] = ctrl & 0xffcf;
    D_8009AB74 = 0;
    while ((D_8009AB7C[0xD7] & 0x7ff) != saved) {
        if (++D_8009AB74 >= 5001) {
            GameDebugPrintf(D_80013EC0, D_80013EF4);
            break;
        }
    }
}
