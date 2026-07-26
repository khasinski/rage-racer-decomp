#include "common.h"
#include "game/car.h"
#include "game/render.h"
#include "psyq/gpu.h"

extern s16 D_8019C7D4;
extern s16 D_8019C7D6;
extern s16 D_8019C7D8;
extern s16 D_8019C7DA;
extern s16 D_8019C7DC;
extern s16 D_8019C7DE;
extern s16 D_8019C7E0;
extern s16 D_8019C7E2;
extern GameSpriteDesc D_8007DAE0;
extern u8 D_801C051C[];
extern u8 D_801E3D04[];
extern u8 D_801E3CEC[];
extern u8 D_801E3CF8[];

void func_80032FF0(SPRT *prim, GameSpriteDesc *src);
void func_80064EB8(u8 *arg0, s32 arg1);
void func_800666F4(u8 *arg0, s32 arg1, s32 arg2, u16 arg3, void *arg4);

/*
 * Rebuilds the tachometer needle quad from the car spec block. The spec is
 * read through a plain `u8 *` on purpose: as GameCarSpec member reads, gcc
 * 2.6.3 stops treating the loads as aliasing the D_8019C7Dx globals this
 * function stores to and reorders the two against each other.
 *   +0x14C..0x14F  spec->needleQuad
 *   +0x138/+0x13A  spec->tachoNeedleX / Y
 *   +0x13C/+0x13E  spec->tachoFaceDX / DY
 */
void func_80032BD0(void) {
    u8 *data = (u8 *)g_CarSpec;
    u8 *prim0 = D_801C051C;
    u8 *prim1 = D_801E3D04;
    GameSpriteDesc *src = &D_8007DAE0;

    D_8019C7D4 = -data[0x14F];
    D_8019C7D6 = data[0x14E];
    D_8019C7D8 = -data[0x14D];
    D_8019C7DA = -data[0x14C];
    D_8019C7DC = data[0x14F];
    D_8019C7DE = data[0x14E];
    D_8019C7E0 = data[0x14D];
    D_8019C7E2 = -data[0x14C];

    src->x = *(u16 *)(data + 0x13C) + *(u16 *)(data + 0x138);
    src->y = *(u16 *)(data + 0x13E) + *(u16 *)(data + 0x13A);

    func_80032FF0((SPRT *)prim0, src);
    func_80032FF0((SPRT *)prim1, src);
    func_80064EB8(prim0, 0);
    func_80064EB8(prim1, 0);
    func_800666F4(prim0 - 0x18, 0, 1, 9, 0);
    func_800666F4(prim0 - 0x0C, 0, 1, 0xA, 0);
    func_800666F4(D_801E3CEC, 0, 1, 9, 0);
    func_800666F4(D_801E3CF8, 0, 1, 0xA, 0);
}
