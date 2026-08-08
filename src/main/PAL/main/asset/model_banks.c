#include "common.h"
#include "game/asset.h"
#include "game/car.h"
#include "game/race.h"
#include "game/render.h"
#include "game/scratchpad.h"
#include "game/track.h"
#include "psyq/gpu.h"

extern u8 g_MainVisibleCellMask;
extern u8 g_MainVisibleCellList;
extern void *g_CarModelAsset;
/* Kept local: this unit only stores an address into them, while track/ and
 * render/ read them as u32[] rows, Vec4[] entries and plain s32, four
 * incompatible element types across seven files. */
extern void *g_VisibleCellMask;
extern void *g_VisibleCellList;

s32 GetCarAssetIndex(s32 model, s32 grade) {
    return g_CarModelBaseIndex[model] + grade;
}

s32 GetCarUnlockLevel(s32 model) {
    return g_CarTable[model].modelVariant + g_CarModelUnlockBase[model];
}

void InitRenderState(s32 otShift) {
    SCRATCH_FACE_OT_SHIFT = 0xA;
    SCRATCH_FT4_B = 0x80;
    SCRATCH_FT4_G = 0x80;
    SCRATCH_FT4_R = 0x80;
    SCRATCH_FT4_CODE = POLY_FT4_CODE;
    SCRATCH_GT4_B = 0xFF;
    SCRATCH_GT4_G = 0xFF;
    SCRATCH_GT4_R = 0xFF;
    SCRATCH_GT4_CODE = POLY_GT4_CODE;
    SCRATCH_CLIP_X1 = SCREEN_WIDTH;
    SCRATCH_CLIP_Y1 = SCREEN_HEIGHT;
    g_VisibleCellMask = &g_MainVisibleCellMask;
    SCRATCH_OT_SHIFT = otShift;
    SCRATCH_CLIP_X0 = 0;
    SCRATCH_CLIP_Y0 = 0;
    g_VisibleCellList = &g_MainVisibleCellList;
    SCRATCH_MIRROR = g_MirrorMode;
}

void RegisterModelBank(s32 *base, s32 index) {
    s32 *ptr;
    s32 i;

    ptr = base + 3;
    g_ModelBanks[index] = base;
    base[1] = (s32)((u8 *)base + base[1]);
    base[2] = (s32)((u8 *)base + base[2]);
    for (i = 0; (u32)i < (u32)base[0]; i++) {
        *ptr = (s32)((u8 *)base + *ptr);
        ptr++;
    }
}

void UnrelocateModelBank(s32 *base, s32 offset) {
    s32 *ptr;
    s32 i;

    ptr = base + 3;
    base[1] -= offset;
    base[2] -= offset;
    for (i = 0; (u32)i < (u32)base[0]; i++) {
        *ptr -= offset;
        ptr++;
    }
}

/*
 * Point the scratchpad model-bank cursor at one registered bank. The bank
 * pointer is re-read from the table before each store because the stores go
 * to the scratchpad, which cse has no reason to believe does not alias it.
 * The two-step address (base into a local, then index it) is what gives
 * retail's base-first addu; `&g_ModelBanks[index]` loses it.
 */
void SelectModelBank(s32 index) {
    s32 *entry;
    s32 bank;
    s32 count;

    bank = (s32)g_ModelBanks;
    entry = (s32 *)((index * 4) + bank);
    count = *entry;
    count = *(s32 *)count;
    bank = *entry;
    SCRATCH_MODEL_TABLE1 = *(s32 *)(bank + 4);
    bank = *entry;
    SCRATCH_MODEL_NORMALS = *(s32 *)(bank + 8);
    bank = *entry;
    g_ModelBankCount = count;
    SCRATCH_MODEL_MODELS = bank + 0xC;
}

void RegisterCourseModels(s32 *base) {
    s32 *ptr;
    s32 count;
    s32 i;
    s32 limit;
    s32 *item;
    /* The 8-byte frame retail has. Its three sibling loops here get it from
     * being pre-test loops (gcc 2.6.3 spills a dead ST_REGS pseudo after
     * duplicate_loop_exit_test); this one cannot, because every pre-test
     * spelling of a two-pointer loop costs 29 instructions of induction
     * variables. So the frame is asked for directly. */
    s32 pad[2];

    (void)&pad;
    ptr = base + 1;
    count = base[0];
    SCRATCH_COURSE_BANK = (s32)ptr;
    g_CourseModelCount = count;
    i = 0;
    if (count > 0) {
        limit = count;
        item = base + 3;
        do {
            *ptr = (s32)((u8 *)base + *ptr);
            *item = (s32)((u8 *)base + *item);
            ptr += 3;
            item += 3;
            i++;
        } while (i < limit);
    }
}

void InstallTerrainCellData(s32 *base) {
    s32 *ptr;
    s32 count;
    s32 i;

    g_TerrainCellGrid = (u16 *)base;
    base = (s32 *)((u8 *)base + TERRAIN_CELL_GRID_SIZE);
    g_CellVisibilityTable = (u8 *)base;
    base = (s32 *)((u8 *)base + CELL_VISIBILITY_TABLE_SIZE);
    ptr = base + 2;
    count = base[0];
    SCRATCH_CELL_TABLE = (s32)ptr;
    g_TerrainCellCount = count;
    SCRATCH_CELL_FACES = (s32)((u8 *)base + base[1]);
    for (i = 0; i < count; i++) {
        *ptr = (s32)((u8 *)base + *ptr);
        ptr++;
    }
}

void SetCarImageSlot(void *asset, s32 index) {
    g_CarImageSlots[index] = asset;
}

void UploadCarImage(s32 index) {
    LoadImage(&g_CarImageRect, g_CarImageSlots[index]);
}

void SetCarModelSlot(void *asset, s32 index) {
    g_CarModelSlots[index] = asset;
}

void SelectCarModelSlot(s32 index) {
    g_CarModelAsset = g_CarModelSlots[index];
}

void ModelBankNoOp(void) {
}
