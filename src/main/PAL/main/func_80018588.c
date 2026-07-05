#include "common.h"
#include "game/car.h"

typedef struct AssetTripleHeader {
    s32 firstOffset;
    s32 secondOffset;
    s32 thirdOffset;
} AssetTripleHeader;

typedef struct CarModelAsset {
    u8 pad0[0x20];
    s32 modelDataOffset;
    s32 imageDataOffset;
} CarModelAsset;

extern s32 D_8007BED8;
extern u32 D_8009E87C;
extern CarModelAsset *D_8009E698;
extern AssetTripleHeader *D_8019CAFC;
extern s32 D_8019CA64;
extern GameCarEntry *D_8019C7C8;
extern s32 D_801E40D4;
extern u8 *D_801E4090;
extern u8 *D_801E4B30;
extern u8 *D_8019C754;
extern u8 *D_801E8AB0;
extern u8 *D_801F17A8;

s32 func_80017848(s32 arg0, s32 arg1);
s32 func_80017C78(s32 assetIndex, void *dst);
void func_80017948(void *arg0, s32 arg1);
void func_80017A6C(void);
void func_80017B44(void *arg0, s32 arg1);
void func_80017B94(void *arg0, s32 arg1);
void func_80017BAC(s32 arg0);
void func_8001A3C0(void *arg0);
void func_8001D748(s32 arg0, s32 arg1);
void func_8001D900(s32 arg0, s32 arg1);
void func_8005B768(s32 arg0, void *arg1, void *arg2, void *arg3);
s32 func_8005B89C(void);
void func_8005DBD8(void);

void func_80018588(void) {
    register s32 state asm("$3") = D_8007BED8;
    register s32 state2 asm("$16");
    register u8 *carModelBase asm("$16");
    register AssetTripleHeader *header asm("$2");
    register AssetTripleHeader *headerArg asm("$4");
    register CarModelAsset *model asm("$2");
    register GameCarEntry *entry asm("$2");
    register s32 carIndex asm("$4");
    register s32 indexOffset asm("$2");
    register s32 firstOffset asm("$3");
    register s32 secondOffset asm("$4");
    register s32 assetOffset asm("$2");
    register s32 modelPtr asm("$4");

    state2 = 2;

    if (state == state2) {
        goto state_2;
    }
    if (state < 3) {
        if (state == 1) {
            goto state_1;
        }
        goto done;
    }
    if (state == 3) {
        goto state_3;
    }
    if (state == 4) {
        goto state_4;
    }
    goto done;

state_1:
        __asm__ volatile("" ::: "$3");
        func_8005B768(1, D_801F17A8, D_801E8AB0, D_8019C754);
        D_8007BED8 = state2;
        goto done;
state_2:
        if ((func_8005B89C() << 16) != 0) {
            func_8005DBD8();
            D_8007BED8 = 3;
            D_8019CAFC = (AssetTripleHeader *)D_801E8AB0;
        }
        goto done;
state_3:
            if (func_80017C78(8, D_8019CAFC) != 0) {
                func_80017948((u8 *)D_8019CAFC + 0xC, 0xE);

                header = D_8019CAFC;
                secondOffset = header->secondOffset;
                firstOffset = header->firstOffset;
                secondOffset = (s32)((u8 *)header + secondOffset);
                header = (AssetTripleHeader *)((u8 *)header + firstOffset);
                D_8019CA64 = (s32)header;
                D_801F17A8 = (u8 *)secondOffset;
                func_80017A6C();

                headerArg = D_8019CAFC;
                assetOffset = headerArg->thirdOffset;
                D_801F17A8 = (u8 *)headerArg + assetOffset;
                func_8001A3C0(D_801F17A8);

                D_8007BED8 = 4;
                D_801E4090 = D_801F17A8;
                D_801E4B30 = D_801F17A8 + 0x40000;
            }
        goto done;
state_4:
            carIndex = D_801E40D4;
            indexOffset = carIndex << 3;
            entry = (GameCarEntry *)(indexOffset + (s32)D_8019C7C8);
            assetOffset = func_80017848(carIndex, entry->modelVariant) << 1;
            carModelBase = D_801E4090;

            if (func_80017C78(assetOffset + 0xA, carModelBase) != 0) {
                func_80017B94(carModelBase, 0);
                func_80017BAC(0);

                model = D_8009E698;
                modelPtr = model->modelDataOffset;
                modelPtr = (s32)(carModelBase + modelPtr);
                model->modelDataOffset = modelPtr;
                func_80017948((void *)modelPtr, 0);

                model = D_8009E698;
                modelPtr = model->imageDataOffset;
                modelPtr = (s32)(carModelBase + modelPtr);
                model->imageDataOffset = modelPtr;
                func_80017B44((void *)modelPtr, 0);

                carIndex = D_801E40D4;
                if (carIndex < 10) {
                    indexOffset = carIndex << 3;
                    entry = (GameCarEntry *)(indexOffset + (s32)D_8019C7C8);
                    func_8001D748(entry->shapeIndex, D_8009E698->imageDataOffset);
                    indexOffset = D_801E40D4 << 3;
                    entry = (GameCarEntry *)(indexOffset + (s32)D_8019C7C8);
                    func_8001D900(entry->textureIndex, D_8009E698->imageDataOffset);
                }

                D_8009E87C = 0;
                D_8007BED8 = 0;
            }
done:
    return;
}
