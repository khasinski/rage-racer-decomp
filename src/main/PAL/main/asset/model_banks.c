#include "common.h"
#include "game/asset.h"
#include "game/asset_internal.h"
#include "game/car.h"
#include "game/race.h"
#include "game/render.h"
#include "game/render_internal.h"
#include "game/scratchpad.h"
#include "game/track.h"
#include "psyq/gpu.h"

/* Kept local: this unit only stores an address into them, while track/ and
 * render/ read them as u32[] rows, Vec4[] entries and plain s32, four
 * incompatible element types across seven files. */

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
    g_VisibleCellMask = g_MainVisibleCellMask;
    SCRATCH_OT_SHIFT = otShift;
    SCRATCH_CLIP_X0 = 0;
    SCRATCH_CLIP_Y0 = 0;
    g_VisibleCellList = g_MainVisibleCellList;
    SCRATCH_MIRROR = g_MirrorMode;
}

void RegisterModelBank(ModelBankHeader *base, s32 index) {
    AssetAddress *ptr;
    s32 i;

    ptr = base->models;
    g_ModelBanks[index] = base;
    base->table.pointer = (u8 *)base + base->table.offset;
    base->normals.pointer = (u8 *)base + base->normals.offset;
    for (i = 0; (u32)i < (u32)base->modelCount; i++) {
        ptr->pointer = (u8 *)base + ptr->offset;
        ptr++;
    }
}

void UnrelocateModelBank(ModelBankHeader *base, s32 offset) {
    AssetAddress *ptr;
    s32 i;

    ptr = base->models;
    base->table.offset -= offset;
    base->normals.offset -= offset;
    for (i = 0; (u32)i < (u32)base->modelCount; i++) {
        ptr->offset -= offset;
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
    SCRATCH_MODEL_TABLE1 = (s32)((ModelBankHeader *)bank)->table.pointer;
    bank = *entry;
    SCRATCH_MODEL_NORMALS = (s32)((ModelBankHeader *)bank)->normals.pointer;
    bank = *entry;
    g_ModelBankCount = count;
    SCRATCH_MODEL_MODELS = (s32)((ModelBankHeader *)bank)->models;
}

void RegisterCourseModels(CourseModelAssetHeader *base) {
    CourseModelAssetEntry *entry;
    s32 count;
    s32 i;
    s32 limit;
    /* The 8-byte frame retail has. Its three sibling loops here get it from
     * being pre-test loops (gcc 2.6.3 spills a dead ST_REGS pseudo after
     * duplicate_loop_exit_test); this one cannot, because every pre-test
     * spelling of a two-pointer loop costs 29 instructions of induction
     * variables. So the frame is asked for directly. */
    s32 pad[2];

    (void)&pad;
    entry = base->models;
    count = base->modelCount;
    SCRATCH_COURSE_BANK = (s32)entry;
    g_CourseModelCount = count;
    i = 0;
    if (count > 0) {
        limit = count;
        do {
            entry->geometry.pointer = (u8 *)base + entry->geometry.offset;
            entry->model.pointer = (u8 *)base + entry->model.offset;
            entry++;
            i++;
        } while (i < limit);
    }
}

void InstallTerrainCellData(u8 *base) {
    TerrainCellAssetHeader *header;
    AssetAddress *ptr;
    s32 count;
    s32 i;

    g_TerrainCellGrid = (u16 *)base;
    base += TERRAIN_CELL_GRID_SIZE;
    g_CellVisibilityTable = base;
    base += CELL_VISIBILITY_TABLE_SIZE;
    header = (TerrainCellAssetHeader *)base;
    ptr = header->cells;
    count = header->cellCount;
    SCRATCH_CELL_TABLE = (s32)ptr;
    g_TerrainCellCount = count;
    SCRATCH_CELL_FACES = (s32)(base + header->faces.offset);
    for (i = 0; i < count; i++) {
        ptr->pointer = base + ptr->offset;
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
