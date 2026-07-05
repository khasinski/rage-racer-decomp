#include "common.h"
#include "psyq/gpu.h"

extern s32 D_8007BED8;
extern s32 D_8007C704;
extern s32 D_8009F0B8;
extern u8 *D_8019CAFC;
extern u8 *D_8019C904;
extern u8 *D_801F17A8;
extern Rect D_8007BEDC;
extern Rect D_8007BEE4;
extern u16 D_801E444C[];
extern u16 D_801E6F2C[];

void func_80042C94(void);
s32 func_80017C78(s32 assetIndex, void *dst);
void func_8001A3C0(void *arg0);
void func_8001A498(void);
void func_80034DCC(void *arg0);
void func_8005B768(s32 arg0, void *arg1, void *arg2, s32 arg3);
s32 func_8005B89C(void);
void StoreImage(Rect *rect, void *data) asm("func_80065B88");
void DrawSync(s32 mode) asm("func_800658FC");

void func_800180CC(void) {
    register u8 *loaded asm("$16");
    register u8 *base asm("$17");
    register u8 *next asm("$2");
    register s32 nextState asm("$3");

    switch (D_8007BED8) {
    case 1:
        base = (u8 *)&D_8009F0B8;
        loaded = (u8 *)func_80017C78(1, base);
        if (loaded != 0) {
            func_8001A498();
            __asm__ volatile("" ::: "memory");
            next = loaded + (s32)base;
            D_801F17A8 = next;
            D_8007BED8 = 2;
        }
        break;
    case 2:
        loaded = (u8 *)func_80017C78(2, D_801F17A8);
        if (loaded != 0) {
            nextState = 3;
            next = D_801F17A8;
            goto setNextBuffer;
        }
        break;
    case 3:
        if (func_80017C78(3, D_8019CAFC) != 0) {
            func_8005B768(0, D_801F17A8, D_8019CAFC, 0);
            D_8007BED8 = 4;
        }
        break;
    case 4:
        if ((func_8005B89C() << 16) != 0) {
            D_8007BED8 = 5;
        }
        break;
    case 5:
        loaded = (u8 *)func_80017C78(4, D_8019CAFC);
        if (loaded != 0) {
            func_80034DCC(D_8019CAFC);
            next = D_8019CAFC;
            __asm__ volatile("" : "=r"(next) : "0"(next));
            nextState = 6;
setNextBuffer:
            D_8007BED8 = nextState;
            __asm__ volatile("" ::: "memory");
            next = loaded + (s32)next;
            D_8019CAFC = next;
        }
        break;
    case 6:
        if (func_80017C78(5, D_8019CAFC) != 0) {
            register u8 *finalBase asm("$2");

            func_8001A3C0(D_8019CAFC);
            StoreImage(&D_8007BEDC, D_801E444C);
            StoreImage(&D_8007BEE4, D_801E6F2C);
            DrawSync(0);
            finalBase = D_8019CAFC;
            D_801E444C[0] = 0;
            D_8007BED8 = 0;
            D_8019C904 = finalBase;
        }
        break;
    }
}

s32 func_800182D0(void) {
    register s32 state asm("$16");

    if (D_8007BED8 != 0) {
        return 1;
    }

    state = 2;
    if (D_8007C704 == state) {
        D_8007C704 = 0;
        return 0;
    }

    func_80042C94();
    D_8007C704 = state;
    D_8007BED8 = 1;
    return 1;
}
