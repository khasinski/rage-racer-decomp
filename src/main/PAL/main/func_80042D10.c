#include "common.h"

extern s32 D_8009B1B4;
extern u32 D_8009B174;
extern u32 D_8009B178;
extern u32 D_8009B17C;
extern u32 D_8009B180;
extern u32 D_8009B184;
extern u32 D_8009B188;
extern u32 D_8009B18C;
extern u32 D_8009B190;

void func_8006A94C(u8 *arg0);

void func_80042D10(void) {
    u8 buf[4];
    register s32 cnt asm("$13");

    cnt = D_8009B1B4;
    if (cnt > 0) {
        register u32 *p asm("$7");
        register s32 n asm("$5");
        register u32 q1 asm("$6");
        register u32 q2 asm("$4");
        register u32 q3 asm("$3");
        register u32 q4 asm("$2");

        p = &D_8009B174;
        n = cnt - 1;
        q1 = (*p * n) / cnt;
        q2 = (D_8009B178 * n) / cnt;
        q3 = (D_8009B17C * n) / cnt;
        q4 = (D_8009B180 * n) / cnt;
        D_8009B1B4 = n;
        *p = q1;
        D_8009B178 = q2;
        D_8009B17C = q3;
        D_8009B180 = q4;
    } else if (cnt < 0) {
        register u32 *p asm("$12");
        register u32 v184 asm("$11");
        register u32 inv asm("$4");
        register u32 div2 asm("$3");
        register u32 d1 asm("$9");
        register u32 v188 asm("$10");
        register u32 d2 asm("$7");
        register u32 v18C asm("$8");
        register u32 d3 asm("$5");
        register u32 v190 asm("$6");
        register u32 d4 asm("$4");
        register s32 c2 asm("$2");

        p = &D_8009B174;
        v184 = D_8009B184;
        inv = ~cnt;
        div2 = inv + 1;
        d1 = ((v184 - *p) * inv) / div2;
        v188 = D_8009B188;
        d2 = ((v188 - D_8009B178) * inv) / div2;
        v18C = D_8009B18C;
        d3 = ((v18C - D_8009B17C) * inv) / div2;
        v190 = D_8009B190;
        d4 = ((v190 - D_8009B180) * inv) / div2;
        v184 = v184 - d1;
        *p = v184;
        c2 = cnt + 1;
        D_8009B1B4 = c2;
        v188 = v188 - d2;
        D_8009B178 = v188;
        v18C = v18C - d3;
        D_8009B17C = v18C;
        v190 = v190 - d4;
        D_8009B180 = v190;
    }
    buf[0] = D_8009B174 >> 12;
    buf[1] = D_8009B178 >> 12;
    buf[2] = D_8009B17C >> 12;
    buf[3] = D_8009B180 >> 12;
    func_8006A94C(buf);
}
