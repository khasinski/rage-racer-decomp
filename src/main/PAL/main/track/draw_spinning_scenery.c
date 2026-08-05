#include "common.h"
#include "game/race.h"
#include "psyq/gte.h"
#include "game/render.h"

extern s32 g_CourseModelCount;
extern u16 g_SpinningSceneryRate[] asm("D_8007E33C");
extern u16 g_SpinningSceneryAngle[] asm("D_8007E334");
extern u8 g_SpinningSceneryYaw[] asm("D_8007E300");
/* One 16-byte { x, y, z, yaw } record per prop -- g_SpinningSceneryYaw is the
 * split symbol for that fourth word. g_SpinningSceneryAngle is the live Z spin
 * of each prop, advanced by g_SpinningSceneryRate[active], and the two rates
 * are re-rolled to rand & 0x1F / rand & 0x3F every 512 frames. */
extern u8 g_SpinningSceneryPos[] asm("D_8007E2F4");

void func_80017794(void *arg0, void *arg1, void *arg2);
s32 Random15(void);

void DrawSpinningScenery(s32 arg0, s32 arg1);

void DrawSpinningScenery(s32 arg0, s32 arg1) {
    s16 sp10[16];
    s16 sp30[16];
    s32 frame = arg0;
    s32 update = arg1;
    u16 *dst;
    u16 *delta;
    u16 *deltaBase;
    s16 *work = sp30;
    register u16 *base asm("$21");
    s32 offset;
    s32 end;
    s32 start;
    s32 loopIndex;
    s32 dstOffset;
    s32 limit;
    s32 active;
    s32 activeValue;
    s32 frameMask;

    activeValue = g_CourseIndex;
    active = activeValue & 3;
    active = active != 0;
    if (active) {
        start = 1;
        end = 4;
    } else {
        start = 0;
        end = 1;
    }

    loopIndex = start;
    asm("" : "=r"(loopIndex) : "0"(loopIndex));
    if (loopIndex < end) {
        deltaBase = g_SpinningSceneryRate;
        delta = &deltaBase[active];
        work = sp30;
        base = g_SpinningSceneryAngle;
        dstOffset = loopIndex * 2;
        dst = (u16 *)(dstOffset + (s32)base);
        offset = loopIndex * 0x10;

        do {
            if (update != 0) {
                *dst += *delta;
            }
            *dst &= 0xFFF;

            BuildRotMatrixY(sp10, *(s32 *)(g_SpinningSceneryYaw + offset));
            MulMatrix2((void *)0x1F800028, sp10);
            BuildRotMatrixZ(work, *(s16 *)dst);
            MulMatrix2(sp10, work);
            func_80017794((void *)0x1F80011C, g_SpinningSceneryPos + offset, work);

            *(s32 *)0x1F800084 = 0;
            limit = 1;
            if (g_CourseModelCount >= 0x3F) {
                limit = 0x3E;
            }
            SubmitCourseModel2((void *)0x1F800000, limit);

            dst++;
            offset += 0x10;
        } while ((s32)dst < ((end * 2) + (s32)base));
    }

    frameMask = frame & 0x1FF;
    if ((frameMask == 0) && (update != 0)) {
        g_SpinningSceneryRate[0] = Random15() & 0x1F;
        g_SpinningSceneryRate[1] = Random15() & 0x3F;
    }
}
