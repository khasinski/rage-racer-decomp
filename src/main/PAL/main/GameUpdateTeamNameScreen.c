#include "common.h"
#include "game/menu.h"
#include "game/audio.h"
#include "game/render.h"

extern u32 D_80081D34;

void func_80051D6C(void);
s32 func_8004E724(s32 a, s32 b);
s32 func_800487D8(void *a, void *b, s32 c);
void func_8001D530(void *a, s32 b);

void GameUpdateTeamNameScreen(void) asm("func_8005873C");
void GameUpdateTeamNameScreen(void) {
    u16 pad;
    s32 newdepth;

    g_MenuAltLayout = g_MenuAltLayoutSetting;
    func_80051D6C();
    if (GameMenuBusy != 0) goto reopen;

    func_8004E724(1, GameMenuCursor);
    if (func_800487D8(&D_80081D34, &g_UiScriptProgress, 1) == 0) return;
    g_MenuOverlayPattern = -1;

    if (g_TeamNameLength < 6) {
        if ((g_PadEdge & 0xF000) && GameMenuCursorAnim < 0) {
            if (g_PadEdge & 0x1000) { s32 u = GameMenuCursor; GameMenuCursor = (u < 0xB) ? u + 0x21 : u - 0xB; }
            if (g_PadEdge & 0x4000) { s32 d = GameMenuCursor; GameMenuCursor = (d < 0x21) ? d + 0xB : d - 0x21; }
            if (g_PadEdge & 0x8000) { s32 l = GameMenuCursor; GameMenuCursor = (l % 11 != 0) ? l - 1 : l + 0xA; }
            if (g_PadEdge & 0x2000) {
                register s32 r asm("$5");
                register s32 res asm("$2");
                s32 rn;
                r = GameMenuCursor;
                rn = r + 1;
                if (rn % 11 == 0) res = r - 0xA; else res = rn;
                GameMenuCursor = res;
            }
            g_MenuViewAngleTarget = 0;
            g_MenuViewAngle = 0x3E8000;
            GameMenuCursorAnim = GameMenuCursor;
            GamePlaySoundCue(1);
            goto after_sound;
        }
        goto after_sound;
    }

    if (!((g_PadEdge & 0xA000) && GameMenuCursorAnim < 0)) goto after_sound;
    {
        s32 nc = (GameMenuCursor == 0x2A) ? 0x2B : 0x2A;
        GameMenuCursor = nc;
        g_MenuViewAngleTarget = 0;
        g_MenuViewAngle = 0x3E8000;
        GameMenuCursorAnim = nc;
    }
    GamePlaySoundCue(1);
after_sound:
    pad = g_PadEdge2;
    if (!(pad & 0x860)) goto maybe_pop;
    {
        s32 c = GameMenuCursor;
        if (c == 0x2A) goto pop;
        if (c != 0x2B) goto push;
    }
    GamePlaySoundCue(3);
    GameMenuBusy = 1;
    g_MenuOverlayPattern = 2;
    g_MenuViewOffsetTarget = 0x3D090;
    return;

push:
    {
        u32 d;
        GamePlaySoundCue(2);
        g_TeamNameChars[g_TeamNameLength] = (u8)GameMenuCursor;
        d = g_TeamNameLength;
        if (d >= 5) GameMenuCursor = 0x2B;
        if (d >= 7) newdepth = d; else newdepth = d + 1;
    }
    goto set_depth;

maybe_pop:
    if (!(pad & 0x90)) return;
pop:
    if (g_TeamNameLength == 0) return;
    GamePlaySoundCue(4);
    {
        register s32 tv asm("$2");
        tv = 0xA;
        g_TeamNameChars[g_TeamNameLength] = tv;
    }
    newdepth = g_TeamNameLength - 1;
set_depth:
    g_TeamNameLength = newdepth;
    return;

reopen:
    g_MenuHandlerIndex = -1;
    g_MenuHandlerIndex2 = 9;
    func_8004E724(-1, GameMenuCursor);
    func_800487D8(&D_80081D34, &g_UiScriptProgress, -1);
    if (g_UiScriptProgress > 0) return;
    if (0x3D08F < g_MenuViewOffset) {
        g_MenuScreen = 6;
        g_MenuHandlerIndex = 6;
        func_8001D530(&g_TeamNameChars, g_TeamNameLength);
        g_UiScriptProgress = 0;
        GameMenuBusy = 0;
    }
}
