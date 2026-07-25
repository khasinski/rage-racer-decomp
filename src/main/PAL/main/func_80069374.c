#include "common.h"
#include "psyq/gte_macros.h"

/* Read GTE rotation matrix + translation (control regs $0..$7) into p[0..7]. */
void func_80069374(volatile u32 *p) {
    register u32 t0 asm("$8");
    register u32 t1 asm("$9");
    register u32 t2 asm("$10");
    register u32 t3 asm("$11");
    register u32 t4 asm("$12");

    gte_cfc2(t0, 0);
    gte_cfc2(t1, 1);
    gte_cfc2(t2, 2);
    gte_cfc2(t3, 3);
    gte_cfc2(t4, 4);
    p[0] = t0;
    p[1] = t1;
    p[2] = t2;
    p[3] = t3;
    p[4] = t4;
    gte_cfc2(t0, 5);
    gte_cfc2(t1, 6);
    gte_cfc2(t2, 7);
    p[5] = t0;
    p[6] = t1;
    p[7] = t2;
}

/* Read GTE light matrix + back-color (control regs $8..$15) into p[0..7]. */
void func_800693BC(volatile u32 *p) {
    register u32 t0 asm("$8");
    register u32 t1 asm("$9");
    register u32 t2 asm("$10");
    register u32 t3 asm("$11");
    register u32 t4 asm("$12");

    gte_cfc2(t0, 8);
    gte_cfc2(t1, 9);
    gte_cfc2(t2, 10);
    gte_cfc2(t3, 11);
    gte_cfc2(t4, 12);
    p[0] = t0;
    p[1] = t1;
    p[2] = t2;
    p[3] = t3;
    p[4] = t4;
    gte_cfc2(t0, 13);
    gte_cfc2(t1, 14);
    gte_cfc2(t2, 15);
    p[5] = t0;
    p[6] = t1;
    p[7] = t2;
}

/* Read GTE color matrix + far-color (control regs $16..$23) into p[0..7]. */
void func_80069404(volatile u32 *p) {
    register u32 t0 asm("$8");
    register u32 t1 asm("$9");
    register u32 t2 asm("$10");
    register u32 t3 asm("$11");
    register u32 t4 asm("$12");

    gte_cfc2(t0, 16);
    gte_cfc2(t1, 17);
    gte_cfc2(t2, 18);
    gte_cfc2(t3, 19);
    gte_cfc2(t4, 20);
    p[0] = t0;
    p[1] = t1;
    p[2] = t2;
    p[3] = t3;
    p[4] = t4;
    gte_cfc2(t0, 21);
    gte_cfc2(t1, 22);
    gte_cfc2(t2, 23);
    p[5] = t0;
    p[6] = t1;
    p[7] = t2;
}

/* trailing alignment padding (3 nops) that fills this TU to the next subsegment */
const u32 func_8006944C[3] __attribute__((section(".text"))) = { 0, 0, 0 };
