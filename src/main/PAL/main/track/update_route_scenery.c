#include "common.h"
#include "psyq/gte.h"
#include "game/render.h"

typedef struct SVec {
    s16 x;
    s16 y;
    s16 z;
    s16 pad;
} SVec;

typedef struct LVec {
    s32 x;
    s32 y;
    s32 z;
} LVec;

typedef struct KF {
    s16 x;
    s16 y;
    s16 z;
    s16 dur;
    s16 rot;
    s16 pad;
} KF;

extern volatile s32 g_RaceSeries asm("D_801E408C");
extern u8 *g_RouteSceneryData asm("D_801E4128");
extern s32 g_RouteSceneryClock asm("D_801E4330");
extern volatile s32 g_RouteSceneryFrame asm("D_801E4338");
extern s16 g_RouteSceneryKeyIndex asm("D_801E433E");
/* One 4-word position vector at 0x801E4340 -- x, y, z and a fourth word
 * copied straight out of the series header -- followed by a 3-word orientation
 * at 0x801E4350 in 12-bit angle units (4096 to the turn: the yaw below is used
 * as 0x800 - yaw, i.e. reflected about half a turn). The block-copy below
 * writes all four position words through one cursor. */
extern s32 g_RouteSceneryX asm("D_801E4340");
extern s32 g_RouteSceneryY asm("D_801E4344");
extern s32 g_RouteSceneryZ asm("D_801E4348");
extern s32 g_RouteSceneryW asm("D_801E434C");
extern s32 g_RouteSceneryRotX asm("D_801E4350");
extern s32 g_RouteSceneryRotY asm("D_801E4354");
extern s32 g_RouteSceneryRotZ asm("D_801E4358");
extern KF *g_RouteSceneryKeyframe asm("D_801E6C88");


void UpdateRouteScenery(void) asm("func_8003EC98");
void UpdateRouteScenery(void) {
    Matrix mtx0;
    Matrix mtx1;
    SVec vin;
    LVec vout;
    volatile s32 pad[6];
    register s32 *cnt asm("$7");
    u8 *base;
    register KF *kp asm("$5");
    register s32 i asm("$4");
    s32 counter;
    s32 c;
    s32 r4354;

    cnt = &g_RouteSceneryClock;
    c = *cnt;
    base = g_RouteSceneryData;
    if (c <= 0) {
        return;
    }
    *cnt = c + 1;

    counter = g_RouteSceneryFrame;
    i = g_RouteSceneryKeyIndex;
    kp = g_RouteSceneryKeyframe;
    counter = counter + 1;
    {
        KF *rec0 = (KF *)(i * 12 + (s32)kp);

        g_RouteSceneryFrame = counter;
        if (RAW(rec0->dur) == counter) {
            register s32 ni asm("$2");

            ni = i + 1;
            g_RouteSceneryKeyIndex = ni;
            g_RouteSceneryFrame = 0;
        }
    }

    i = g_RouteSceneryKeyIndex;
    if ((kp + i)->dur == -1) {
        s32 idx;
        u8 *src;
        KF *r3;
        s32 off;
        s32 n;
        s32 value;
        register s32 *dst asm("$6");
        register s32 word0 asm("$2");
        s32 word1;
        s32 word2;

        idx = g_RaceSeries;
        g_RouteSceneryKeyIndex = 0;
        n = *(s16 *)((u8 *)((idx << 2) + (s32)base) + 8);
        off = n * 12 + 0x50;
        r3 = (KF *)((s32)base + off);
        value = *(s16 *)((s32)r3 + 0);
        g_RouteSceneryRotX = value;
        value = *(s16 *)((s32)r3 + 2);
        g_RouteSceneryRotY = value;
        value = *(s16 *)((s32)r3 + 4);
        src = (u8 *)((idx << 5) + (s32)base);
        g_RouteSceneryKeyframe = r3;
        g_RouteSceneryRotZ = value;
        __asm__ volatile("" ::: "memory");
        dst = &g_RouteSceneryX;
        __asm__ volatile("" : "=r"(dst) : "0"(dst), "r"(src) : "memory");
        word0 = *(s32 *)(src + 0x10);
        word1 = *(s32 *)(src + 0x14);
        word2 = *(s32 *)(src + 0x18);
        dst[0] = word0;
        dst[1] = word1;
        dst[2] = word2;
        word0 = *(s32 *)(src + 0x1C);
        dst[3] = word0;
        *cnt = 1;
        g_RouteSceneryFrame = 0;
    }

    {
        KF *rec;
        s32 t;
        Matrix *m0;
        s32 t0v;

        /* RAW() on the keyframe terms is what keeps each g_RouteSceneryRot*
         * store from sinking past the next term's loads -- see common.h. */
        rec = g_RouteSceneryKeyframe + (s32)g_RouteSceneryKeyIndex;
        t = g_RouteSceneryFrame;
        t0v = rec->dur - t;
        g_RouteSceneryRotX = (rec[1].x * t + rec->x * t0v) / rec->dur;
        r4354 = (RAW(rec[1].y) * t + RAW(rec->y) * t0v) / RAW(rec->dur);
        g_RouteSceneryRotY = r4354;
        g_RouteSceneryRotZ = (RAW(rec[1].z) * t + RAW(rec->z) * t0v) / RAW(rec->dur);
        vin.x = 0;
        vin.y = 0;
        m0 = &mtx0;
        __asm__("" : "=r"(m0) : "0"(m0));
        vin.z = -RAW(rec->rot) * 4;
        BuildRotMatrixY(m0, 0x800 - r4354);
    }

    BuildRotMatrixX(&mtx1, g_RouteSceneryRotX);
    MulMatrix2(&mtx0, &mtx1);
    BuildRotMatrixZ(&mtx0, g_RouteSceneryRotZ);
    MulMatrix(&mtx1, &mtx0);
    ApplyMatrix((s32 *)&mtx1, (s32 *)&vin, (s32 *)&vout);

    g_RouteSceneryX += vout.x / 4;
    g_RouteSceneryY += vout.y / 4;
    g_RouteSceneryZ += vout.z / 4;
}
