#include "common.h"
#include "psyq/gte.h"
#include "game/render.h"
#include "game/state.h"
#include "game/cd.h"
#include "game/audio.h"
#include "game/race.h"
extern GameRenderView *g_CarModelAsset;
extern Matrix g_SceneLightMatrix;
extern s32 g_ScratchRenderMode;
s32 GetTrackZoneBlend(s32 arg0);
void DrawPlayerCarModel(GameRenderObject *obj);
extern u8 *g_CamRow;
void *ApplyMatrixLV(void *mtx, void *vec, void *out);
extern s32 g_StreamReturnScene;
void CdSync(s32 arg0, s32 arg1);
s32 CdControl(s32 com, void *param, s32 result);
void BeginFmv(s32 arg0);
extern u32 g_AssetBase;
void StartFmvPlayback(u32 arg0);

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
