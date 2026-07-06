#include "common.h"
#include "game/car.h"

void func_80038C4C(GameCarRuntime *arg0) {
    register GameCarRuntime *obj asm("$6") = arg0;
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

    if (obj->motionActive != 0) {
        timer = obj->motionTimer - 1;
        obj->motionTimer = timer;
        if ((s32)(timer << 16) <= 0) {
            obj->motionActive = 0;
            obj->motionTimer = 0;
        }

        velX = obj->velocityX;
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
        scaledZ = obj->velocityZ;
        dampX = scaledX >> 3;
        obj->velocityX = dampX;
        __asm__ volatile("" ::: "memory");

        scaledZ = (scaledZ * 8) - scaledZ;
        if (scaledZ < 0) {
            scaledZ += 7;
        }
        __asm__ volatile("" : "=r"(scaledZ) : "0"(scaledZ));
        obj->velocityZ = scaledZ >> 3;
    }
}
