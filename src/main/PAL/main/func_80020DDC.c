#include "common.h"

typedef struct {
    u8 pad[0x10];
    s32 elapsedTime;
} RaceProg;

extern s32 D_8019CB74;
extern s32 D_801E6DA0;
extern u16 D_801E436A;
extern u16 D_801E436E;
extern s32 D_801E6C78;
extern s32 D_801E40B8;
extern s32 D_801F17B0;
extern s32 D_8019CE0C;
extern RaceProg *D_801E4FAC;
extern s32 D_801E4D0C;
extern s32 D_801E4B94;
extern s32 D_8019C8EC;

void func_80033AA0(s32, s32);
void func_800204F4(s32);
void func_800206B8(s32);
void func_8005D6EC(s32);
void func_80020D90(void);
void func_80018410(void);
void func_80020B08(void);
void func_800201D4(void);

void func_80020DDC(void) {
    s32 lim1 = D_801E6DA0;
    s32 lim0 = D_801E6C78;
    s32 st;
    s32 t;

    if (D_801E436A & 0x860) {
        lim1 <<= 2;
        lim0 <<= 2;
    }

    switch (D_8019CB74) {
    case 0:
        D_801E40B8 -= 8;
        func_80033AA0(D_801E40B8, 0x49);
        if (D_801E40B8 == 0) D_8019CB74 = 1;
        func_800204F4(0);
        goto L428;
    case 1:
        func_800204F4(0);
        if (D_801E436E & 0x860) {
            D_8019CB74 = 2;
            D_801E40B8 = 0;
        }
        goto L428;
    case 2:
        D_801E40B8 += 8;
        func_800204F4(D_801E40B8);
        if ((u32)D_801E40B8 >= 129) D_8019CB74 = 3;
        goto L428;
    case 3:
        D_801E40B8 -= 8;
        func_800206B8(D_801E40B8);
        if (D_801E40B8 == 0) D_8019CB74 = 4;
        goto L428;
    case 4:
        D_801E40B8 += 1;
        if ((u32)D_801E40B8 < 121) goto L234;
        if (D_801F17B0 == 0) goto L248;
        func_8005D6EC((D_801E436A & 0x860) ? 0x10 : 0xf);
        t = D_801F17B0;
        if (t >= lim1) {
            D_801F17B0 = t - lim1;
            D_801E4FAC->elapsedTime += lim1;
        } else {
            s32 e = D_801E4FAC->elapsedTime;
            D_801F17B0 = 0;
            D_801E4FAC->elapsedTime = e + t;
        }
    L234:
        if (D_801F17B0 != 0) goto L420;
    L248:
        D_801E40B8 = 0;
        if (D_8019CE0C == 0) goto Lstore7;
        st = 5;
        goto Lstore;
    case 5:
        func_8005D6EC(0x11);
        if (!(D_801E436E & 0x860)) goto L420;
        st = 6;
    Lstore:
        D_8019CB74 = st;
        goto L420;
    case 6:
        func_80020D90();
        if (D_8019CE0C == 0) { st = 7; goto Lstore; }
        func_8005D6EC((D_801E436A & 0x860) ? 0x10 : 0xf);
        t = D_8019CE0C;
        if (t >= lim0) {
            D_8019CE0C = t - lim0;
            D_801E4FAC->elapsedTime += lim0;
        } else {
            s32 e = D_801E4FAC->elapsedTime;
            D_8019CE0C = 0;
            D_801E4FAC->elapsedTime = e + t;
        }
        if (D_8019CE0C != 0) goto L420;
    Lstore7:
        st = 7;
        goto Lstore;
    case 7:
        func_80020D90();
        func_8005D6EC(0x11);
        if (!(D_801E436E & 0x860)) goto L420;
        if (D_801E4D0C != 0) goto L420;
        if (D_801E4B94 != 0) { st = 8; goto Lstore; }
        func_80018410();
        st = 8;
        goto Lstore;
    case 8:
        if (D_8019C8EC != 0)
            D_801E40B8 += 1;
        else
            D_801E40B8 += 2;
        func_80033AA0(D_801E40B8, 0x49);
        if ((u32)D_801E40B8 < 0x100) goto L420;
        break;
    default:
        goto L428;
    }
    func_80020B08();
L420:
    func_800206B8(0);
L428:
    func_800201D4();
}
