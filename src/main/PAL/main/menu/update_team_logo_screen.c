#include "common.h"
/*
 * func_80057748: sound/menu state machine. The redundant
 * `if (g_PadEdge2) { ... } else { ... }` around the state>0 block is a
 * deliberate no-op guard (both arms identical): its presence forces GCC 2.6.3
 * to rematerialize the literal 2 at both the direction ternary and the switch
 * case instead of CSE-ing it into a saved register, reproducing retail codegen.
 * Verified byte-exact; both arms are semantically identical so behaviour is
 * unchanged. This is the accepted resolution while the legal-only permuter
 * searches for a cleaner shape.
 */
#include "psyq/gpu.h"
#include "game/race.h"
#include "game/menu.h"
#include "game/render.h"

s32 GameDrawLogoSampleScreen(s32 arg0);

extern s32 g_MenuConfirmTimer asm("D_8009B300");
extern s32 g_MenuHintButtonsVisible asm("D_8009B314");
extern u8 g_MenuSubCursor asm("D_8009B2F0");
extern s32 D_8019CAB8;
extern void *D_801E8A44;
extern s32 D_801F1804;
extern u16 g_TeamLogoClut[] asm("D_801E444C");
extern Rect g_TeamLogoClutRect asm("D_8007BEDC");
extern u8 D_80082844;
extern u8 g_UiChromeScript2 asm("D_80082790");
extern u8 D_80081C14;
extern u8 g_UiChromeScript asm("D_80082460");
extern u8 D_80082574;
extern u8 D_80082664;
extern u8 g_MenuBlankCaption asm("D_80011BA0");

void func_80046A2C(void *ot, s32 x0, s32 y0, s32 x1, s32 y1, s32 u0, s32 v0,
                   s32 r, s32 g, s32 b, s32 clut, s32 sh, s32 st, s32 flags);
s32 func_800487D8(u8 *commands, s32 *progress, s32 step);
void func_800489AC(s32 arg0, s32 arg1, s32 arg2);
void func_80048B88(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6,
                   s32 a7, s32 a8, s32 a9, s32 a10);
void func_80048D64(s32 x0, s32 y0, s32 x1, s32 y1, s32 useFlash);
void func_8004A248(s32 arg0, s32 arg1);
void GameRampTeamLogoCanvas(s32 arg0, s32 arg1) asm("func_8004B8B4");
void GameUpdateTeamLogoCanvas(void) asm("func_8004C0D8");
void GameDrawLogoSamplePanel(s32 arg0, s32 arg1) asm("func_8004E368");
void GamePlaySoundCue(s32 cue) asm("func_8005D6EC");
void func_8005EA14(void);
void func_8005EA6C(void);
void LoadImage(Rect *rect, void *data) asm("func_80065B24");

void GameUpdateTeamLogoScreen(void) asm("func_80057748");
void GameUpdateTeamLogoScreen(void)
{
  void *ot;
  s32 state;
  s32 sel;
  s32 edge;
  s32 cnt;
  int new_var;
  ot = *((void **) 0x1F800004);
  g_MenuAltLayout = 0;
  state = GameMenuBusy;
  if (state == 0)
  {
    GameRampTeamLogoCanvas(-13, -21);
    func_800487D8(&D_80082844, &g_UiScriptProgress2, -1);
    func_800487D8(&g_UiChromeScript2, &g_UiScriptProgress2, 0);
    func_800487D8(D_801E8A44, &g_UiScriptProgress2, 0);
    func_8004A248(1, -1);
    func_800489AC(g_UiScriptProgress, 2, D_801F1804);
    func_800487D8(&D_80081C14, &g_UiScriptProgress, 0);
    if ((func_800487D8(&g_UiChromeScript, &g_UiScriptProgress, 1) != 0) && (g_UiScriptProgress2 <= 0))
    {
      g_MenuHintButtonsVisible = 1;
      g_MenuOverlayPattern = -1;
      if (g_PadEdge2 & 0x1000)
      {
        GamePlaySoundCue(1);
        D_801F1804 = (D_801F1804 > 0) ? (D_801F1804 - 1) : (2);
      }
      if (g_PadEdge2 & 0x4000)
      {
        GamePlaySoundCue(1);
        D_801F1804 = (D_801F1804 < 2) ? (D_801F1804 + 1) : (0);
      }
      edge = g_PadEdge2;
      if (edge & 0x860)
      {
        sel = D_801F1804;
        if (sel == 0)
        {
          GamePlaySoundCue(2);
          GameMenuBusy = -1;
          g_MenuSubCursor = 0;
          g_UiScriptProgress2 = 0;
          D_801E8A44 = &D_80082574;
        }
        else
          if (sel == 1)
        {
          GamePlaySoundCue(2);
          func_8005EA14();
          GameMenuBusy = -3;
          D_8019CAB8 = 0;
          g_UiScriptProgress2 = 0;
          D_801E8A44 = &D_80082664;
        }
        else
          if (sel == 2)
        {
          GamePlaySoundCue(3);
          GameMenuBusy = sel;
          g_MenuOverlayPattern = sel;
        }
      }
      else
        if (edge & 0x90)
      {
        GamePlaySoundCue(3);
        GameMenuBusy = 2;
        g_MenuOverlayPattern = 2;
      }
    }
  }
  else
    if (state < 0)
  {
    if (state == (-1))
    {
      u16 *pad;
      func_800487D8(&D_80082844, &g_UiScriptProgress2, 0);
      func_800487D8(&g_UiChromeScript2, &g_UiScriptProgress2, 0);
      if (func_800487D8(D_801E8A44, &g_UiScriptProgress2, 1) != 0)
      {
        if (g_PadEdge2 & 0x860)
        {
          if (g_MenuSubCursor != 0)
          {
            GamePlaySoundCue(2);
            GameMenuBusy = -2;
            g_MenuConfirmTimer = 0x23;
          }
          else
          {
            GamePlaySoundCue(3);
            GameMenuBusy = 0;
          }
        }
        pad = (u16 *) (&g_PadEdge2);
        if ((*pad) & 0x90)
        {
          GamePlaySoundCue(3);
          GameMenuBusy = 0;
        }
        if ((*pad) & 0x8000)
        {
          if (g_MenuSubCursor == 0)
          {
            GamePlaySoundCue(1);
            g_MenuSubCursor = 1;
          }
        }
        if (g_PadEdge2 & 0x2000)
        {
          if (g_MenuSubCursor != 0)
          {
            GamePlaySoundCue(1);
            g_MenuSubCursor = 0;
          }
        }
        func_80048D64((g_MenuSubCursor != 0) ? (0xB8) : (0xDA), 0x44, 0x20, 0x20, 0);
        func_80046A2C(ot, 0xC0, 0x4C, 0x10, 0x10, 0x9D, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
        func_80046A2C(ot, 0xE3, 0x4C, 0x10, 0x10, 0xAD, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
        new_var = 0x20;
        func_80048B88(0xB8, 0x44, 0x20, new_var, 0x95, 0x25, 0x1E, 0, 0, 0, (s32) (&g_MenuBlankCaption));
        func_80048B88(0xDA, 0x44, 0x20, 0x20, 0x3A, 0x1E, 0x95, 0, 0, 0, (s32) (&g_MenuBlankCaption));
      }
      func_8004A248(1, 0);
    }
    else
      if (state == (-2))
    {
      cnt = g_MenuConfirmTimer;
      if (cnt <= 0)
      {
        func_800487D8(&D_80082844, &g_UiScriptProgress2, -1);
        func_800487D8(&g_UiChromeScript2, &g_UiScriptProgress2, 0);
        func_800487D8(D_801E8A44, &g_UiScriptProgress2, 0);
        if (g_UiScriptProgress2 <= 0)
        {
          GameMenuBusy = 1;
          g_MenuOverlayPattern = 1;
        }
      }
      else
      {
        g_MenuConfirmTimer = cnt - 1;
        func_800487D8(&D_80082844, &g_UiScriptProgress2, 0);
        func_800487D8(&g_UiChromeScript2, &g_UiScriptProgress2, 0);
        func_800487D8(D_801E8A44, &g_UiScriptProgress2, 1);
        func_80048D64((g_MenuSubCursor != 0) ? (0xB8) : (0xDA), 0x44, 0x20, 0x20, 1);
        func_80046A2C(ot, 0xC0, 0x4C, 0x10, 0x10, 0x9D, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
        func_80046A2C(ot, 0xE3, 0x4C, 0x10, 0x10, 0xAD, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
        func_80048B88(0xB8, 0x44, 0x20, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, (s32) (&g_MenuBlankCaption));
        func_80048B88(0xDA, 0x44, 0x20, 0x20, 0x3A, 0x1E, 0x95, 0, 0, 0, (s32) (&g_MenuBlankCaption));
      }
      func_8004A248(1, 0);
    }
    else
      if (state == (-3))
    {
      GameRampTeamLogoCanvas(9, 0x15);
      if (func_800487D8(D_801E8A44, &g_UiScriptProgress2, 1) != 0)
      {
        if (g_PadEdge2 & 0x800)
        {
          GamePlaySoundCue(3);
          func_8005EA6C();
          GameMenuBusy = -4;
        }
        GameUpdateTeamLogoCanvas();
      }
      if (g_UiScriptProgress2 >= 8)
      {
        g_MenuHintButtonsVisible = 0;
      }
      func_8004A248(1, 1);
    }
    else
    {
      GameRampTeamLogoCanvas(-13, -21);
      func_800487D8(D_801E8A44, &g_UiScriptProgress2, -1);
      func_8004A248(1, -1);
      if (g_UiScriptProgress2 < 7)
      {
        g_MenuHintButtonsVisible = 1;
      }
      if (g_UiScriptProgress2 <= 0)
      {
        GameMenuBusy = 0;
      }
    }
    func_800489AC(g_UiScriptProgress, 2, D_801F1804);
    func_800487D8(&D_80081C14, &g_UiScriptProgress, 0);
    func_800487D8(&g_UiChromeScript, &g_UiScriptProgress, 1);
  }
  else
  {
    if (g_PadEdge2)
    {
      g_MenuHandlerIndex = -1;
      g_MenuHandlerIndex2 = 7;
      func_8004A248((state == 2) ? (-1) : (1), 0);
      func_800487D8(&D_80081C14, &g_UiScriptProgress, -1);
      func_800487D8(&g_UiChromeScript, &g_UiScriptProgress, 0);
      func_800489AC(g_UiScriptProgress, 2, D_801F1804);
    }
    else
    {
      g_MenuHandlerIndex = -1;
      g_MenuHandlerIndex2 = 7;
      func_8004A248((state == 2) ? (-1) : (1), 0);
      func_800487D8(&D_80081C14, &g_UiScriptProgress, -1);
      func_800487D8(&g_UiChromeScript, &g_UiScriptProgress, 0);
      func_800489AC(g_UiScriptProgress, 2, D_801F1804);
    }
    if (g_UiScriptProgress <= 0)
    {
      switch (GameMenuBusy)
      {
        case 1:
          g_MenuScreen = 8;
          g_MenuHandlerIndex = 8;
          GameDrawLogoSamplePanel(0, 0);
          break;

        case 2:
          g_MenuScreen = 6;
          g_MenuHandlerIndex = 6;
          D_801F1804 = 0;
          g_TeamLogoClut[0] = 0;
          LoadImage(&g_TeamLogoClutRect, g_TeamLogoClut);
          break;

        default:
            break;

      }

      g_UiScriptProgress = 0;
      GameMenuBusy = 0;
    }
  }
}

extern s32 D_8009B2DC;

s32 GameDrawLogoSampleScreen(s32 step) asm("func_8005803C");
s32 GameDrawLogoSampleScreen(s32 arg0) {
    s32 value;

    if (arg0 == 0) {
        D_8009B2DC = 0;
        return;
    }

    if (arg0 > 0) {
        value = arg0 + D_8009B2DC;
        D_8009B2DC = value;
        if (value >= 0x1FD) {
            D_8009B2DC = 0x1FC;
        }
    } else {
        value = arg0 + D_8009B2DC;
        D_8009B2DC = value;
        if (value < 0) {
            D_8009B2DC = 0;
        }
    }

    return D_8009B2DC;
}
