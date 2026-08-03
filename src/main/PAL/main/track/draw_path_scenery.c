#include "common.h"
#include "psyq/gte.h"
#include "game/state.h"
#include "game/render.h"
#include "game/track.h"
#include "game/race.h"

/* The looping prop's live orientation: three 12-bit angles copied wholesale out
 * of the current rotation keyframe by InitPathScenery, which sees the same
 * eight bytes as one Blk8. */
extern s16 g_PathSceneryRot[3] asm("D_801E4DC8");
extern s16 g_PathSceneryX asm("D_801E4DB8");
extern s32 g_ModelBankCount asm("D_801E4168");
extern s32 g_ScratchRenderMode asm("D_1F800084");

void func_80017794(void *arg0, void *arg1, Matrix *mtx);

void DrawPathScenery(void) asm("func_80040730");
void DrawPathScenery(void) {
    Matrix mtx0;
    Matrix mtx1;
    volatile s32 pad[4];
    s32 drawId;
    s32 frameValue;
    register s16 *anglePtr asm("$16");
    Matrix *mtx1Ptr;
    Matrix *mtx0Ptr;
    void *scratchVec;

    mtx0Ptr = &mtx0;
    __asm__("" : "=r"(mtx0Ptr) : "0"(mtx0Ptr));
    anglePtr = &g_PathSceneryRot[1];
    mtx1Ptr = &mtx1;

    BuildRotMatrixY(mtx0Ptr, 0x800 - anglePtr[0]);
    BuildRotMatrixX(mtx1Ptr, g_PathSceneryRot[0]);
    MulMatrix2(&mtx0, mtx1Ptr);
    MulMatrix2((Matrix *)0x1F800028, mtx1Ptr);
    BuildRotMatrixZ(&mtx0, g_PathSceneryRot[2]);
    MulMatrix2(mtx1Ptr, &mtx0);

    SelectModelBank(1);
    scratchVec = (void *)0x1F80011C;
    __asm__("" : "=r"(scratchVec) : "0"(scratchVec));
    anglePtr = (s16 *)((u8 *)anglePtr - 0x12);
    func_80017794(scratchVec, anglePtr, &mtx0);
    frameValue = g_ModelBankCount;
    *(s32 *)0x1F800084 = 0;
    drawId = 1;
    if (frameValue >= 0x24) {
        drawId = 0x23;
    }
    SubmitModel((void *)0x1F800000, drawId);

    {
        s32 base;
        s32 acc;
        s32 tmp;

        base = g_SceneTimer;
        acc = base << 2;
        acc += base;
        tmp = acc << 5;
        acc += tmp;
        acc <<= 1;
        acc += base;
        BuildRotMatrixY(mtx1Ptr, acc & 0xFFF);
    }
    MulMatrix2(&mtx0, mtx1Ptr);
    func_80017794((void *)0x1F80011C, anglePtr, mtx1Ptr);
    frameValue = g_ModelBankCount;
    g_ScratchRenderMode = 0;
    drawId = 1;
    if (frameValue >= 0x25) {
        drawId = 0x24;
    }
    SubmitModel((void *)0x1F800000, drawId);
}

/* g_PlayerCar + 0x3C. Named for its identity only: this caller branches on
 * its sign while the other user treats it as an unsigned 0..0x800 width
 * fraction, and docs/names.md 15g leaves that unreconciled. */
extern s32 g_PlayerField3C asm("D_8009E710");
extern s32 g_PlayerSpeed asm("D_8009E778");
extern s32 g_PlayerTrackPoint asm("D_8009E704");

s32 func_80068634(s32 arg0);
void func_8005BEA8(s32 arg0, s32 arg1);

void func_8004087C(s16 arg) {
    s32 base;
    s16 *p;
    s16 *cur;
    s16 *end;
    s32 data;
    s16 lo;
    s32 s0, s1, s2, s3;
    s32 val;
    s32 t;
    s32 a0v, a1v;

    data = 0;
    base = (s32)g_TrackEventData;
    p = (s16 *)(base + 0x1B7C);
    end = (s16 *)(base + 0x1C6C);
    cur = p;
    do {
        lo = cur[0];
        if (arg < lo) {
        } else {
        if (cur[1] >= arg) {
            data = p[2];
            break;
        }
        }
        if (lo == -1) {
            break;
        }
        p = cur + 4;
        cur = p;
    } while ((s32)p < (s32)end);

    if (!(data == 0)) {
    switch (0) { default:
    s0 = g_PlayerField3C;
    if (!(s0 >= 0)) {
    s0 += 0x100;
    if (s0 <= 0) {
        break;
    }
    s0 = 0;
    } else {
    s0 -= 0x100;
    if (s0 >= 0) {
    } else {
    s0 = 0;
    }
    }
    }
    if (s0 != 0) {
        s0 = (s0 * g_PlayerSpeed) / 12775;
        t = *(s32 *)0x1F80001C - 0xC00;
        s3 = (t + g_TrackPoints[g_PlayerTrackPoint].angle) & 0xFFF;
        if (s0 < 0 && (data & 2) > 0) {
            val = s0 * func_80068634(s3);
            if (val < 0) {
                val += 0xFFF;
            }
            s1 = -(s0 + (val >> 12));
            val = (-s0) * func_80068634(s3);
            if (val < 0) {
                val += 0xFFF;
            }
            s2 = -(s0 + (val >> 12));
        } else if (s0 > 0 && (data & 1) > 0) {
            val = s0 * func_80068634(s3);
            if (val < 0) {
                val += 0xFFF;
            }
            s2 = s0 + (val >> 12);
            val = (-s0) * func_80068634(s3);
            if (val < 0) {
                val += 0xFFF;
            }
            s1 = s0 + (val >> 12);
        } else {
            s2 = 0;
            s1 = 0;
        }
    } else {
        s2 = 0;
        s1 = 0;
    }
    if (g_MirrorMode) {
        a0v = s2;
        a1v = s1;
    } else {
        a0v = s1;
        a1v = s2;
    }
    } else {
    a0v = 0;
    a1v = 0;
    }
    func_8005BEA8(a0v, a1v);
}

s32 func_80068568(s32 arg0);
void func_8005C31C(s32 arg0, s32 arg1, s32 arg2);

typedef struct TrackSeg {
    s32 lo;    /* 0x00 */
    s32 hi;    /* 0x04 */
    u16 f08;   /* 0x08 */
    u16 f0A;   /* 0x0A */
    s32 f0C;   /* 0x0C */
    s32 f10;   /* 0x10 */
    s32 f14;   /* 0x14 */
} TrackSeg;    /* size 0x18 */

void UpdatePointAmbience(s32 arg) asm("func_80040ADC");
void UpdatePointAmbience(s32 arg) {
    s32 base;
    s32 startp;
    TrackSeg *seg;
    register s32 v1 asm("v1");
    s32 t0;
    u16 a1raw;
    u16 t1raw;
    s32 sentinel;
    register s32 s0v asm("s0");
    s32 s1;
    s32 s2;
    s32 s3;
    s32 s4;
    s32 s5;
    s32 s6;
    s32 a2v;
    s32 a1s;
    s32 v0;
    s32 angle, sinv;

    base = (s32)g_TrackEventData;
    startp = base + 0x1C6C;
    if (g_RaceSeries != 0) {
        arg = g_TrackLength - arg;
    }

    s5 = 0;
    a2v = 0;
    s2 = 0;
    s0v = 0;
    sentinel = -1;
    seg = (TrackSeg *)startp;
loop:
    v1 = seg->lo;
    t0 = seg->hi;
    switch (0) { default:
    if (!(v1 == sentinel)) {
    a1raw = seg->f08;
    t1raw = seg->f0A;
    if (!(arg < v1)) {
    if (!(t0 < arg)) {
    switch (0) { default:
    v0 = a1raw << 16;
    a1s = v0 >> 16;
    if (arg < v1 + a1s) {
        v0 = arg - v1;
    } else {
        v0 = t1raw << 16;
        __asm__("" : "=r"(v0) : "0"(v0));
        a1s = v0 >> 16;
        if ((t0 - a1s) < arg) {
            v0 = t0 - arg;
        } else {
            break;
        }
    }
    v1 = (v0 * 48) / a1s;
    s2 = v1;
    goto load;
    }
    s2 = 0x30;
load:
    s3 = seg->f0C;
    s4 = seg->f10;
    s6 = seg->f14;
    break;
    }
    }
    s0v++;
    seg++;
    if (s0v < 2) {
        goto loop;
    }

    }
}
    v0 = s2 << 16;
    s0v = v0 >> 16;
    if (s0v != 0) {
        s3 -= *(s32 *)0x1F800008;
        s4 -= *(s32 *)0x1F800010;
        v0 = SquareRoot12((s3 * s3) / 4 + (s4 * s4) / 4);
        v0 = s2 - (v0 >> 11);
        s1 = v0;
        __asm__("" : "=r"(v0) : "0"(v0));
        v0 = s0v < (s16)v0;
        if (v0) {
            s1 = s2;
        }
        if ((s16)s1 < 0) {
            s1 = 0;
        }
        angle = Atan2(s3, s4);
        v1 = *(s32 *)0x1F80001C;
        v1 -= 0xC00;
        v1 += angle;
        s0v = v1 & 0xFFF;
        sinv = func_80068568(s0v);
        v1 = s1 << 16;
        s1 = v1 >> 16;
        v0 = s2 + (s1 * sinv) / 4096;
        s5 = v0 + 0x20;
        sinv = func_80068568(s0v);
        v1 = -s1;
        v0 = s2 + (v1 * sinv) / 4096;
        a2v = v0 + 0x20;
        if (s6 < 0) {
            s6 = -s6;
        }
    }

    if (s6 == 1) {
        if (g_MirrorMode != 0) {
            func_8005C31C(2, (s16)s5, (s16)a2v);
        } else {
            func_8005C31C(2, (s16)a2v, (s16)s5);
        }
    } else {
        if (g_MirrorMode != 0) {
            func_8005C31C(3, (s16)s5, (s16)a2v);
        } else {
            func_8005C31C(3, (s16)a2v, (s16)s5);
        }
    }
}
