#include "common.h"
#include "game/state.h"
#include "game/menu.h"
#include "game/race.h"
#include "game/audio.h"
#include "game/screens.h"
#include "game/sound.h"
#include "game/car.h"


/* Scene 19: counts the prize money and then the class-clear bonus into the save block. */
void UpdatePrizeMoneyScreen(void) {
    s32 lim1 = g_PrizeCountStep;
    s32 lim0 = g_BonusCountStep;
    s32 st;
    s32 t;

    if (g_PadHeld & PAD_CONFIRM) {
        lim1 <<= 2;
        lim0 <<= 2;
    }

    switch (g_PrizeScreenState) {
    case 0:
        g_SceneTimer -= 8;
        DrawFullscreenFadeTile(g_SceneTimer, 0x49);
        if (g_SceneTimer == 0) g_PrizeScreenState = 1;
        DrawRaceTimePanel(0);
        DrawGrandprixIntro();
        return;
    case 1:
        DrawRaceTimePanel(0);
        if (g_PadPressed & PAD_CONFIRM) {
            g_PrizeScreenState = 2;
            g_SceneTimer = 0;
        }
        DrawGrandprixIntro();
        return;
    case 2:
        g_SceneTimer += 8;
        DrawRaceTimePanel(g_SceneTimer);
        if ((u32)g_SceneTimer >= 129) g_PrizeScreenState = 3;
        DrawGrandprixIntro();
        return;
    case 3:
        g_SceneTimer -= 8;
        DrawPrizeMoneyPanel(g_SceneTimer);
        if (g_SceneTimer == 0) g_PrizeScreenState = 4;
        DrawGrandprixIntro();
        return;
    case 4:
        g_SceneTimer += 1;
        if (!((u32)g_SceneTimer < 121)) {
        if (g_PrizeAmount == 0) {
        g_SceneTimer = 0;
        if (g_PromotionBonus == 0) goto Lstore7;
        st = 5;
        goto Lstore;
        }
        PlaySoundCue((g_PadHeld & PAD_CONFIRM) ? 0x10 : 0xf);
        t = g_PrizeAmount;
        if (t >= lim1) {
            g_PrizeAmount = t - lim1;
            g_RaceProgress->money += lim1;
        } else {
            s32 e = g_RaceProgress->money;
            g_PrizeAmount = 0;
            g_RaceProgress->money = e + t;
        }
        }
        if (g_PrizeAmount != 0) break;
        g_SceneTimer = 0;
        if (g_PromotionBonus == 0) goto Lstore7;
        st = 5;
        goto Lstore;
    case 5:
        PlaySoundCue(0x11);
        if (!(g_PadPressed & PAD_CONFIRM)) break;
        st = 6;
    Lstore:
        g_PrizeScreenState = st;
        break;
    case 6:
        TickClassClearFanfare();
        if (g_PromotionBonus == 0) { st = 7; goto Lstore; }
        PlaySoundCue((g_PadHeld & PAD_CONFIRM) ? 0x10 : 0xf);
        t = g_PromotionBonus;
        if (t >= lim0) {
            g_PromotionBonus = t - lim0;
            g_RaceProgress->money += lim0;
        } else {
            s32 e = g_RaceProgress->money;
            g_PromotionBonus = 0;
            g_RaceProgress->money = e + t;
        }
        if (g_PromotionBonus != 0) break;
    Lstore7:
        st = 7;
        goto Lstore;
    case 7:
        TickClassClearFanfare();
        PlaySoundCue(0x11);
        if (!(g_PadPressed & PAD_CONFIRM)) break;
        if (g_ClassClearFanfareTimer != 0) break;
        if (g_ClassCompleted == 0) { RequestSelectBgmAssets(); }
        st = 8;
        goto Lstore;
    case 8:
        if (g_SeriesCleared != 0)
            g_SceneTimer += 1;
        else
            g_SceneTimer += 2;
        DrawFullscreenFadeTile(g_SceneTimer, 0x49);
        if ((u32)g_SceneTimer < 0x100) break;
        AdvanceGrandPrixClass();
        break;
    default:
        DrawGrandprixIntro();
        return;
    }
    DrawPrizeMoneyPanel(0);
    DrawGrandprixIntro();
}
