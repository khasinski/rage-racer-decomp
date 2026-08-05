#include "common.h"
#include "psyq/gte.h"
#include "game/render.h"
#include "game/state.h"
#include "game/cd.h"
extern GameRenderView *g_CarModelAsset;
extern Matrix g_SceneLightMatrix;
extern s32 g_ModelBankCount;
extern s32 g_ScratchRenderMode asm("D_1F800084");
s32 GetTrackZoneBlend(s32 arg0);
void ApplyZoneLighting(s32 arg0, Matrix *arg1);
void func_80017794(void *a0, void *a1, void *a2);
void RestoreColorMatrix(void);
void DrawPlayerCarModel(GameRenderObject *obj);
extern u8 *g_CamRow;
extern s32 g_CourseIndex;
extern u8 g_CarModelByCourse[][11];
extern s16 D_8007D380[][2];
void *ApplyMatrixLV(void *mtx, void *vec, void *out);
extern s32 g_FmvState;
extern s32 g_StreamReturnScene;
s32 CloseLoadedAudioSlots(void);
void CdSync(s32 arg0, s32 arg1);
s32 CdControl(s32 com, void *param, s32 result);
void BeginFmv(s32 arg0);
extern u32 g_AssetBase;
void StartFmvPlayback(u32 arg0);
void DecodeFmvFrame(void);
void EndFmv(void);
void UpdateFmv(void);

void BeginFmv(s32 arg0);
void BeginFmv(s32 arg0) {
    CloseLoadedAudioSlots();
    ResetCdAudioState();
    g_FmvState = 0;
    g_StreamReturnScene = arg0;
    g_SceneId = 5;
    CdSync(0, 0);
    CdControl(9, 0, 0);
}

extern u32 g_AssetBase;

void StartFmvPlayback(u32 arg0);
void DecodeFmvFrame(void);
void EndFmv(void);

void UpdateFmv(void);
void UpdateFmv(void) {
    switch (g_FmvState) {
    case 0:
        StartFmvPlayback(g_AssetBase);
        /* fall through */
    case 1:
        DecodeFmvFrame();
        break;
    case 2:
        EndFmv();
        break;
    }
}
