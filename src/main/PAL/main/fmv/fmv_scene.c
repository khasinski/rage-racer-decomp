#include "common.h"
#include "game/fmv.h"
#include "game/asset.h"
#include "game/render.h"
#include "game/state.h"
#include "game/fmv_internal.h"
#include "game/cd.h"
#include "game/audio.h"

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
    FmvWorkBufferAddress workAddress;

    switch (g_FmvState) {
    case 0:
        workAddress.bytes = g_AssetBase;
        StartFmvPlayback(workAddress.buffers);
        /* fall through */
    case 1:
        DecodeFmvFrame();
        break;
    case 2:
        EndFmv();
        break;
    }
}
