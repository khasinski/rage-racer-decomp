#include "common.h"
#include "psyq/gte.h"

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s16 pad;
} SVec;

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} LVec;

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s16 dur;
    s16 rot;
    s16 pad;
} KF;

extern volatile s32 D_801E408C;
extern u8 *D_801E4128;
extern s32 D_801E4330;
extern volatile s32 D_801E4338;
extern s16 D_801E433E;
extern s32 D_801E4340;
extern s32 D_801E4344;
extern s32 D_801E4348;
extern s32 D_801E434C;
extern s32 D_801E4350;
extern s32 D_801E4354;
extern s32 D_801E4358;
extern KF *D_801E6C88;

void func_8001A530(Matrix *mtx, s32 angle);
void func_8001A5A0(Matrix *mtx, s32 angle);
void func_8001A4C0(Matrix *mtx, s32 angle);
void func_80069568(Matrix *lhs, Matrix *rhs);
void func_80069458(Matrix *lhs, Matrix *rhs);
s32 *func_80069678(s32 *matrix, s32 *vec, s32 *out);

void func_8003EC98(void) {
    Matrix mtx0;
    Matrix mtx1;
    SVec vin;
    LVec vout;
    volatile s32 pad[6];
    register s32 *cnt asm("$7");
    register u8 *base asm("$6");
    register KF *kp asm("$5");
    register s32 i asm("$4");
    register s32 counter asm("$3");
    s32 c;
    s32 r4354;

    cnt = &D_801E4330;
    c = *cnt;
    base = D_801E4128;
    if (c <= 0) {
        return;
    }
    *cnt = c + 1;

    counter = D_801E4338;
    i = D_801E433E;
    kp = D_801E6C88;
    counter = counter + 1;
    __asm__("" : "=r"(counter) : "0"(counter));
    {
        KF *rec0 = (KF *)(i * 12 + (s32)kp);

        D_801E4338 = counter;
        __asm__ volatile("" ::: "memory");
        if (rec0->dur == counter) {
            register s32 ni asm("$2");

            ni = i + 1;
            D_801E433E = ni;
            D_801E4338 = 0;
        }
    }

    i = D_801E433E;
    if ((kp + i)->dur == -1) {
        register s32 idx asm("$4");
        register u8 *src asm("$4");
        register KF *r3 asm("$2");
        register s32 off asm("$2");
        register s32 n asm("$3");
        register s32 value asm("$3");
        register s32 *dst asm("$6");
        register s32 word0 asm("$2");
        register s32 word1 asm("$3");
        register s32 word2 asm("$5");

        idx = D_801E408C;
        D_801E433E = 0;
        n = *(s16 *)((u8 *)((idx << 2) + (s32)base) + 8);
        off = n * 12 + 0x50;
        __asm__("" : "=r"(off) : "0"(off));
        r3 = (KF *)((s32)base + off);
        value = *(s16 *)((s32)r3 + 0);
        D_801E4350 = value;
        value = *(s16 *)((s32)r3 + 2);
        D_801E4354 = value;
        value = *(s16 *)((s32)r3 + 4);
        src = (u8 *)((idx << 5) + (s32)base);
        D_801E6C88 = r3;
        D_801E4358 = value;
        __asm__ volatile("" ::: "memory");
        dst = &D_801E4340;
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
        D_801E4338 = 0;
    }

    {
        register KF *rec asm("$6");
        register s32 t asm("$7");
        register Matrix *m0 asm("$4");
        s32 t0v;

        rec = D_801E6C88 + (s32)D_801E433E;
        t = D_801E4338;
        t0v = rec->dur - t;
        D_801E4350 = (rec[1].x * t + rec->x * t0v) / rec->dur;
        __asm__ volatile("" ::: "memory");
        r4354 = (rec[1].y * t + rec->y * t0v) / rec->dur;
        D_801E4354 = r4354;
        __asm__ volatile("" ::: "memory");
        D_801E4358 = (rec[1].z * t + rec->z * t0v) / rec->dur;
        vin.x = 0;
        vin.y = 0;
        m0 = &mtx0;
        __asm__("" : "=r"(m0) : "0"(m0));
        __asm__ volatile("" ::: "memory");
        vin.z = -rec->rot * 4;
        func_8001A530(m0, 0x800 - r4354);
    }

    func_8001A5A0(&mtx1, D_801E4350);
    func_80069568(&mtx0, &mtx1);
    func_8001A4C0(&mtx0, D_801E4358);
    func_80069458(&mtx1, &mtx0);
    func_80069678((s32 *)&mtx1, (s32 *)&vin, (s32 *)&vout);

    D_801E4340 += vout.x / 4;
    D_801E4344 += vout.y / 4;
    D_801E4348 += vout.z / 4;
}
