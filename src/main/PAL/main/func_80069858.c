#include "common.h"
#include "psyq/gte_macros.h"

/*
 * PSY-Q libgte low-level GTE (COP2) accessor routines.
 *
 * This translation unit is the set of small "set control register / load
 * data register / run one COP2 command" leaf routines from the SDK. Each
 * function body is expressed with the named GTE macros in
 * include/psyq/gte_macros.h; ordinary CPU work (the fixed-point shifts) stays
 * in C. The `.align 4` directives reproduce the 16-byte function alignment the
 * retail object uses from func_80069A18 onward (the padding shows up as the
 * zero-word "functions" func_80069A70 / func_80069A84).
 */

/* --- func_80069858.s --- */

void func_80069858(void *m) { gte_SetRotMatrix(m); }
void func_80069888(void *m) { gte_SetLightMatrix(m); }
void func_800698B8(void *m) { gte_SetColorMatrix(m); }
void func_800698E8(void *m) { gte_SetTransMatrix(m); }

void func_80069908(void *v) { gte_ldv0(v); }
void func_80069918(void *v) { gte_ldv1(v); }
void func_80069928(void *v) { gte_ldv2(v); }
void func_80069938(void *v0, void *v1, void *v2) {
    gte_ldv0(v0);
    gte_ldv1(v1);
    gte_ldv2(v2);
}

/* --- func_80069958.s --- */

void func_80069958(void *a, void *b, void *c) {
    gte_lwc2(20, a);
    gte_lwc2(21, b);
    gte_lwc2(22, c);
}
void func_8006996C(s32 a, s32 b, s32 c) {
    gte_mtc2(a, 9);
    gte_mtc2(b, 10);
    gte_mtc2(c, 11);
}
void func_80069980(s32 a) { gte_mtc2(a, 8); }
void func_8006998C(s32 a, s32 b, s32 c) {
    gte_mtc2(a, 17);
    gte_mtc2(b, 18);
    gte_mtc2(c, 19);
}
void func_800699A0(s32 a, s32 b, s32 c, s32 d) {
    gte_mtc2(a, 16);
    gte_mtc2(b, 17);
    gte_mtc2(c, 18);
    gte_mtc2(d, 19);
}
void func_800699B8(s32 a, s32 b, s32 c) {
    gte_mtc2(a, 12);
    gte_mtc2(b, 13);
    gte_mtc2(c, 14);
}
void func_800699CC(s32 a, s32 b, s32 c) {
    gte_ctc2(a, 0);
    gte_ctc2(b, 2);
    gte_ctc2(c, 4);
}
void func_800699E0(s32 a, s32 b, s32 c) {
    gte_mtc2(a, 25);
    gte_mtc2(b, 26);
    gte_mtc2(c, 27);
}
void func_800699F4(s32 a) { gte_mtc2(a, 30); }
void func_80069A00(s32 a) { gte_ctc2(a, 27); }
void func_80069A0C(s32 a) { gte_ctc2(a, 28); }

__asm__(".align 4");
void func_80069A18(s32 a, s32 b, s32 c) {
    s32 x = a << 4, y = b << 4, z = c << 4;
    gte_ctc2(x, 13);
    gte_ctc2(y, 14);
    gte_ctc2(z, 15);
}
__asm__(".align 4");
void func_80069A38(s32 a, s32 b, s32 c) {
    s32 x = a << 4, y = b << 4, z = c << 4;
    gte_ctc2(x, 21);
    gte_ctc2(y, 22);
    gte_ctc2(z, 23);
}
__asm__(".align 4");
void func_80069A58(s32 a, s32 b) {
    s32 x = a << 16, y = b << 16;
    gte_ctc2(x, 24);
    gte_ctc2(y, 25);
}
__asm__(".align 4");
void func_80069A78(s32 a) { gte_ctc2(a, 26); }

/* --- func_80069A88.s .. func_80069C24.s --- */

__asm__(".align 4");
void func_80069A88(void *in, void *out) {
    gte_ldir(in);
    gte_nop();
    gte_lc();
    gte_stir(out);
}

void func_80069AB0(void *in, void *rgb, s32 ir0, void *out) {
    gte_ldir(in);
    gte_lwc2(6, rgb);
    gte_mtc2(ir0, 8);
    gte_nop();
    gte_dpcl();
    gte_swc2(22, out);
}

void func_80069AD8(void *v0, void *v1, void *v2, s32 ir0, void *o0, void *o1,
                   void *o2) {
    (void)o0;
    (void)o1;
    (void)o2;
    gte_dpct3(v0, v1, v2, ir0);
}

void func_80069B14(void *in, s32 ir0, void *out) {
    gte_ldir(in);
    gte_mtc2(ir0, 8);
    gte_nop();
    gte_intpl();
    gte_swc2(22, out);
}

void *func_80069B38(void *in, void *out) {
    register void *p asm("$5") = out;
    gte_ldir(in);
    gte_nop();
    gte_sqr12();
    gte_stmac(p);
    return p;
}

void *func_80069B60(void *in, void *out) {
    register void *p asm("$5") = out;
    gte_ldir(in);
    gte_nop();
    gte_sqr0();
    gte_stmac(p);
    return p;
}

s32 func_80069B88(s32 a, s32 b, s32 c) {
    s32 r;
    gte_mtc2(a, 17);
    gte_mtc2(b, 18);
    gte_mtc2(c, 19);
    gte_nop();
    gte_avsz3();
    gte_mfc2(r, 7);
    return r;
}

s32 func_80069BA8(s32 a, s32 b, s32 c, s32 d) {
    s32 r;
    gte_mtc2(a, 16);
    gte_mtc2(b, 17);
    gte_mtc2(c, 18);
    gte_mtc2(d, 19);
    gte_nop();
    gte_avsz4();
    gte_mfc2(r, 7);
    return r;
}

void func_80069BCC(void *m, void *v, void *out) {
    gte_op12_diag(m, v, out);
}

void func_80069C24(void *m, void *v, void *out) {
    gte_op0_diag(m, v, out);
}
