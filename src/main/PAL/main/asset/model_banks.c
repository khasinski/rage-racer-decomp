#include "common.h"
#include "game/car.h"
#include "game/race.h"

/* (model, owned grade) -> index of the CAR_xx asset pair, 0..31. */
s32 GetCarAssetIndex(s32 model, s32 grade) asm("func_80017848");
/* Progress level needed to buy this model's next grade. */
s32 GetCarUnlockLevel(s32 model) asm("func_8001785C");

extern u8 g_CarModelBaseIndex[] asm("D_8007C464");
extern u8 g_CarModelUnlockBase[] asm("D_8007C474");
extern u32 g_CarImageRect asm("D_8007C484");
extern u8 D_8019C86C;
extern u8 D_8009EC94;
extern void *g_CarModelAsset asm("D_8009E698");
extern void *g_VisibleCellMask asm("D_801E6828");
extern void *g_VisibleCellList asm("D_801E4BC8");
extern void *g_ModelBanks[] asm("D_801E41A8");
extern s32 g_ModelBankCount asm("D_801E4168");
extern s32 g_CourseModelCount asm("D_801E40E4");
extern void *g_TerrainCellGrid asm("D_801E5020");
extern void *g_CellVisibilityTable asm("D_801E4B98");
extern s32 D_801E4144;
extern void *g_CarImageSlots[] asm("D_801E424C");
extern void *g_CarModelSlots[] asm("D_801E8A54");
void LoadImage(void *rect, void *data) asm("func_80065B24");

s32 GetCarAssetIndex(s32 model, s32 grade) {
    return g_CarModelBaseIndex[model] + grade;
}

s32 GetCarUnlockLevel(s32 model) {
    return g_CarTable[model].modelVariant + g_CarModelUnlockBase[model];
}

void InitRenderState(s32 arg0) asm("func_80017884");
void InitRenderState(s32 arg0) {
    register s32 value asm("$2");
    register s32 ptr asm("$2");
    s32 tmp;

    tmp = g_MirrorMode;
    value = 0xA;
    *(s32 *)0x1F80006C = value;
    value = 0x80;
    *(u8 *)0x1F800072 = value;
    *(u8 *)0x1F800071 = value;
    *(u8 *)0x1F800070 = value;
    value = 0x2C;
    *(u8 *)0x1F800073 = value;
    value = 0xFF;
    *(u8 *)0x1F800076 = value;
    *(u8 *)0x1F800075 = value;
    *(u8 *)0x1F800074 = value;
    value = 0x3C;
    *(u8 *)0x1F800077 = value;
    value = 0x140;
    *(u16 *)0x1F80007C = value;
    value = 0xF0;
    *(u16 *)0x1F80007E = value;
    ptr = (s32)&D_8019C86C;
    g_VisibleCellMask = (void *)ptr;
    ptr = (s32)&D_8009EC94;
    *(s32 *)0x1F800064 = arg0;
    *(u16 *)0x1F800078 = 0;
    *(u16 *)0x1F80007A = 0;
    g_VisibleCellList = (void *)ptr;
    *(s32 *)0x1F800068 = tmp;
}

void RegisterModelBank(s32 *base, s32 index) asm("func_80017948");
void RegisterModelBank(s32 *base, s32 index) {
    s32 *ptr;
    s32 i;
    register s32 value asm("$2");
    s32 count;
    s32 pad[2];

    (void)&pad;
    ptr = base + 3;
    g_ModelBanks[index] = base;
    value = base[1];
    i = 0;
    value = (s32)base + value;
    base[1] = value;
    value = base[2];
    count = base[0];
    value = (s32)base + value;
    base[2] = value;
    if (count != 0) {
        do {
            value = *ptr;
            i++;
            value = (s32)base + value;
            *ptr = value;
            value = base[0];
            ptr++;
        } while ((u32)i < (u32)value);
    }
}

void UnrelocateModelBank(s32 *base, s32 offset) asm("func_800179B4");
void UnrelocateModelBank(s32 *base, s32 offset) {
    s32 *ptr;
    s32 i;
    register s32 value asm("$2");
    s32 count;
    s32 pad[2];

    (void)&pad;
    ptr = base + 3;
    value = base[1];
    i = 0;
    value -= offset;
    base[1] = value;
    value = base[2];
    count = base[0];
    value -= offset;
    base[2] = value;
    if (count != 0) {
        do {
            value = *ptr;
            i++;
            value -= offset;
            *ptr = value;
            value = base[0];
            ptr++;
        } while ((u32)i < (u32)value);
    }
}

void SelectModelBank(s32 index) asm("func_80017A10");
void SelectModelBank(s32 index) {
    s32 *entry;
    s32 ptr;
    s32 value;

    ptr = (s32)g_ModelBanks;
    entry = (s32 *)((index * 4) + ptr);
    value = *(volatile s32 *)entry;
    value = *(s32 *)value;
    ptr = *entry;
    *(s32 *)0x1F800054 = *(s32 *)(ptr + 4);
    ptr = *entry;
    *(s32 *)0x1F800058 = *(s32 *)(ptr + 8);
    ptr = *entry;
    g_ModelBankCount = value;
    *(s32 *)0x1F800050 = ptr + 0xC;
}

void RegisterCourseModels(s32 *base) asm("func_80017A6C");
void RegisterCourseModels(s32 *base) {
    s32 *ptr;
    s32 count;
    s32 i;
    s32 limit;
    s32 *item;
    s32 value;
    s32 pad[2];

    (void)&pad;
    ptr = base + 1;
    count = base[0];
    *(s32 *)0x1F800048 = (s32)ptr;
    g_CourseModelCount = count;
    i = 0;
    if (count > 0) {
        limit = count;
        item = base + 3;
        do {
            value = *ptr;
            i++;
            { s32 rel = value; value = (s32)base + rel; }
            *ptr = value;
            value = *item;
            ptr += 3;
            { s32 rel = value; value = (s32)base + rel; }
            *item = value;
            item += 3;
        } while (i < limit);
    }
}

void InstallTerrainCellData(s32 *base) asm("func_80017AD0");
void InstallTerrainCellData(s32 *base) {
    s32 *ptr;
    s32 count;
    s32 i;
    register s32 value asm("$2");
    s32 pad[2];

    (void)&pad;
    g_TerrainCellGrid = base;
    base = (s32 *)((s32)base + 0x800);
    g_CellVisibilityTable = base;
    base = (s32 *)((s32)base + 0x1000);
    ptr = base + 2;
    count = base[0];
    asm volatile("" : "=r"(count) : "0"(count));
    value = base[1];
    *(s32 *)0x1F80005C = (s32)ptr;
    value = (s32)base + value;
    asm volatile("" : "=r"(value) : "0"(value));
    D_801E4144 = count;
    *(s32 *)0x1F800060 = value;
    i = 0;
    if (count > 0) {
        do {
            value = *ptr;
            i++;
            value = (s32)base + value;
            *ptr = value;
            ptr++;
        } while (i < count);
    }
}

void SetCarImageSlot(void *asset, s32 index) asm("func_80017B44");
void SetCarImageSlot(void *asset, s32 index) {
    g_CarImageSlots[index] = asset;
}

void UploadCarImage(s32 index) asm("func_80017B5C");
void UploadCarImage(s32 index) {
    LoadImage(&g_CarImageRect, g_CarImageSlots[index]);
}

void SetCarModelSlot(void *asset, s32 index) asm("func_80017B94");
void SetCarModelSlot(void *asset, s32 index) {
    g_CarModelSlots[index] = asset;
}

void SelectCarModelSlot(s32 index) asm("func_80017BAC");
void SelectCarModelSlot(s32 index) {
    g_CarModelAsset = g_CarModelSlots[index];
}

void ModelBankNoOp(void) asm("func_80017BCC");
void ModelBankNoOp(void) {
}
