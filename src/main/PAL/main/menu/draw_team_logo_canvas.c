#include "common.h"
#include "game/audio.h"
#include "game/menu.h"
#include "game/scratchpad.h"
#include "psyq/gpu.h"
extern void DrawRectOutline(
    s32 ot, s16 x, s16 y, s16 w, s32 h, u8 r, u8 g, u8 b, u8 alpha);
extern void GameDrawTexturedQuad(
    s32 ot,
    s16 x0,
    s16 y0,
    s16 x1,
    s16 y1,
    s16 x2,
    s16 y2,
    s16 x3,
    s16 y3,
    u8 u0,
    u8 v0,
    u8 u1,
    u8 v1,
    u8 u2,
    u8 v2,
    u8 u3,
    u8 v3,
    u8 r,
    u8 g,
    u8 b,
    s32 clut,
    s32 shadeTex,
    s32 semiTrans,
    s32 tpage);

typedef struct TeamLogoClutPos {
    s16 cx;
    s16 cy;
} TeamLogoClutPos;

typedef struct TeamLogoTexturePos {
    s16 tx;
    u16 ty;
} TeamLogoTexturePos;

typedef union {
    s32 value;
    u16 lo;
    s16 slo;
} TeamLogoWord;

extern TeamLogoClutPos g_TeamLogoClutRect;
extern TeamLogoTexturePos g_TeamLogoRect;
extern TeamLogoWord D_8007F934;
extern TeamLogoWord D_8007F93C;
extern s32 D_8007F950;
extern u8 g_PadType;
extern u16 D_801E6F2C asm("g_TeamLogoCanvas");

extern void LoadImage(void *rect, void *data);
extern s32 rsin(s32 angle);
extern void SetDrawClipRect(s32 ot, s16 x, s16 y, s16 w, s16 h);
extern void DrawSprite(
    s32 ot,
    s16 x,
    s16 y,
    s16 w,
    s16 h,
    u8 u,
    u8 v,
    u8 r,
    u8 g,
    u8 b,
    u16 clut,
    s32 shadeTex,
    s32 semiTrans,
    s32 flags);
extern void func_80046A2C_prepared(
    s32 ot,
    s16 x,
    s16 y,
    s16 w,
    s16 h,
    s32 u,
    s32 v,
    u8 r,
    u8 g,
    u8 b,
    u16 clut,
    s32 shadeTex,
    s32 semiTrans,
    s32 flags) asm("DrawSprite");
extern void DrawLine(
    s32 ot, s16 x0, s16 y0, s16 x1, s16 y1, u8 r, u8 g, u8 b, u8 alpha);
extern void GameDrawNumber(
    s16 x, s32 y, s16 flags, s32 value, u8 r, u8 g, u8 b, s32 clut, s32 primitiveCount);

void GameDrawSolidRectWide(void* ot, s32 x0, s32 y0, s32 x1, s32 y1, s32 r, s32 g, s32 b, s32 alpha) asm("DrawSolidRect");
void func_80047024_prepared(s32 ot, s16 x0, s16 y0, s16 x1, s16 y1, u8 r, u8 g, u8 b, u8 alpha) asm("DrawSolidRect");

void DrawTeamLogoCanvasFade(s32 delta) {
    u8 *scratch;
    s32 value;
    s32 sum;
    s32 limit;
    s32 alpha;

    scratch = SCRATCH_OT_BASE_AS(u8);
    if (delta > 0) {
        value = D_8009B280;
        sum = delta + value;
        value = sum;
        D_8009B280 = value;
        if (0xFFFF < value) {
            D_8009B280 = 0xFFFF;
        }
    } else {
        value = D_8009B280;
        sum = delta + value;
        value = sum;
        D_8009B280 = value;
        if (value < 0) {
            D_8009B280 = 0;
        }
    }

    limit = 0x1E0;
    D_8009B284 = D_8009B280 >> 8;
    alpha = D_8009B284;
    GameDrawSolidRectWide(scratch + 0x18, 0x48, 0, 0xF8, limit, alpha, alpha, alpha, 0x40);
}

void DrawTeamLogoCanvas(s32 panelStep, s32 editorStep)
{
  s32 kreg;
  s32 a0v;
  s32 a1v;
  s32 ot;
  s32 i;
  s32 d;
  s32 v;
  s32 ang;
  s32 gx;
  s32 gy;
  s32 gx2;
  s32 gy2;
  s32 pal;
  u8 clut;
  u16 x0;
  u16 x1;
  u16 y1;
  s32 x2;
  s32 yA0;
  s16 yA8;
  s16 yB8;
  s16 yC8;
  register s32 phaseValue asm("$2");
  register s32 secondaryValue asm("$3");
  register s32 drawValue asm("$4");
  a0v = panelStep;
  a1v = editorStep;
  ot = SCRATCH_OT_BASE_WORD;
  if (panelStep == 0)
  {
    D_8007FB0C = 0;
    D_8007FB10 = 0;
    return;
  }
  g_TeamLogoClut[0] = 0x8000;
  g_TeamLogoClut[0] |= ((rsin(D_8009B288 % 0x1000) / 128) + 0x20) >> 3;
  ang = D_8009B288 + 0x55;
  g_TeamLogoClut[0] |= (((rsin(ang % 0x1000) / 128) + 0x20) >> 3) << 5;
  ang = D_8009B288 + 0xAA;
  d = rsin(ang % 0x1000);
  i = 0;
  if (d < 0)
  {
    d += 0x7F;
  }
  {
    u16 *dst;
    s32 mul;
    u16 k8;
    k8 = 0x8000;
    g_TeamLogoClut[0] |= (((d >> 7) + 0x20) >> 3) << 10;
    D_8009B2A0[0] = g_TeamLogoClut[0];
    dst = D_8009B2A0;
    D_8009B288 += 0x20;
    mul = D_8009B298;
    loop16:
    {
      u16 *src = &g_TeamLogoClut[i];
      *dst = k8;
      phaseValue = (((*src) & 0x1F) * mul) / 256;
      drawValue = phaseValue | ((u16) 0x8000);
      *dst = drawValue;
      drawValue |= (((((*src) >> 5) & 0x1F) * mul) / 256) << 5;
      *dst = drawValue;
      v = (((*src) >> 10) & 0x1F) * mul;
      if (v < 0)
      {
        v += 0xFF;
      }
      i++;
      phaseValue = drawValue | ((v >> 8) << 10);
      *dst = phaseValue;
      dst++;
      if (i < 16)
      {
        goto loop16;
      }
    }

  }
  LoadImage(&g_TeamLogoRect, &D_801E6F2C);
  LoadImage(&g_TeamLogoClutRect, g_TeamLogoClut);
  LoadImage(&D_8007F95C, D_8009B2A0);
  if (a0v < 0)
  {
    D_8007FB0C = a0v + D_8007FB0C;
    if (D_8007FB0C < 0)
    {
      D_8007FB0C = 0;
    }
  }
  if (a1v < 0)
  {
    D_8007FB10 = a1v + D_8007FB10;
    if (D_8007FB10 < 0)
    {
      D_8007FB10 = 0;
    }
  }
  d = D_8007FB0C - 0xA;
  if (d >= 0)
  {
    s32 sy2;
    s32 sy;
    register s32 drawX asm("$5");
    register s32 sy2Arg;
    u8 ff;
    s32 sx;
    s32 x88;
    s32 w1;
    s32 delta;
    s32 scaleDelta;
    s32 texY;
    s32 gyTemp;
    if (d >= 0xC)
    {
      d = 0xB;
    }
    x0 = 0x87;
    drawX = 0x87;
    asm("" : : "r"(drawX));
    sy = (((u32) (d * 0x460)) >> 5) + 0xFEC9;
    ff = 0xFF;
    DrawRectOutline(ot, drawX, sy, 0x82, 0x104, 0xB4, 0xB4, 0xB4, ff);
    kreg = sy;
    if (D_8009B29C >= 0x100)
    {
      if (D_8007F954 == 0)
      {
        s32 syOffset;
        register s32 angleSource;
        register s32 angleValue;
        angleSource = D_8009B288;
        secondaryValue = D_8007F934.value;
        angleValue = angleSource * 2;
        drawValue = angleValue;
        secondaryValue *= 4;
        sy2 = secondaryValue + 0x88;
        sy2Arg = sy2;
        syOffset = (D_8007F938 * 8) + 2;
        asm("" : "=r"(sy) : "0"(sy));
        sy += syOffset;
        if (angleValue < 0)
        {
          drawValue = angleValue + 0xFFF;
        }
        drawValue >>= 12;
        drawValue *= 0x1000;
        drawValue = angleValue - drawValue;
        clut = (rsin(drawValue) / 64) - 0x41;
        DrawRectOutline(ot, sy2Arg, sy, D_8007F94C * 4, (s16) (D_8007F94C * 8), 0, clut, 0, ff);
      }
    }
    sx = (s16) kreg;
    x2 = ((s16) x0) - (D_8007F948 < 0x220);
    if (D_8007F948 < 0x220)
    {
      sx -= 2;
    }
    w1 = sx + 0x110;
    x88 = x2 + 0x88;
    delta = 0x220 - D_8007F948;
    scaleDelta = (delta * D_8007F93C.value) / 272;
    phaseValue = (g_TeamLogoRect.tx * 4) - 1;
    drawValue = phaseValue + scaleDelta;
    gx = drawValue;
    scaleDelta = (delta * D_8007F940) / 272;
    texY = (*(u8 *)(&g_TeamLogoRect.ty)) - 1;
    gyTemp = texY + scaleDelta;
    phaseValue = gyTemp;
    gy = phaseValue;
    gx2 = drawValue + (D_8007F948 / 8);
    asm("" : : "r"(scaleDelta));
    gy2 = phaseValue + (D_8007F948 / 8);
    clut = (g_TeamLogoRect.ty >> 4) & 0x10;
    clut |= (g_TeamLogoRect.tx & 0x3FF) >> 6;
    SetDrawClipRect(ot, 0, 0, 0x140, 0x1E0);
    GameDrawTexturedQuad(ot, x2, sx, x88, sx, x2, w1, x88, w1, gx, gy, gx2, gy, gx, gy2, gx2, gy2, 0x7F, 0x7F, 0x7F, 0x27F, 1, 0, clut);
    SetDrawClipRect(ot, x0 + 1, kreg + 2, 0x80, 0x100);
  }
  d = D_8007FB0C - 0xE;
  if (d >= 0)
  {
    register s32 sy asm("$16");
    u32 su;
    s32 ff;
    s16 w1;
    s16 xb;
    if (d >= 8)
    {
      d = 7;
    }
    su = ((u32) (-(d * 0x460))) >> 5;
    sy = su + 0x1FB;
    x0 = 0x2F;
    ff = 0xFF;
    DrawRectOutline(ot, 0x2F, sy, 0x42, 0x84, 0xB4, 0xB4, 0xB4, ff);
    kreg = sy;
    if ((D_8009B29C >= 0x100) && (D_8007F944 != 0))
    {
      x1 = D_8007F93C.lo + 0x30;
      y1 = sy + ((D_8007F940 * 2) + 2);
      clut = (rsin((D_8009B288 * 2) % 0x1000) / 64) - 0x41;
      if (D_8007F944 == 2)
      {
        s16 ya;
        s16 yb;
        s16 xa;
        ya = su + 0x1FD;
        yb = su + 0x27D;
        xa = x1 + D_8007F934.lo;
        DrawLine(ot, xa, ya, xa, yb, clut, clut, clut, ff);
        xa = ((x1 + D_8007F934.lo) + ((u16) D_8007F94C)) - 1;
        DrawLine(ot, xa, ya, xa, yb, clut, clut, clut, ff);
        xa = y1 + (D_8007F938 * 2);
        DrawLine(ot, 0x30, xa, 0x70, xa, clut, clut, clut, ff);
        {
          s32 odd;
          odd = D_8007F938 * 2 + 1;
          xa = y1 + odd;
        }
        DrawLine(ot, 0x30, xa, 0x70, xa, clut, clut, clut, ff);
        xa = y1 + (((D_8007F938 + D_8007F94C) - 1) * 2);
        DrawLine(ot, 0x30, xa, 0x70, xa, clut, clut, clut, ff);
        {
          s32 odd;
          odd = ((D_8007F938 + D_8007F94C) - 1) * 2 + 1;
          xa = y1 + odd;
        }
        DrawLine(ot, 0x30, xa, 0x70, xa, clut, clut, clut, ff);
      }
      else
        if (D_8007F94C == 1)
      {
        s16 xa = x1 + D_8007F934.lo;
        s16 ya = y1 + (D_8007F938 * 2);
        DrawLine(ot, xa, ya, xa, (s16) (ya + 1), clut, clut, clut, ff);
      }
      else
      {
        DrawRectOutline(ot, x1 + D_8007F934.lo, y1 + (D_8007F938 * 2), (s16) D_8007F94C, (s16) (D_8007F94C * 2), clut, clut, clut, 0xFF);
      }
      DrawRectOutline(ot, x1, y1, 0x20, 0x40, 0, clut, 0, 0xFF);
    }
    gx = (g_TeamLogoRect.tx * 4) - 1;
    gy = (*(u8 *)(&g_TeamLogoRect.ty)) - 1;
    gx2 = gx;
    gx2 += 0x41;
    gy2 = gy;
    gy2 += 0x41;
    pal = GetClut(g_TeamLogoClutRect.cx, g_TeamLogoClutRect.cy);
    clut = (g_TeamLogoRect.ty >> 4) & 0x10;
    clut |= (g_TeamLogoRect.tx & 0x3FF) >> 6;
    w1 = kreg + 0x83;
    xb = x0 + 0x41;
    SetDrawClipRect(ot, 0, 0, 0x140, 0x1E0);
    asm("" : "=r"(gx2) : "0"(gx2));
    GameDrawTexturedQuad(ot, x0, kreg, xb, kreg, x0, w1, xb, w1, gx, gy, gx2, gy, gx, gy2, gx2, gy2, 0x7F, 0x7F, 0x7F, pal & 0xFFFF, 1, 0, clut);
    SetDrawClipRect(ot, x0 + 1, kreg + 2, 0x40, 0x80);
  }
  d = D_8007FB10 - 8;
  if (d >= 0)
  {
    s32 j;
    register s32 vs7 asm("$23");
    register s32 vs6 asm("$22");
    u32 panelY;
    if (d >= 6)
    {
      d = 5;
    }
    x0 = 0x8A;
    panelY = ((u32) (-(d * 0x3C0))) >> 5;
    kreg = panelY + 0x1EA;
    y1 = panelY + 0x1E7;
    x1 = (D_8007F950 * 8) + 0x80;
    if (D_8007F954 == 1)
    {
      s32 panelAng;
      panelAng = D_8009B288 * 2;
      clut = (rsin(panelAng % 0x1000) / 64) - 0x41;
      DrawRectOutline(ot, x1, y1, 0xD, 0x1A, 0, clut, 0, 0xFF);
    }
    else
    {
      DrawRectOutline(ot, x1, y1, 0xD, 0x1A, 0xB4, 0xB4, 0xB4, 0xFF);
    }
    func_80047024_prepared(ot, x1 + 1, y1 + 2, 0xB, 0x16, (*(u8 *)(&g_TeamLogoClut[D_8007F950])) * 8, (g_TeamLogoClut[D_8007F950] >> 2) & 0xF8, (g_TeamLogoClut[D_8007F950] >> 7) & 0xF8, 0xFF);
    {
      s32 fy2 = (kreg + 2) << 16;
      i = 0;
      j = 1;
      loop15:
      func_80047024_prepared(ot, x0 + j, fy2 >> 16, 8, 0x10, (*(u8 *)(&g_TeamLogoSwatches[i])) * 8, (g_TeamLogoSwatches[i] >> 2) & 0xF8, (g_TeamLogoSwatches[i] >> 7) & 0xF8, 0xFF);

      i++;
      j += 8;
      if (i < 15)
      {
        goto loop15;
      }
    }
    {
      DrawRectOutline(ot, x0, kreg, 0x7A, 0x14, 0xB4, 0xB4, 0xB4, 0xFF);
      y1 = kreg + 0x1C;
    i = 0;
    yA0 = (kreg + 0x22) << 16;
    asm("" : : "r"(yA0));
    kreg = 0x60;
    vs7 = -0x30;
    vs6 = -0xF;
    for (; i < 4; i++)
    {
      register s32 shade asm("$21");
      register s32 swatchWidth asm("$7");
      s32 gxArg;
      s32 gyArg;
      register s32 clutArg asm("$20");

      x1 = x0 + vs6;
      if (g_PadType == 0x23)
      {
        gx = vs7;
        gy = 0;
        pal = 0x233;
        clut = 0x1E;
      }
      else
      {
        gx = kreg;
        gy = 0x58;
        pal = 0x1F6;
        clut = 0x1C;
      }
      gxArg = (u8) gx;
      gyArg = (u8) gy;
      asm("" : : "r"(gxArg), "r"(gyArg));
      asm("" : : "r"(pal));
      swatchWidth = 0xC;
      asm("" : : "r"(swatchWidth));
      shade = (u8) (i * 0x24);
      asm("" : : "r"(shade));
      drawValue = ot;
      asm("" : : "r"(drawValue));
      asm("" : : "r"(x1));
      asm("" : "=r"(clutArg) : "0"(clut));
      func_80046A2C_prepared(drawValue, x1 + 0x13, yA0 >> 16, swatchWidth, 0x18,
                            gxArg, gyArg, 0, 0, 0, (u16) pal, 1, 0, clutArg);
      asm("" : "=r"(shade) : "0"(shade));
      secondaryValue = (u8) shade;
      DrawSprite(ot, x1, y1, 0x22, 0x32, secondaryValue, 0xC0,
                    0, 0, 0, 0x1F5, 1, 0, 0x1D);
      kreg += 0xC;
      vs7 += 0xC;
      vs6 += 0x28;
      }
    }
  }
  d = D_8007FB10 - 7;
  if (d >= 0)
  {
    if (d >= 7)
    {
      d = 6;
    }
    DrawSprite(ot, (((u32) (d * 0x250)) >> 5) + 0xFFA1, 0xC0, 0x61, 0x32, 0x90, 0xC0, 0, 0, 0, 0x1F5, 1, 0, 0x1D);
  }
  d = D_8007FB10 - 8;
  if ((d >= 0) && (D_8007F930 != 0))
  {
    s16 sy;
    s16 sx;
    s16 xa;
    s16 xb;
    s16 xc;
    s32 x0Calc;
    s32 syBase;
    register s32 tileSize;
    kreg = 0xC8;
    if (d >= 6)
    {
      d = 5;
    }
    x0Calc = (((u32) (-(d * 0x140))) >> 5) + 0x140;
    asm("" : : "r"(x0Calc));
    syBase = D_8007F958;
    x0 = (u16) x0Calc;
    sy = (syBase * 0x30) + 0xD9;
    if (D_8007F954 == 1)
    {
      clut = (rsin((D_8009B288 * 2) % 0x1000) / 64) - 0x41;
      DrawRectOutline(ot, x0, sy, 0x12, 0x15, 0, clut, 0, 0xFF);
    }
    yA8 = (s16) (kreg + 0x14);
    sx = 0x3F;
    sx = x0 - sx;
    GameDrawNumber(sx, yA8, 3, g_TeamLogoClut[D_8007F950] & 0x1F, 0x7F, 0x7F, 0x7F, 0x244, 0x20);
    yB8 = (s16) (kreg + 0x44);
    GameDrawNumber(sx, yB8, 3, (g_TeamLogoClut[D_8007F950] >> 5) & 0x1F, 0x7F, 0x7F, 0x7F, 0x244, 0x20);
    yC8 = (s16) (kreg + 0x74);
    GameDrawNumber(sx, yC8, 3, (g_TeamLogoClut[D_8007F950] >> 10) & 0x1F, 0x7F, 0x7F, 0x7F, 0x244, 0x20);
    {
      s32 alpha;
      alpha = 0xFF;
      DrawRectOutline(ot, x0, kreg, 0x12, 0x26, 0xB4, 0xB4, 0xB4, alpha);
      DrawRectOutline(ot, x0, kreg + 0x30, 0x12, 0x26, 0xB4, 0xB4, 0xB4, alpha);
      DrawRectOutline(ot, x0, kreg + 0x60, 0x12, 0x26, 0xB4, 0xB4, 0xB4, alpha);
    }
    xa = x0 + 1;
    xb = x0 + 0x11;
    DrawLine(ot, xa, kreg + 0x11, xb, kreg + 0x11, 0xB4, 0xB4, 0xB4, 0xFF);
    DrawLine(ot, xa, kreg + 0x12, xb, kreg + 0x12, 0xB4, 0xB4, 0xB4, 0xFF);
    DrawLine(ot, xa, kreg + 0x41, xb, kreg + 0x41, 0xB4, 0xB4, 0xB4, 0xFF);
    DrawLine(ot, xa, kreg + 0x42, xb, kreg + 0x42, 0xB4, 0xB4, 0xB4, 0xFF);
    DrawLine(ot, xa, kreg + 0x71, xb, kreg + 0x71, 0xB4, 0xB4, 0xB4, 0xFF);
    DrawLine(ot, xa, kreg + 0x72, xb, kreg + 0x72, 0xB4, 0xB4, 0xB4, 0xFF);
    xc = x0 + 5;
    tileSize = 0x10;
    DrawSprite(ot, xc, kreg + 2, 8, tileSize, 0xD8, 0x18, 0, 0, 0, 0x244, 1, 1, 0x5B);
    DrawSprite(ot, xc, kreg + 0x32, 8, tileSize, 0x80, 0x18, 0, 0, 0, 0x244, 1, 1, 0x5B);
    DrawSprite(ot, xc, kreg + 0x62, 8, tileSize, 0x58, 0x18, 0, 0, 0, 0x244, 1, 1, 0x5B);
    func_80047024_prepared(ot, xa, kreg + 2, 0x10, tileSize, 0xC0, 0, 0, 0xFF);
    func_80047024_prepared(ot, xa, yA8, 0x10, tileSize, 0, 0, 0, 0xFF);
    func_80047024_prepared(ot, xa, kreg + 0x32, 0x10, tileSize, 0, 0xC0, 0, 0xFF);
    func_80047024_prepared(ot, xa, yB8, 0x10, tileSize, 0, 0, 0, 0xFF);
    func_80047024_prepared(ot, xa, kreg + 0x62, 0x10, tileSize, 0, 0, 0xC0, 0xFF);
    func_80047024_prepared(ot, xa, yC8, 0x10, tileSize, 0, 0, 0, 0xFF);
  }
  if (a0v > 0)
  {
    D_8007FB0C = a0v + D_8007FB0C;
    if (D_8007FB0C >= 0x1A)
    {
      D_8007FB0C = 0x19;
    }
  }
  if (a1v > 0)
  {
    D_8007FB10 = a1v + D_8007FB10;
    if (D_8007FB10 >= 0x11)
    {
      D_8007FB10 = 0x10;
    }
  }
}

void RampTeamLogoCanvas(s32 stepA, s32 stepB) {
    s32 temp;
    s32 y;

    if (stepA > 0) {
        temp = stepA + D_8009B298;
        D_8009B298 = temp;
        if (temp >= 0x101) {
            D_8009B298 = 0x100;
        }
    } else {
        temp = stepA + D_8009B298;
        D_8009B298 = temp;
        if (temp < 0x40) {
            D_8009B298 = 0x40;
        }
    }

    if (stepB > 0) {
        temp = stepB + D_8009B29C;
        D_8009B29C = temp;
        if (temp >= 0x101) {
            D_8009B29C = 0x100;
        }
    } else {
        temp = stepB + D_8009B29C;
        D_8009B29C = temp;
        if (temp < 0) {
            D_8009B29C = 0;
        }
    }

    y = D_8009B29C;
    temp = (y * 17) << 4;
    if (temp < 0) {
        temp += 0xFF;
    }
    D_8007F948 = 0x220 - (temp >> 8);
}

extern u32 g_TeamLogoCanvas[];

void ScrollTeamLogoUp(void) {
    s32 i;
    u32 *base;
    u32 saved[8];

    PlaySoundCue(1);

    base = g_TeamLogoCanvas;
    for (i = 0; i < 8; i++) {
        saved[i] = base[i];
    }
    for (i = 0; i < 0x1F8; i++) {
        base[i] = base[i + 8];
    }
    for (i = 0; i < 8; i++) {
        base[i + 0x1F8] = saved[i];
    }
}

void ScrollTeamLogoDown(void) {
    s32 i;
    u32 *newPtr;
    u32 *stackPtr;
    u32 *base;
    u32 *cursor;
    u32 saved[8];
    u32 value;

    PlaySoundCue(1);

    i = 0;
    stackPtr = saved;
    base = g_TeamLogoCanvas;
    cursor = base;
    do {
        value = cursor[0x1F8];
        cursor++;
        i++;
        *stackPtr = value;
        stackPtr++;
    } while (i < 8);

    i = 0x1F7;
    newPtr = base + 0x1F7;
    cursor = newPtr;
    do {
        value = *cursor;
        i--;
        cursor[8] = value;
        cursor--;
    } while (i >= 0);

    i = 0;
    stackPtr = base;
    cursor = saved;
    do {
        value = *cursor;
        cursor++;
        i++;
        newPtr = stackPtr;
        *newPtr = value;
        stackPtr++;
    } while (8 > i);
}

void ScrollTeamLogoLeft(void) {
    s32 row;
    u32 *savePtr;
    u32 *savePtr2;
    register u32 *rowBase asm("$8");
    s32 offset;
    s32 col;
    u32 *base;
    u32 *base2;
    u32 *addr;
    u32 *cursor;
    u32 saved[64];
    register u32 value asm("$2");
    register u32 next asm("$3");

    PlaySoundCue(1);

    row = 0;
    savePtr = saved;
    base = g_TeamLogoCanvas;
    cursor = base;
    do {
        value = *cursor;
        cursor += 8;
        row++;
        value <<= 28;
        *savePtr = value;
        savePtr++;
    } while (row < 0x40);

    row = 0;
    savePtr2 = saved;
    rowBase = base;
    offset = 0;
    do {
        col = 0;
        base2 = base;
        do {
            addr = (u32 *)(offset + (s32)base2);
            base2++;
            value = addr[0];
            next = addr[1];
            value >>= 4;
            next <<= 28;
            value |= next;
            addr[0] = value;
            col++;
        } while (col < 7);

        value = *savePtr2;
        savePtr2++;
        offset += 0x20;
        next = rowBase[7];
        row++;
        next >>= 4;
        next |= value;
        rowBase[7] = next;
        rowBase += 8;
    } while (row < 0x40);
}

void ScrollTeamLogoRight(void) {
    s32 row;
    u32 *savePtr;
    u32 *savePtr2;
    u32 *rowBase;
    s32 offset;
    s32 col;
    u32 *base;
    u32 *base2;
    u32 *cursor;
    u32 saved[64];

    PlaySoundCue(1);

    row = 0;
    savePtr = saved;
    base = g_TeamLogoCanvas;
    cursor = base;
    do {
        u32 last = cursor[7];
        cursor += 8;
        row++;
        last >>= 28;
        *savePtr = last;
        savePtr++;
    } while (row < 0x40);

    row = 0;
    rowBase = base;
    savePtr2 = saved;
    offset = 0;
    do {
        col = 7;
        base2 = base + 7;
        do {
            u32 *word = (u32 *)(offset + (s32)base2);
            u32 hi;
            u32 lo;
            base2--;
            col--;
            hi = word[0];
            lo = word[-1];
            hi <<= 4;
            lo >>= 28;
            hi |= lo;
            word[0] = hi;
        } while (col > 0);

        {
            u32 wrap = *savePtr2;
            u32 first;
            savePtr2++;
            offset += 0x20;
            first = rowBase[0];
            row++;
            first <<= 4;
            first |= wrap;
            rowBase[0] = first;
            rowBase += 8;
        }
    } while (row < 0x40);
}

void FlipTeamLogoVertical(void) {
    s32 i;
    s32 j;
    s32 mirror;
    u32 *base;

    PlaySoundCue(8);
    base = g_TeamLogoCanvas;
    i = 0;
    mirror = 0x3F;
    do {
        u8 *cursor;
        s32 leftOffset;
        s32 rightOffset;

        j = 0;
        leftOffset = i * 32;
        rightOffset = (mirror - i) << 5;
        cursor = (u8 *)base;
        do {
            u32 temp;
            u32 *left;
            u32 *right;

            left = (u32 *)(leftOffset + (s32)cursor);
            right = (u32 *)(rightOffset + (s32)cursor);
            cursor += 4;
            temp = *left;
            *left = *right;
            j++;
            *right = temp;
        } while (j < 8);
        i++;
    } while (i < 0x20);
}
