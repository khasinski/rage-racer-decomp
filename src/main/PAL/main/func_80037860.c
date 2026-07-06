#include "common.h"
#include "game/audio.h"
#include "game/waypoint.h"

extern s32 D_8019C700;
extern TrackWaypointRuntime D_801E4DF4[];
extern s32 D_801E43F8;
extern s32 D_8009E798[];

s32 func_80037808(TrackWaypointRuntime *waypoint);

void func_80037860(void) {
    register TrackWaypointRuntime *waypoint asm("$17");
    register s32 i asm("$18");
    register s32 activeState asm("$19");
    register char *tail asm("$16");

    if (D_8019C700 != 0) {
        D_8019C700--;
    }

    waypoint = D_801E4DF4;
    i = 0;
    activeState = 1;
    tail = (char *)waypoint + 0x34;
    do {
        if (waypoint->active == 0) {
            if (func_80037808(waypoint) != 0) {
                register volatile s32 *src asm("$5");
                register s32 src0 asm("$2");
                register s32 src1 asm("$3");
                register s32 src2 asm("$4");

                D_801E43F8++;
                GamePlaySoundCue(0xA);

                waypoint->active = activeState;
                asm volatile("" ::: "memory");
                src = D_8009E798;
                asm volatile("" : "=r"(src) : "0"(src));
                src0 = src[0];
                src1 = src[1];
                src2 = src[2];
                *(s32 *)(tail - 0x10) = src0;
                *(s32 *)(tail - 0xC) = src1;
                *(s32 *)(tail - 0x8) = src2;
                src0 = src[3];
                *(s32 *)(tail - 0x4) = src0;
                asm volatile("" ::: "memory");

                *(s32 *)(tail - 0x10) *= 2;
                *(s32 *)(tail - 0x8) *= 2;
                *(s32 *)tail =
                    ((*(s32 *)(tail - 0x10) * *(s32 *)(tail - 0x10)) + (*(s32 *)(tail - 0x8) * *(s32 *)(tail - 0x8))) /
                    0x2000;
            }
        } else if (waypoint->active == activeState) {
            *(s32 *)(tail - 0x30) += *(s32 *)(tail - 0x10) / 0x100;
            *(s32 *)(tail - 0x28) += *(s32 *)(tail - 0x8) / 0x100;
            *(s32 *)(tail - 0x10) = (*(s32 *)(tail - 0x10) * 15) / 16;
            *(s32 *)(tail - 0x8) = (*(s32 *)(tail - 0x8) * 15) / 16;
            *(s32 *)(tail - 0x1C) += *(s32 *)tail / 0x100;
            *(s32 *)tail = (*(s32 *)tail * 15) / 16;

            if (*(s32 *)(tail - 0x20) < 0x400) {
                *(s32 *)(tail - 0x20) += 0x80;
            } else {
                *(s32 *)(tail - 0x20) = 0x400;
            }

            if ((*(s32 *)(tail - 0x10) == 0) && (*(s32 *)(tail - 0x8) == 0) && (*(s32 *)tail == 0)) {
                waypoint->active = 2;
            }
        }

        i++;
        tail += sizeof(*waypoint);
        waypoint++;
    } while (i < 6);
}
