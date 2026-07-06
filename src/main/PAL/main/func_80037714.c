#include "common.h"
#include "game/waypoint.h"

extern s16 D_8009E83C;
extern s32 D_8019C700;
extern TrackWaypointSeed D_8007DFD4[];
extern TrackWaypointRuntime D_801E4DF4[];

void func_80037714(void) {
    register TrackWaypointRuntime *waypoint asm("$7");
    register s32 i asm("$5");
    register TrackWaypointSeed *seed asm("$6");
    register s32 code asm("$9");
    register s32 field18 asm("$8");
    register char *tail asm("$4");
    register s32 track asm("$4");
    register s32 temp asm("$2");
    register s32 magic asm("$3");

    magic = 0x66666667;
    temp = D_8009E83C;
    track = temp - 1;
    asm volatile("mult %0,%1" : : "r"(track), "r"(magic));
    temp = track >> 31;
    asm volatile("mfhi %0" : "=r"(magic));
    magic >>= 2;
    magic -= temp;
    temp = (magic << 2) + magic;
    temp <<= 1;
    track -= temp;
    if (track < 0) {
        track = 0;
    } else if (track >= 9) {
        track = 9;
    }

    waypoint = D_801E4DF4;
    temp = (track << 1) + track;
    temp <<= 2;
    magic = (s32)D_8007DFD4;
    seed = (TrackWaypointSeed *)(temp + magic);
    i = 0;
    code = 0x1766;
    field18 = 0x174;
    tail = (char *)waypoint + 0x1C;

    do {
        waypoint->active = 0;
        *(s32 *)(tail - 0x18) = seed->x + (seed->stepX * i);
        *(s32 *)(tail - 0x10) = seed->y + (seed->stepY * i);
        *(s32 *)(tail - 0x14) = code;
        *(s32 *)(tail - 0x4) = field18;
        *(s32 *)(tail - 0x8) = 0;
        *(s32 *)tail = 0;

        i++;
        waypoint++;
        tail += sizeof(*waypoint);
    } while (i < 6);

    D_8019C700 = 0;
}
