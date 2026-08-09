#include "common.h"
#include "game/asset.h"
#include "game/race.h"
#include "game/random.h"
#include "game/render.h"
#include "game/scratchpad.h"
#include "game/track.h"
#include "psyq/gte.h"

void DrawSpinningScenery(s32 timer, s32 animate) {
    s16 yawMatrix[16];
    s16 objectMatrix[16];
    s32 frame = timer;
    s32 update = animate;
    u16 *dst;
    u16 *delta;
    u16 *deltaBase;
    s16 *work = objectMatrix;
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
    AssetAddress dataAddress;
    SpinningSceneryAngleAddress startAddress;
    SpinningSceneryAngleAddress cursorAddress;
    SpinningSceneryAngleAddress endAddress;

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
        work = objectMatrix;
        base = g_SpinningSceneryAngle;
        dstOffset = loopIndex * 2;
        startAddress.pointer = base;
        startAddress.byteOffset = dstOffset + startAddress.byteOffset;
        dst = startAddress.pointer;
        offset = loopIndex * 0x10;

        do {
            if (update != 0) {
                *dst += *delta;
            }
            *dst &= 0xFFF;

            dataAddress.pointer = g_SpinningSceneryYaw;
            dataAddress.offset += offset;
            BuildRotMatrixY(yawMatrix,
                            ((SpinningSceneryOrientation *)dataAddress.pointer)->yaw);
            MulMatrix2(SCRATCH_VIEW_MATRIX_GTE, yawMatrix);
            BuildRotMatrixZ(work, *(s16 *)dst);
            MulMatrix2(yawMatrix, work);
            dataAddress.pointer = g_SpinningSceneryPos;
            dataAddress.offset += offset;
            SetGteObjectMatrix((void *)0x1F80011C, dataAddress.pointer, work);

            SCRATCH_ENV_MODE4 = 0;
            limit = 1;
            if (g_CourseModelCount >= 0x3F) {
                limit = 0x3E;
            }
            SubmitCourseModel2((void *)SCRATCHPAD_ADDR, limit);

            dst++;
            offset += 0x10;
            cursorAddress.pointer = dst;
            endAddress.pointer = base;
            endAddress.byteOffset = (end * 2) + endAddress.byteOffset;
        } while (cursorAddress.byteOffset < endAddress.byteOffset);
    }

    frameMask = frame & 0x1FF;
    if ((frameMask == 0) && (update != 0)) {
        g_SpinningSceneryRate[0] = Random15() & 0x1F;
        g_SpinningSceneryRate[1] = Random15() & 0x3F;
    }
}
