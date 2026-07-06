#include "common.h"
#include "game/waypoint.h"
#include "psyq/gte.h"

extern s32 D_801E4168;
extern TrackWaypointRuntime D_801E4DF4[];

void func_80017A10(s32 arg0);
void func_80017794(void *arg0, void *arg1, Matrix *mtx);
void func_8001A4C0(Matrix *mtx, s32 angle);
void func_8001A530(Matrix *mtx, s32 angle);
void func_80028DEC(void *arg0, s32 arg1);
void func_80069458(Matrix *dst, Matrix *src);
void func_80069568(Matrix *lhs, Matrix *rhs);

static inline void ClearScratchRenderMode37AAC(void) {
    __asm__ volatile(
        ".set push\n"
        ".set noat\n"
        "lui $1, 0x1F80\n"
        "sw $0, 0x84($1)\n"
        ".set pop\n"
        ::: "memory");
}

void func_80037AAC(void) {
    Matrix mtx0;
    Matrix mtx1;
    register s32 drawId asm("$19");
    register s32 i asm("$18");
    register Matrix *mtx1Ptr asm("$17");
    register char *point asm("$16");
    s32 frameValue;
    s32 drawArg;

    drawId = 2;
    func_80017A10(0);
    i = 0;
    mtx1Ptr = &mtx1;
    point = (char *)&D_801E4DF4[0].x;

    do {
        func_8001A530(&mtx0, *(s32 *)(point + 0x14));
        func_80069568((Matrix *)0x1F800028, &mtx0);
        func_8001A4C0(mtx1Ptr, *(s32 *)(point + 0x10));
        func_80069458(&mtx0, mtx1Ptr);
        func_80017794((void *)0x1F80011C, point, &mtx0);
        frameValue = D_801E4168;
        ClearScratchRenderMode37AAC();
        drawArg = 1;
        if (drawId < frameValue) {
            drawArg = drawId;
        }
        func_80028DEC((void *)0x1F800000, drawArg);

        func_8001A530(mtx1Ptr, 0x800);
        func_80069568(&mtx0, mtx1Ptr);
        func_80017794((void *)0x1F80011C, point, mtx1Ptr);
        frameValue = D_801E4168;
        ClearScratchRenderMode37AAC();
        drawArg = 1;
        if (drawId < frameValue) {
            drawArg = drawId;
        }
        func_80028DEC((void *)0x1F800000, drawArg);

        i++;
        point += sizeof(TrackWaypointRuntime);
    } while (i < 6);
}
