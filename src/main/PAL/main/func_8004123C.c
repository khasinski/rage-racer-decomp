#include "common.h"
#include "psyq/gte.h"
#include "game/state.h"
#include "game/render.h"

typedef struct {
    s16 id;    /* 0x0 */
    s16 f2;    /* 0x2 */
    s32 f4;    /* 0x4 */
    s32 f8;    /* 0x8 */
    s32 fC;    /* 0xC */
    s32 flags; /* 0x10 */
} Obj;

extern Obj *D_801E4B2C;
extern s32 D_801E4BBC;
extern s32 *D_801E6828;
extern s32 g_IsEnvironmentMode4 asm("D_801E4030");

void func_80069858(void *a);
void func_800698E8(void *a);

/*
 * Draw loop over the world-object array D_801E4B2C (D_801E4BBC entries). For
 * each visible object (id != -1, passing the per-sector visibility bitmask test
 * against D_801E6828) it builds a Z-rotation matrix in the scratchpad
 * (0x1F800028), transforms the object position through the GTE
 * (0x1F80011C -> 0x1F800124), sets the primitive shade/semi-trans mode word at
 * 0x1F800084, then dispatches a prim builder (GameSubmitCourseModel2 / GameSubmitCourseModel)
 * on the scratchpad OT at 0x1F800000.
 */
void GameDrawCourseObjects(void) asm("func_8004123C");
void GameDrawCourseObjects(void) {
    Matrix mtx;
    volatile s32 pad[10];
    Obj *obj;
    s32 i;
    s32 visShift;
    s32 vis;
    s32 flags;

    obj = D_801E4B2C;
    i = 0;
    if (D_801E4BBC <= 0) {
        return;
    }

    do {
        if (obj->id == -1) {
            goto next;
        }
        visShift = obj->f4 / 2048;  /* per-sector visibility bit index */
        {
            register s32 r2 asm("$2");
            register s32 r3 asm("$3");
            r3 = obj->fC / 2048;
            r2 = (s32)D_801E6828;
            r3 = ((s32 *)r2)[r3];
            r2 = 1 << visShift;
            r2 &= r3;
            if (r2 == 0) {
                goto next;
            }
        }

        GameBuildRotMatrixY(&mtx, obj->f2);
        MulMatrix2((void *)0x1F800028, &mtx);
        {
            register s32 ov asm("$2");
            register s32 cv asm("$3");
            ov = (u16)obj->f4;
            cv = *(u16 *)0x1F800008;
            ov -= cv;
            *(s16 *)0x1F80011C = ov;
            ov = (u16)obj->f8;
            cv = *(u16 *)0x1F80000C;
            ov -= cv;
            *(s16 *)0x1F80011E = ov;
            ov = (u16)obj->fC;
            cv = *(u16 *)0x1F800010;
            ov -= cv;
            *(s16 *)0x1F800120 = ov;
        }
        ApplyMatrix((void *)0x1F800028, (void *)0x1F80011C, (void *)0x1F800124);
        {
            register s32 a asm("$2");
            register s32 b asm("$3");
            a = *(s32 *)0x1F800124;
            b = *(s32 *)0x1F80012C;
            a <<= 2;
            *(s32 *)0x1F800148 = a;
            a = *(s32 *)0x1F800128;
            b <<= 2;
            *(s32 *)0x1F800150 = b;
            a <<= 2;
            *(s32 *)0x1F80014C = a;
        }
        func_80069858(&mtx);
        func_800698E8((void *)0x1F800134);

        flags = obj->flags;
        if (flags & 8) {
            *(s32 *)0x1F800084 = ((g_AnimTimer & 0x10) == 0) << 16;
        } else if (flags & 4) {
            *(s32 *)0x1F800084 = 0x10000;
        } else {
            *(s32 *)0x1F800084 = 0;
        }

        if (g_IsEnvironmentMode4 ? (obj->flags & 2) : (obj->flags & 1)) {
            GameSubmitCourseModel2((void *)0x1F800000, obj->id);
        } else {
            GameSubmitCourseModel((void *)0x1F800000, obj->id);
        }

    next:
        i++;
        obj++;
    } while (i < D_801E4BBC);
}
