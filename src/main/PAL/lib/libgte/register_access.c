#include "common.h"
#include "psyq/gte_macros.h"

/*
 * PSY-Q 3.5 libgte object reg03.o (LIBGTE.A): the direct GTE data-register
 * writers, SetVertex0..SetDQB.  Exports SetVertex0/1/2, SetVertexTri,
 * SetRGBfifo, SetIR123, SetIR0, SetSZfifo3/4, SetSXSYfifo, SetRii, SetMAC123,
 * SetData32, SetDQA, SetDQB.  Boundaries byte-matched against reg03.o
 *.  Bodies use the named GTE macros.
 */

void SetVertex0(void *v) { gte_ldv0(v); }
void SetVertex1(void *v) { gte_ldv1(v); }
void SetVertex2(void *v) { gte_ldv2(v); }
void SetVertexTri(void *v0, void *v1, void *v2) {
    gte_ldv0(v0);
    gte_ldv1(v1);
    gte_ldv2(v2);
}

/* --- SetRGBfifo.s --- */

void SetRGBfifo(void *rgb0, void *rgb1, void *rgb2) {
    gte_lwc2(20, rgb0);
    gte_lwc2(21, rgb1);
    gte_lwc2(22, rgb2);
}
void SetIR123(s32 ir1, s32 ir2, s32 ir3) {
    gte_mtc2(ir1, 9);
    gte_mtc2(ir2, 10);
    gte_mtc2(ir3, 11);
}
void SetIR0(s32 ir0) { gte_mtc2(ir0, 8); }
void SetSZfifo3(s32 sz1, s32 sz2, s32 sz3) {
    gte_mtc2(sz1, 17);
    gte_mtc2(sz2, 18);
    gte_mtc2(sz3, 19);
}
void SetSZfifo4(s32 sz0, s32 sz1, s32 sz2, s32 sz3) {
    gte_mtc2(sz0, 16);
    gte_mtc2(sz1, 17);
    gte_mtc2(sz2, 18);
    gte_mtc2(sz3, 19);
}
void SetSXSYfifo(s32 sxy0, s32 sxy1, s32 sxy2) {
    gte_mtc2(sxy0, 12);
    gte_mtc2(sxy1, 13);
    gte_mtc2(sxy2, 14);
}
void SetRii(s32 r11, s32 r22, s32 r33) {
    gte_ctc2(r11, 0);
    gte_ctc2(r22, 2);
    gte_ctc2(r33, 4);
}
void SetMAC123(s32 mac1, s32 mac2, s32 mac3) {
    gte_mtc2(mac1, 25);
    gte_mtc2(mac2, 26);
    gte_mtc2(mac3, 27);
}
void SetData32(s32 data) { gte_mtc2(data, 30); }
void SetDQA(s32 dqa) { gte_ctc2(dqa, 27); }
void SetDQB(s32 dqb) { gte_ctc2(dqb, 28); }
