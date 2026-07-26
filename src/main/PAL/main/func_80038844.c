#include "common.h"
#include "game/car.h"

extern s32 D_801E42E4;

extern s32 D_801E7740;

extern s32 D_801E408C;

extern s16 D_8019CABC;

extern GameCarRuntime D_801F1854[];

extern s32 D_8007E074[];

extern s32 D_8007E0A4[];

void func_80038AB8(GameCarRuntime *arg0);

void func_800383A8(GameCarRuntime *arg0, s32 arg1, s32 *arg2);

void func_800385FC(GameCarRuntime *arg0, s32 arg1, s32 *arg2);

void func_80039644(void);

void func_80017A10(s32 arg0);

void func_8001DFC0(GameCarRuntime *arg0);

extern s16 D_801E6E74;

extern u8 *D_801E42D8;

/*
 * Updates the car's skid/tilt counter (field_8C / field_8E), clamping it
 * against the spec block's redline value (D_801E42D8 + 0x106). Register-pinned,
 * goto-structured; the raw drive-block reads (+0xA2 / +0x34) preserve the match.
 */

/*
 * Applies and decays the collision shake: while motionActive/motionTimer are
 * set, subtracts field_7C / field_7E from the car position and damps the
 * velocity by 7/8 (*8-1 >> 3) each frame. field_7C / field_7E are read via
 * inline __asm__ lh loads and the registers are pinned, so the struct offsets
 * must not change.
 */

void func_80038844(void) {
    register GameCarRuntime *entryBase asm("s2");
    register s32 *table asm("s3");
    register s32 i asm("s0");
    register s16 *flagPtr asm("s1");
    register s32 one asm("s5");
    register s32 *cursor asm("s4");
    register s32 state asm("v1");
    register u16 track asm("v0");

    entryBase = D_801F1854;
    state = D_801E42E4;
    D_801E7740 = 3;

    if (state == 0xB) {
        table = D_8007E074;
        i = 0;
        one = 1;
        flagPtr = &entryBase->activeFlag;
        D_801E408C = D_8019CABC;
        cursor = table;
        do {
            track = *(u16 *)&D_801E408C;
            *(s16 *)flagPtr = 0;
            *(s16 *)(flagPtr + 6) = track;
            if (*cursor >= 0) {
                func_80038AB8(entryBase);
                *(s16 *)flagPtr = one;
                func_800383A8(entryBase, i, table);
                func_800385FC(entryBase, i, table);
            } else {
                *(s16 *)flagPtr = 0;
            }
            cursor++;
            i++;
            flagPtr = (s16 *)((u8 *)flagPtr + 0x19C);
            entryBase++;
        } while (i < 0xB);
    } else {
        table = D_8007E0A4;
        i = 0;
        one = 1;
        flagPtr = &entryBase->activeFlag;
        D_801E408C = D_8019CABC;
        cursor = table;
        do {
            track = *(u16 *)&D_801E408C;
            *(s16 *)flagPtr = 0;
            *(s16 *)(flagPtr + 6) = track;
            if (*cursor >= 0) {
                func_80038AB8(entryBase);
                *(s16 *)flagPtr = one;
                func_800383A8(entryBase, i, table);
                func_800385FC(entryBase, i, table);
            } else {
                *(s16 *)flagPtr = 0;
            }
            cursor++;
            i++;
            flagPtr = (s16 *)((u8 *)flagPtr + 0x19C);
            entryBase++;
        } while (i < 0xB);
    }

    func_80039644();
}

void func_800389F0(void) {
    GameCarRuntime *base;
    s32 i;
    u8 *entry;
    s32 one;
    s32 minus_one;

    base = D_801F1854;
    func_80017A10(1);

    i = 0;
    minus_one = -1;
    one = 1;
    entry = (u8 *)&base->field_BC;
    do {
        if (*(s16 *)(entry - 0x10) != (i++, minus_one)) {
            if (*(s32 *)entry == one) {
                func_8001DFC0(base);
            }
        }
        entry += 0x19C;
        base++;
    } while (i < 11);
}

void func_80038A88(void) {
    func_80017A10(1);
    func_8001DFC0(D_801F1854);
}

void func_80038AB8(GameCarRuntime *arg0) {
    arg0->field_8A = 0;
    arg0->motionMode = 0;
    arg0->motionModeTimer = 0;
    arg0->motionValue = 0;
    arg0->motionActive = 0;
    arg0->motionTimer = 0;
    arg0->velocityX = 0;
    arg0->velocityZ = 0;
    arg0->field_8C = 0;
    arg0->field_8E = 0;
    arg0->field_90 = 0;
    arg0->field_92 = 0;
    arg0->field_94 = 0;
    arg0->field_96 = 0;
    arg0->field_98 = 0;
    arg0->field_9A = 0;
    arg0->field_9C = 0;
    arg0->field_9E = 0;
}

void func_80038B04(GameCarRuntime *arg0) {
    register GameCarRuntime *obj asm("$5");
    register u8 *ptr asm("$4");
    register s32 value asm("$2");
    register s32 limit asm("$3");

    obj = arg0;
    __asm__ volatile("" : "=r"(obj) : "0"(obj));

    ptr = (u8 *)&obj->field_BC;
    if (D_801E6E74 >= 2) {
        goto main_body;
    }

    value = 8;
    goto store_value;

main_body:
    if (obj->field_98 != 0) {
        goto dampen;
    }

    if (obj->field_134 < *(s16 *)(D_801E42D8 + 0x106)) {
        goto side_check;
    }
    if (obj->field_15C < 0x81) {
        goto side_check;
    }
    if (obj->field_F0 != 0) {
        goto side_check;
    }

    ptr = (u8 *)(s32)(u16)obj->field_8C;
    value = obj->field_132;
    ptr -= 4;
    obj->field_8C = (s32)ptr;
    ptr = (u8 *)((s32)ptr << 16 >> 16);
    limit = 9 - value;
    value = (limit << 2) + limit;
    value = -value;
    if ((s32)ptr < value) {
        obj->field_8C = value;
    }
    return;

side_check:
    if (*(s16 *)(ptr + 0xA2) >= 0x81 || *(s16 *)(ptr + 0x34) > 0) {
        if (obj->field_A4 >= 0x51) {
            value = (u16)obj->field_8C + 2;
            obj->field_8C = value;
            value = (s16)value;
            if (value >= 9) {
                obj->field_8C = 8;
            }
            return;
        }
    }

dampen:
    limit = obj->field_8C;
    value = limit * 3;
    if (value < 0) {
        value += 3;
    }
    value >>= 2;

store_value:
    obj->field_8C = value;
}

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
