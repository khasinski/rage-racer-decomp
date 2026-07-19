#include "common.h"
#include "psyq/gte.h"

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
extern s32 D_801E4030;
extern s32 D_8009E694;

void func_8001A530(Matrix *mtx, s32 angle);
void func_80069568(void *a, void *b);
void func_80069678(void *a, void *b, void *c);
void func_80069858(void *a);
void func_800698E8(void *a);
void func_80029E50(void *a, s32 b);
void func_800296B4(void *a, s32 b);

void func_8004123C(void) {
    Matrix mtx;
    volatile s32 pad[10];
    Obj *p;
    s32 i;
    s32 shift;
    s32 vis;
    s32 flags;

    p = D_801E4B2C;
    i = 0;
    if (D_801E4BBC <= 0) {
        return;
    }

    do {
        if (p->id == -1) {
            goto next;
        }
        shift = p->f4 / 2048;
        {
            register s32 r2 asm("$2");
            register s32 r3 asm("$3");
            r3 = p->fC / 2048;
            r2 = (s32)D_801E6828;
            r3 = ((s32 *)r2)[r3];
            r2 = 1 << shift;
            r2 &= r3;
            if (r2 == 0) {
                goto next;
            }
        }

        func_8001A530(&mtx, p->f2);
        func_80069568((void *)0x1F800028, &mtx);
        {
            register s32 ov asm("$2");
            register s32 cv asm("$3");
            ov = (u16)p->f4;
            cv = *(u16 *)0x1F800008;
            ov -= cv;
            *(s16 *)0x1F80011C = ov;
            ov = (u16)p->f8;
            cv = *(u16 *)0x1F80000C;
            ov -= cv;
            *(s16 *)0x1F80011E = ov;
            ov = (u16)p->fC;
            cv = *(u16 *)0x1F800010;
            ov -= cv;
            *(s16 *)0x1F800120 = ov;
        }
        func_80069678((void *)0x1F800028, (void *)0x1F80011C, (void *)0x1F800124);
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

        flags = p->flags;
        if (flags & 8) {
            *(s32 *)0x1F800084 = ((D_8009E694 & 0x10) == 0) << 16;
        } else if (flags & 4) {
            *(s32 *)0x1F800084 = 0x10000;
        } else {
            *(s32 *)0x1F800084 = 0;
        }

        if (D_801E4030 ? (p->flags & 2) : (p->flags & 1)) {
            func_80029E50((void *)0x1F800000, p->id);
        } else {
            func_800296B4((void *)0x1F800000, p->id);
        }

    next:
        i++;
        p++;
    } while (i < D_801E4BBC);
}
