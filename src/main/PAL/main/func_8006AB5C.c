#include "common.h"

extern volatile u8 *D_80099300;
extern volatile u8 *D_80099304;
extern volatile u8 *D_80099308;
extern volatile u8 *D_8009930C;
extern s32 D_80099048;
extern u32 D_8009904C;
extern u32 D_80099050;
extern u32 D_80099054;
extern u32 D_80099060[];
extern u32 D_80099100[];
extern u32 D_80099200[];
extern u8 D_8009905D;
extern volatile u8 D_80099318;
extern volatile u8 D_80099319;
extern volatile u8 D_8009931A;
extern u8 D_8009BAF0;
extern u8 D_8009BAF8;
extern u8 D_8009BB00;
extern u8 D_80013840;
extern u8 D_8001384C;
extern u8 D_80013868;
extern u8 D_8001387C;

void func_80063C38();
void func_8001674C();

static __inline__ void copy8(u8 *d, u8 *s) {
    s32 n;
    if (d == 0) {
        return;
    }
    n = 7;
    do {
        *d++ = *s++;
    } while (--n != -1);
}

s32 func_8006AB5C(void) {
    volatile u8 mode;
    volatile u8 buf[8];
    s32 i;
    s32 flag;
    s32 v;
    volatile u8 *p;
    volatile u8 *q;

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
        volatile u8 *r;
        r = (volatile u8 *)(i + (s32)buf);
        do {
            *r++ = 0;
        } while ((s32)r < (s32)&buf[8]);
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
            func_8001674C(&D_8001384C, D_80099060[D_8009905D], D_8009904C, D_80099050);
        }
    }

    switch (mode) {
    case 3:
        if (flag) {
            volatile u8 *sp = &D_80099318;
            *sp = 5;
            copy8(&D_8009BAF0, (u8 *)buf);
            return 2;
        }
        if (D_80099100[D_8009905D] != 0) {
            volatile u8 *sp = &D_80099318;
            *sp = 3;
            copy8(&D_8009BAF0, (u8 *)buf);
            return 1;
        }
        {
            volatile u8 *sp = &D_80099318;
            *sp = 2;
        }
        copy8(&D_8009BAF0, (u8 *)buf);
        return 2;
    case 2:
        D_80099318 = flag ? 5 : 2;
        copy8(&D_8009BAF0, (u8 *)buf);
        return 2;
    case 1:
        if (flag) {
            if (i == 1) {
                flag = 0;
            }
        }
        D_80099319 = flag ? 5 : 1;
        copy8(&D_8009BAF8, (u8 *)buf);
        *D_80099300 = 0;
        *D_8009930C = 0;
        return 4;
    case 4: {
        volatile u8 *sp = &D_80099319;
        D_8009931A = 4;
        *sp = D_8009931A;
        copy8(&D_8009BB00, (u8 *)buf);
        copy8(&D_8009BAF8, (u8 *)buf);
        return 4;
    }
    case 5: {
        volatile u8 *sp = &D_80099318;
        D_80099319 = 5;
        *sp = D_80099319;
        copy8(&D_8009BAF0, (u8 *)buf);
        copy8(&D_8009BAF8, (u8 *)buf);
        return 6;
    }
    default:
        func_80063C38(&D_80013868);
        func_8001674C(&D_8001387C, mode);
        break;
    }
    return 0;
}
