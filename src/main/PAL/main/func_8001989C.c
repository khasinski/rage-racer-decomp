#include "common.h"

typedef struct SceneAssetHeader {
    s32 offsets[11];
} SceneAssetHeader;

extern s32 D_8007BED8;
extern s32 D_8009E6A4;
extern s32 D_801E428C;
extern SceneAssetHeader *D_8019CAFC;
extern u8 *D_801F17A8;

s32 func_80017C78(s32 assetIndex, void *dst);
s32 func_80017C2C(void);
void func_80017BD4(void *arg0);
void func_8004553C(void *arg0);
void func_8004550C(void *arg0);
void func_80017948(void *arg0, s32 arg1);
void func_8002A6B0(void *arg0);
void func_80017A6C(void *arg0);
void func_80017AD0(void *arg0);
void func_8004121C(void *arg0);
void func_80034E04(void *arg0);
void func_80043AC8(void *arg0, s32 arg1);

void func_8001989C(void) {
    register SceneAssetHeader *header asm("$4");
    register void *dst asm("$5");
    register s32 offset asm("$2");

    switch (D_8007BED8) {
    case 1:
        dst = D_8019CAFC;
        offset = D_801E428C;
        __asm__ volatile("" : "=r"(offset) : "0"(offset));
        header = (SceneAssetHeader *)D_8009E6A4;
        if (func_80017C78(((s32)header << 3) + (offset << 1) + 0x58, dst) != 0) {
            header = D_8019CAFC;
            offset = header->offsets[0];
            D_801F17A8 = (u8 *)header + offset;
            func_80017BD4(D_801F17A8);

            header = D_8019CAFC;
            offset = header->offsets[1];
            D_801F17A8 = (u8 *)header + offset;
            func_8004553C(D_801F17A8);

            header = D_8019CAFC;
            offset = header->offsets[2];
            D_801F17A8 = (u8 *)header + offset;
            func_8004550C(D_801F17A8);

            header = D_8019CAFC;
            offset = header->offsets[3];
            D_801F17A8 = (u8 *)header + offset;
            func_80017948(D_801F17A8, 1);

            header = D_8019CAFC;
            offset = header->offsets[4];
            D_801F17A8 = (u8 *)header + offset;
            func_8002A6B0(D_801F17A8);

            header = D_8019CAFC;
            offset = header->offsets[5];
            D_801F17A8 = (u8 *)header + offset;
            func_80017A6C(D_801F17A8);

            header = D_8019CAFC;
            offset = header->offsets[6];
            D_801F17A8 = (u8 *)header + offset;
            func_80017948(D_801F17A8, 2);

            header = D_8019CAFC;
            offset = header->offsets[7];
            D_801F17A8 = (u8 *)header + offset;
            func_80017AD0(D_801F17A8);

            header = D_8019CAFC;
            offset = header->offsets[8];
            D_801F17A8 = (u8 *)header + offset;
            func_8004121C(D_801F17A8);

            header = D_8019CAFC;
            offset = header->offsets[9];
            D_801F17A8 = (u8 *)header + offset;
            func_80034E04(D_801F17A8);

            header = D_8019CAFC;
            offset = header->offsets[10];
            D_801F17A8 = (u8 *)header + offset;
            func_80043AC8(D_801F17A8, 0);

            D_8007BED8 = 2;
        }
        break;
    case 2:
        if (func_80017C2C() != 0) {
            D_8007BED8 = 0;
        }
        break;
    }
}
