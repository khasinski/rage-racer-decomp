/* MASPSX_FLAGS: --store-return-delay */
#include "common.h"

typedef struct {
    s32 field_00;
    s32 pad04;
    s32 field_08;
    s16 field_0C;
    s16 field_0E;
    s32 vec0[4];
    s32 field_20;
    s32 field_24;
    s32 field_28;
    s32 pad2C;
    s32 pad30;
} Unk3F0F8State;

extern s32 D_801E428C;
extern Unk3F0F8State D_801E4FB8;
extern s32 D_801E4FEC;
extern s32 D_801E4FF4;
extern s16 D_801E4FF8;
extern s16 D_801E4FFA;
extern s32 D_801E4FFC;
extern s32 D_801E5000;
extern s32 D_801E5004;
extern s32 D_801E5008;
extern s32 D_801E500C;
extern s32 D_801E5010;
extern s32 D_801E5014;
extern s32 D_8007E3A0[];
extern s32 D_8007E360[];
extern s16 D_8007E3C0[];
extern s16 D_8007E3E0[];

void func_8003F0F8(void) {
    register Unk3F0F8State *state asm("$7");
    register s32 *src asm("$5");
    register s32 *dst asm("$4");
    register s32 index asm("$4");
    register s32 value asm("$2");
    register s32 v1 asm("$3");
    register s32 a4 asm("$4");
    register s32 a5 asm("$5");
    register s32 a6 asm("$6");

    state = &D_801E4FB8;
    if ((D_801E428C & 3) == 2) {
        D_801E4FFA = 2;
        asm("" ::: "memory");
        src = D_8007E3A0;
        dst = &D_801E4FFC;
        asm("" : "=r"(src), "=r"(dst) : "0"(src), "1"(dst));
        value = src[0];
        v1 = src[1];
        dst[0] = value;
        dst[1] = v1;
        value = src[2];
        v1 = src[3];
        dst[2] = value;
        dst[3] = v1;

        asm("" ::: "memory");
        index = D_801E4FFA;
        v1 = index << 3;
        D_801E500C = *(s16 *)((s32)D_8007E3C0 + v1);
        D_801E5010 = *(s16 *)((s32)D_8007E3C0 + v1 + 2);
        value = *(s16 *)((s32)D_8007E3C0 + v1 + 4);
        index <<= 1;
        D_801E4FF8 = 0;
        D_801E4FF4 = 0;
        D_801E5014 = value;
        v1 = *(s16 *)((s32)D_8007E3E0 + index);
        state->field_0E = 1;
        D_801E4FEC = v1;
        goto updateState;
    }

    state->field_0E = 0;
updateState:
    asm("" ::: "memory");
    asm("" : "=r"(state) : "0"(state));
    asm volatile("lh %0,0x0e(%1)" : "=r"(value) : "r"(state));
    value <<= 5;
    v1 = *(s32 *)((s32)D_8007E360 + value);
    a4 = *(s32 *)((s32)D_8007E360 + value + 4);
    a5 = *(s32 *)((s32)D_8007E360 + value + 8);
    a6 = *(s32 *)((s32)D_8007E360 + value + 12);
    *(s32 *)((s32)state + 0x10) = v1;
    *(s32 *)((s32)state + 0x14) = a4;
    *(s32 *)((s32)state + 0x18) = a5;
    *(s32 *)((s32)state + 0x1C) = a6;
    asm volatile("lh %0,0x0e(%1)" : "=r"(value) : "r"(state));
    value <<= 3;
    v1 = *(s16 *)((s32)D_8007E3C0 + value);
    asm volatile("lh %0,0x0e(%1)" : "=r"(value) : "r"(state));
    value <<= 3;
    *(s32 *)((s32)state + 0x20) = v1;
    v1 = *(s16 *)((s32)D_8007E3C0 + value + 2);
    asm volatile("lh %0,0x0e(%1)" : "=r"(value) : "r"(state));
    value <<= 3;
    *(s32 *)((s32)state + 0x24) = v1;
    v1 = *(s16 *)((s32)D_8007E3C0 + value + 4);
    value = state->field_0E;
    *(s16 *)((s32)state + 0x0C) = 0;
    *(s32 *)((s32)state + 0x08) = 0;
    value <<= 1;
    *(s32 *)((s32)state + 0x28) = v1;
    value = *(s16 *)((s32)D_8007E3E0 + value);
    *(s32 *)((s32)state + 0x00) = value;
}
