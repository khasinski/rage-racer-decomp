#include "common.h"

typedef struct {
    u32 unk0;
    u32 unk4;
} UnkCopyChunk;

extern u8 D_8007BE68;
extern u8 D_801E4388;
extern s16 D_8019CB40;
extern s16 D_8019CB42;
extern s32 D_801E4DA8;
extern s32 D_8019C980;
extern s32 D_8019C984;
extern s32 D_8019C988;
extern s32 D_8019C98C;
extern s32 D_8019C990;
extern u8 D_801E4F44;
extern s32 D_801E4094;
extern u8 D_8019C914;
extern s32 D_801E6E7C;
extern u8 D_8009E874;
extern u8 D_801E42EC;
extern u8 *D_8009E67C;
extern s32 D_801E7730;
extern s32 D_801E772C;
extern s32 D_801E40A8;
extern s32 D_801E42CC;
extern s32 D_8019C704;
extern s32 D_801E8A50;
extern s32 D_801E6C70;

void func_80021224(void);
void func_80021288(void *arg0, s32 *arg1);
void func_800212F0(s32 arg0);
void func_8001B488(void);

void func_80021338(void) {
    u8 *src;
    u8 *dst;
    s32 i;
    s32 offset;
    s32 emptySlot;

    i = 0;
    dst = &D_801E4388;
    src = &D_8007BE68;
    do {
        __builtin_memcpy(dst, src, sizeof(UnkCopyChunk));
        dst += sizeof(UnkCopyChunk);
        i++;
        src += sizeof(UnkCopyChunk);
    } while (i < 13);

    D_8019CB40 = 0;
    D_8019CB42 = 0;
    D_801E4DA8 = 0;

    emptySlot = -1;
    for (offset = 4; offset < 0x2C; offset += 4) {
        *(s16 *)((u8 *)&D_8019CB40 + offset) = emptySlot;
        *(s16 *)((u8 *)&D_8019CB42 + offset) = 0;
    }

    D_8019C980 = 0;
    D_8019C984 = 3;
    D_8019C988 = 0;
    D_8019C98C = 0;
    D_8019C990 = 0;
    func_80021288(&D_801E4F44, &D_801E4094);
    func_80021288(&D_8019C914, &D_801E6E7C);

    D_8009E67C = &D_8009E874;
    func_800212F0(0);
    D_8009E67C = &D_801E42EC;
    func_800212F0(0);

    D_801E7730 = 0;
    D_801E772C = 0;
    D_801E40A8 = 9;
    D_801E42CC = 0;
    func_8001B488();
    D_8019C704 = 0xF;
    D_801E8A50 = 0xF;
    D_801E6C70 = 0;
    func_80021224();
}
