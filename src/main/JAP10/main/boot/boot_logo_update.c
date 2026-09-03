#include "common.h"
#include "game/prim.h"
#include "game/asset.h"
#include "game/menu.h"
#include "game/race.h"
#include "game/render.h"
#include "game/render_internal.h"
#include "game/scratchpad.h"
#include "game/state.h"
#include "psyq/gpu.h"
#include "psyq/gte.h"


void UpdateBootLogoScene(void) {
    BootLogoState state;

    state = g_BootLogoState;
    switch (state) {
    case BOOT_LOGO_STATE_FADE_IN: {
        u32 sceneTime;

        sceneTime = g_SceneTimer;
        if (sceneTime < 0x102) {
            g_SceneTimer += 3;
        }
        if (g_AssetLoadState == 0) {
            g_BootLogoState = BOOT_LOGO_STATE_HOLD;
        }
        break;
    }
    case BOOT_LOGO_STATE_HOLD:
        g_SceneTimer -= 3;
        if (g_SceneTimer == 0) {
            g_BootLogoState = BOOT_LOGO_STATE_FADE_OUT;
            SetupDisplay240(0, 0, 0);
        }
        break;
    case BOOT_LOGO_STATE_FADE_OUT: {
        u32 sceneTime;

        g_SceneTimer++;
        sceneTime = g_SceneTimer;
        if (sceneTime >= 21) {
            BeginIntroFmv(3);
        }
        break;
    }
    }

    if (g_BootLogoState != BOOT_LOGO_STATE_FADE_OUT) {
        u32 sceneTime;

        DrawBootLogo();
        sceneTime = g_SceneTimer;
        if (sceneTime >= 10) {
            SetDispMask(1);
        }
    }
}
