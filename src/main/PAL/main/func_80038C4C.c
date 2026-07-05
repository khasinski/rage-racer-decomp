#include "common.h"

typedef struct {
    s32 x;
    u8 pad4[4];
    s32 z;
    u8 padC[0x70];
    s16 velX;
    s16 velZ;
    s16 active;
    u16 timer;
} Unk2944C;

void func_80038C4C(Unk2944C *arg0) {
    register Unk2944C *obj asm("$6") = arg0;
    register s32 velX asm("$3");
    register s32 subX asm("$4");
    register s32 subZ asm("$5");
    s32 scaledX;
    register s32 scaledZ asm("$2");
    register s32 dampX asm("$3");
    s32 x;
    s32 z;
    u32 timer;

    __asm__ volatile("" : "=r"(obj) : "0"(obj));

    if (obj->active != 0) {
        timer = obj->timer - 1;
        obj->timer = timer;
        if ((s32)(timer << 16) <= 0) {
            obj->active = 0;
            obj->timer = 0;
        }

        velX = obj->velX;
        __asm__ volatile("lh %0, 0x7C(%1)" : "=r"(subX) : "r"(obj));
        __asm__ volatile("lh %0, 0x7E(%1)" : "=r"(subZ) : "r"(obj));
        scaledX = (velX * 8) - velX;
        x = obj->x;
        z = obj->z;
        x -= subX;
        z -= subZ;
        obj->x = x;
        obj->z = z;

        if (scaledX < 0) {
            scaledX += 7;
        }
        scaledZ = obj->velZ;
        dampX = scaledX >> 3;
        obj->velX = dampX;
        __asm__ volatile("" ::: "memory");

        scaledZ = (scaledZ * 8) - scaledZ;
        if (scaledZ < 0) {
            scaledZ += 7;
        }
        __asm__ volatile("" : "=r"(scaledZ) : "0"(scaledZ));
        obj->velZ = scaledZ >> 3;
    }
}
