#include "common.h"
#include "game/menu.h"
#include "game/audio.h"

extern s32 D_8009B338, D_8019CB0C, D_8009B318;
extern s32 D_8009B350, D_8009B34C, D_8009B35C, D_8009B340, D_8009B344, D_8009B358;
extern s32 D_8019C9F0, D_8019C9F8;
extern u32 D_80081D34;

void func_80051D6C(void);
s32 func_8004E724(s32 a, s32 b);
s32 func_800487D8(void *a, void *b, s32 c);
void func_8001D530(void *a, s32 b);

void func_8005873C(void) {
    u16 pad;
    s32 newdepth;

    D_8019CB0C = D_8009B338;
    func_80051D6C();
    if (GameMenuBusy != 0) goto reopen;

    func_8004E724(1, GameMenuCursor);
    if (func_800487D8(&D_80081D34, &D_8019C9F0, 1) == 0) return;
    D_8009B318 = -1;

    if (GameMenuStackDepth < 6) {
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
            D_8009B350 = 0;
            D_8009B34C = 0x3E8000;
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
        D_8009B350 = 0;
        D_8009B34C = 0x3E8000;
        GameMenuCursorAnim = nc;
    }
    GamePlaySoundCue(1);
after_sound:
    pad = g_PadEdgeHeld;
    if (!(pad & 0x860)) goto maybe_pop;
    {
        s32 c = GameMenuCursor;
        if (c == 0x2A) goto pop;
        if (c != 0x2B) goto push;
    }
    GamePlaySoundCue(3);
    GameMenuBusy = 1;
    D_8009B318 = 2;
    D_8009B35C = 0x3D090;
    return;

push:
    {
        u32 d;
        GamePlaySoundCue(2);
        GameMenuStack[GameMenuStackDepth] = (u8)GameMenuCursor;
        d = GameMenuStackDepth;
        if (d >= 5) GameMenuCursor = 0x2B;
        if (d >= 7) newdepth = d; else newdepth = d + 1;
    }
    goto set_depth;

maybe_pop:
    if (!(pad & 0x90)) return;
pop:
    if (GameMenuStackDepth == 0) return;
    GamePlaySoundCue(4);
    {
        register s32 tv asm("$2");
        tv = 0xA;
        GameMenuStack[GameMenuStackDepth] = tv;
    }
    newdepth = GameMenuStackDepth - 1;
set_depth:
    GameMenuStackDepth = newdepth;
    return;

reopen:
    D_8009B340 = -1;
    D_8009B344 = 9;
    func_8004E724(-1, GameMenuCursor);
    func_800487D8(&D_80081D34, &D_8019C9F0, -1);
    if (D_8019C9F0 > 0) return;
    if (0x3D08F < D_8009B358) {
        D_8019C9F8 = 6;
        D_8009B340 = 6;
        func_8001D530(&GameMenuStack, GameMenuStackDepth);
        D_8019C9F0 = 0;
        GameMenuBusy = 0;
    }
}
