#include <sys/types.h>

#include "common.h"

/*
 * LEFT RAW ON PURPOSE (docs/names.md 17i). This is libcd's interrupt decoder:
 * it reads the 8-byte response FIFO, decodes intr codes 1..5 into D_80099318
 * and the two result buffers, and is drained in a while loop by CD_sync,
 * CD_ready, CD_cw and the IRQ2 handler. Every other libcd internal in this
 * image was pinned because it stores its own name into a trace slot; this one
 * owns only "DiskError: " and "CDROM: unknown intr", neither of which names a
 * function. CD_intr / CD_getintr / CD_status / CD_readIntr are all
 * unfalsifiable here, so no name is asserted.
 */

extern volatile u_char *D_80099300;
extern volatile u_char *D_80099304;
extern volatile u_char *D_80099308;
extern volatile u_char *D_8009930C;
extern long D_80099048;
extern u_long D_8009904C;
extern u_long D_80099050;
extern u_long D_80099054;
extern u_long D_80099060[];
extern u_long D_80099100[];
extern u_long D_80099200[];
extern u_char D_8009905D;
extern volatile u_char D_80099318;
extern volatile u_char D_80099319;
extern volatile u_char D_8009931A;
extern u_char D_8009BAF0;
extern u_char D_8009BAF8;
extern u_char D_8009BB00;
extern u_char D_80013840;
extern u_char D_8001384C;
extern u_char D_80013868;
extern u_char D_8001387C;

void func_80063C38();

static __inline__ void copy8(u_char *d, u_char *s) {
    long n;
    if (d == 0) {
        return;
    }
    n = 7;
    do {
        *d++ = *s++;
    } while (--n != -1);
}

long func_8006AB5C(void) {
    volatile u_char mode;
    volatile u_char buf[8];
    long i;
    long flag;
    long v;
    volatile u_char *p;
    volatile u_char *q;

    *D_80099300 = 1;
    p = D_8009930C;
    mode = *p & 7;
    if (mode == 0) {
        return 0;
    }
    flag = 0;
    while (mode != (*p & 7)) {
        mode = *p & 7;
    }
    i = 0;
    q = buf;
    for (; i < 8; i++) {
        if (!(*D_80099300 & 0x20)) {
            break;
        }
        *q++ = *D_80099304;
    }
    if (i < 8) {
        volatile u_char *r;
        r = (volatile u_char *)(i + (long)buf);
        do {
            *r++ = 0;
        } while ((long)r < (long)&buf[8]);
    }

    *D_80099300 = 1;
    *D_8009930C = 7;
    *D_80099308 = 7;

    if (!(mode == 3 && D_80099200[D_8009905D] == 0)) {
        if (!(D_8009904C & 0x10) && (buf[0] & 0x10)) {
            D_80099054++;
        }
        v = buf[0];
        flag = v & 0x1d;
        D_8009904C = v;
        D_80099050 = buf[1];
    }

    if (mode == 5) {
        func_80063C38(&D_80013840);
        if (D_80099048 > 0) {
            GameDebugPrintf(&D_8001384C, D_80099060[D_8009905D], D_8009904C, D_80099050);
        }
    }

    switch (mode) {
    case 3:
        if (flag) {
            volatile u_char *sp = &D_80099318;
            *sp = 5;
            copy8(&D_8009BAF0, (u_char *)buf);
            return 2;
        }
        if (D_80099100[D_8009905D] != 0) {
            volatile u_char *sp = &D_80099318;
            *sp = 3;
            copy8(&D_8009BAF0, (u_char *)buf);
            return 1;
        }
        {
            volatile u_char *sp = &D_80099318;
            *sp = 2;
        }
        copy8(&D_8009BAF0, (u_char *)buf);
        return 2;
    case 2:
        D_80099318 = flag ? 5 : 2;
        copy8(&D_8009BAF0, (u_char *)buf);
        return 2;
    case 1:
        if (flag) {
            if (i == 1) {
                flag = 0;
            }
        }
        D_80099319 = flag ? 5 : 1;
        copy8(&D_8009BAF8, (u_char *)buf);
        *D_80099300 = 0;
        *D_8009930C = 0;
        return 4;
    case 4: {
        volatile u_char *sp = &D_80099319;
        D_8009931A = 4;
        *sp = D_8009931A;
        copy8(&D_8009BB00, (u_char *)buf);
        copy8(&D_8009BAF8, (u_char *)buf);
        return 4;
    }
    case 5: {
        volatile u_char *sp = &D_80099318;
        D_80099319 = 5;
        *sp = D_80099319;
        copy8(&D_8009BAF0, (u_char *)buf);
        copy8(&D_8009BAF8, (u_char *)buf);
        return 6;
    }
    default:
        func_80063C38(&D_80013868);
        GameDebugPrintf(&D_8001387C, mode);
        break;
    }
    return 0;
}
