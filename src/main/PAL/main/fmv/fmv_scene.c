#include "common.h"
#include "game/fmv.h"
#include "game/render.h"
#include "game/state.h"
#include "game/cd.h"
#include "game/audio.h"
extern s32 g_StreamReturnScene;
extern u32 g_AssetBase;

void BeginFmv(s32 returnScene) {
    CloseLoadedAudioSlots();
    ResetCdAudioState();
    g_FmvState = 0;
    g_StreamReturnScene = returnScene;
    g_SceneId = 5;
    CdSync(0, 0);
    CdControl(9, 0, 0);
}



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
