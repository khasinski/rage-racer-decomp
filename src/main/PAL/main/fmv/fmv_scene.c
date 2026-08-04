#include "common.h"
#include "psyq/gte.h"
#include "game/render.h"
#include "game/state.h"
#include "game/cd.h"
extern GameRenderView *g_CarModelAsset asm("D_8009E698");
extern Matrix g_SceneLightMatrix asm("D_8009E6AC");
extern s32 g_ModelBankCount asm("D_801E4168");
extern s32 g_ScratchRenderMode asm("D_1F800084");
s32 GetTrackZoneBlend(s32 arg0) asm("func_800350B4");
void ApplyZoneLighting(s32 arg0, Matrix *arg1) asm("func_8001C248");
void func_80069888(Matrix *arg0);
void func_80017794(void *a0, void *a1, void *a2);
void RestoreColorMatrix(void) asm("func_8001C794");
void DrawPlayerCarModel(GameRenderObject *obj) asm("func_8001DAB0");
extern u8 *g_CamRow2 asm("D_8019C9A8");
extern s32 g_CourseIdx asm("D_801E428C");
extern u8 g_CarModelByCourse[][11] asm("D_8007D3AC");
extern s16 D_8007D380[][2];
void *ApplyMatrixLV(void *mtx, void *vec, void *out) asm("func_80068F80");
extern s32 g_FmvState asm("D_8009F094");
extern s32 g_StreamReturnScene asm("D_8019C760");
s32 CloseLoadedAudioSlots(void) asm("func_8005B9CC");
void CdSync(s32 arg0, s32 arg1) asm("func_8006A534");
s32 CdControl(s32 com, void *param, s32 result) asm("func_8006A5A4");
void BeginFmv(s32 arg0) asm("func_8001E6B4");
extern u32 g_AssetBase asm("D_8019C904");
void StartFmvPlayback(u32 arg0) asm("func_8001E79C");
void DecodeFmvFrame(void) asm("func_8001E8A4");
void EndFmv(void) asm("func_8001EA34");
void UpdateFmv(void) asm("func_8001E71C");

void BeginFmv(s32 arg0) asm("func_8001E6B4");
void BeginFmv(s32 arg0) {
    CloseLoadedAudioSlots();
    ResetCdAudioState();
    g_FmvState = 0;
    g_StreamReturnScene = arg0;
    g_SceneId = 5;
    CdSync(0, 0);
    CdControl(9, 0, 0);
}

extern u32 g_AssetBase asm("D_8019C904");

void StartFmvPlayback(u32 arg0) asm("func_8001E79C");
void DecodeFmvFrame(void) asm("func_8001E8A4");
void EndFmv(void) asm("func_8001EA34");

void UpdateFmv(void) asm("func_8001E71C");
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
