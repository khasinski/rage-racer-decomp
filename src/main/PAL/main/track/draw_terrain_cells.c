#include "common.h"
void SetRotMatrix(s32 arg0) asm("func_80069858");

extern s32 g_VisibleCellList asm("D_801E4BC8");
void BuildVisibleCells(s32 arg0, s32 arg1);
void SubmitTerrainCells(s32 arg0, s32 arg1, s32 arg2) asm("func_80027FF4");
void DrawTerrainCells(void);
void DrawTerrainCells(void) {
    BuildVisibleCells(-12288, 0x14000);
    SetRotMatrix(0x1F800028);
    SubmitTerrainCells(0x1F800000, g_VisibleCellList, 0x40);
}

void DrawTerrainCellsWide(void);
void DrawTerrainCellsWide(void) {
    BuildVisibleCells(0xFFFF6000, 0x14000);
    SetRotMatrix(0x1F800028);
    SubmitTerrainCells(0x1F800000, g_VisibleCellList, 0x40);
}

extern u32 *g_FinalSkyOrderingTable asm("D_1F800004");
void AddPrim(void *, void *);
void SetShadeTex(void *, s32) asm("func_80064EB8");
void SetPolyF4(void *);
void SetPolyFT4(void *);
void SetPolyG4(void *)asm("func_80064F58");
s32 GameSin(s32) asm("func_80068568");
s32 GameCos(s32) asm("func_80068634");
extern s16 g_SkyTileMap[][16] asm("D_8007F470");
extern s32 g_MirrorMode asm("D_8019CACC");
extern s32 g_CourseIndex;
extern s32 g_SkyRowBase asm("D_801E4D84");
typedef struct
{
  u32 tag;
  u8 r0;
  u8 g0;
  u8 b0;
  u8 code;
} P_TAG;
typedef struct
{
  P_TAG t;
  s16 x0;
  s16 y0;
  s16 x1;
  s16 y1;
  s16 x2;
  s16 y2;
  s16 x3;
  s16 y3;
} POLY_F4;
typedef union
{
  struct
  {
    u8 u;
    u8 v;
  } bytes;
  u16 packed;
} SkyUV;
typedef struct
{
  SkyUV corner[4];
} SkyTileUV;
typedef struct
{
  u8 *packetCursor;
  u32 *orderingTable;
  s32 cameraX;
  s32 cameraY;
  s32 cameraZ;
  s32 pad14;
  s32 pitch;
  s32 yaw;
  s32 roll;
  s32 pad24[17];
  s32 mirrorFlag;
} SkyRenderScratchpad;
typedef struct
{
  s16 xMinTop;
  s16 xMaxTop;
  s16 xMinBottom;
  s16 xMaxBottom;
  s16 yEdge0;
  s16 yEdge1;
  s16 yEdge2;
  s16 yEdge3;
} SkyClipBounds;
typedef struct
{
  P_TAG t;
  s16 x0;
  s16 y0;
  SkyUV uv0;
  u16 clut;
  s16 x1;
  s16 y1;
  SkyUV uv1;
  u16 tpage;
  s16 x2;
  s16 y2;
  SkyUV uv2;
  u16 pad1E;
  s16 x3;
  s16 y3;
  SkyUV uv3;
  u16 pad26;
} POLY_FT4;
typedef struct
{
  P_TAG t;
  s16 x0;
  s16 y0;
  u8 r1;
  u8 g1;
  u8 b1;
  u8 pad0;
  s16 x1;
  s16 y1;
  u8 r2;
  u8 g2;
  u8 b2;
  u8 pad1;
  s16 x2;
  s16 y2;
  u8 r3;
  u8 g3;
  u8 b3;
  u8 pad2;
  s16 x3;
  s16 y3;
} POLY_G4;
extern SkyTileUV g_SkyTileUV[] asm("D_8007F510");
typedef struct
{
  u8 r;
  u8 g;
  u8 b;
  u8 cd;
} SkyCVec;
typedef union
{
  u32 rgb;
  struct
  {
    u8 r;
    u8 g;
    u8 b;
    u8 unused;
  } bytes;
} GameEnvColor;
typedef struct
{
  GameEnvColor cur;
  GameEnvColor from;
  GameEnvColor to;
} GameEnvColorSlot;
extern GameEnvColorSlot g_EnvColors[] asm("D_801E3FB6");
extern GameEnvColor g_EnvColor1Red;
extern GameEnvColor g_EnvColor2Red;
extern GameEnvColor g_EnvColor4Red;
extern GameEnvColor g_EnvColor5Red;
extern GameEnvColor g_EnvColor7Red;
extern u8 g_EnvColor1G;
extern u8 g_EnvColor1B;
extern u8 g_EnvColor2G;
extern u8 g_EnvColor2B;
extern u8 g_EnvColor3G;
extern u8 g_EnvColor3B;
extern u8 g_EnvColor4G;
extern u8 g_EnvColor4B;
extern u8 g_EnvColor5G;
extern u8 g_EnvColor5B;
extern u8 g_EnvColor6G;
extern u8 g_EnvColor6B;
extern u8 g_EnvColor7G;
extern u8 g_EnvColor7B;
extern u8 g_EnvColor8G;
extern u8 g_EnvColor8B;
extern u8 g_EnvColor3R;
extern u8 g_EnvColor6R;
extern u8 g_EnvColor8R;
inline static s32 DivideSigned32(s32 value)
{
  s32 adjustedValue = value;
  s32 divisionInput = value;
  if (divisionInput < 0)
    adjustedValue += 31;
  return adjustedValue >> 5;
}

inline static s32 GameRoundTerrainCoordinate(s32 value)
{
  return value / 256;
}

inline static s32 GameRoundTerrainCoordinate11(s32 value)
{
  return value / 2048;
}

enum SkyOrderingTableIndex
{
  SKY_OT_FAR = 702,
  SKY_OT_NEAR = 703
};
register u8 *stackPointer asm("$29");
void GameDrawSkyBackground(void) asm("func_800418D4");
void GameDrawSkyBackground(void)
{
  SkyRenderScratchpad *scratch = (SkyRenderScratchpad *)0x1F800000;
  s32 panelXFixed;
  s32 panelYFixed;
  s32 columnStepX;
  s32 columnStepY;
  s32 rowStepX;
  s32 rowStepY;
  s32 new_var;
  s32 savedSinRoll;
  s32 savedCosRoll;
  s32 textureColumn;
  s32 bandOriginXFixed;
  s32 bandOriginYFixed;
  s32 horizonTopY;
  s32 screenX0;
  s32 screenX1;
  s32 screenX2;
  s32 screenX3;
  s32 savedCourseX0;
  u8 *packetCursor = scratch->packetCursor;
  s32 savedCourseX1;
  s32 new_var3;
  s32 xWork_late;
  s32 adjW;
  s32 courseSaveY1;
  u8 *new_var2;
  s32 doubleRowStepY;
  s32 nextCellXFixed;
  s32 rowOffsetYFixed;
  s32 xWork;
  s32 upperBandYFixed;
  s32 savedCourseY1;
  s32 lowerPanelXFixed;
  s32 cellXFixed;
  s32 coordinateAccumulator;
  {
    s32 cameraY;
    s32 var_a3_184;
    s32 temp_lo_117;
    s32 sinRoll;
    s32 nearVerticalFixed;
    s32 horizontalFixed;
    s32 farVerticalFixed;
    s32 negativeSinRoll;
    s32 cosRoll;
    s32 angleWork;
    s32 leftViewAngle;
    s32 rollAngle;
    s32 rotatedX;
    s32 rotatedY;
    s32 pitchAngle;
    s32 yawAngle;
    s32 unroundedX;
    s32 unroundedY;
    if (scratch->mirrorFlag != g_MirrorMode)
    {
      pitchAngle = -scratch->pitch;
    }
    else
    {
      pitchAngle = scratch->pitch;
    }
    coordinateAccumulator = pitchAngle & 0xFFF;
    leftViewAngle = coordinateAccumulator;
    if (leftViewAngle >= 0x800)
    {
      leftViewAngle -= 0x1000;
    }
    if (coordinateAccumulator >= 0x800)
    {
      coordinateAccumulator -= 0x1000;
    }
    cameraY = scratch->cameraY;
    {
      s32 leftPlusTwo;
      s32 rightPlusTwo;
      angleWork = cameraY - 6000;
      leftPlusTwo = leftViewAngle + 2;
      cellXFixed = DivideSigned32(angleWork);
      leftViewAngle = leftPlusTwo + cellXFixed;
      rightPlusTwo = coordinateAccumulator + 2;
      coordinateAccumulator = rightPlusTwo + cellXFixed;
      yawAngle = scratch->mirrorFlag;
    }
    angleWork = scratch->yaw;
    if (yawAngle != 0)
    {
      angleWork = -angleWork;
      yawAngle = angleWork + 0x200;
    }
    else
    {
      yawAngle = angleWork + 0x200;
    }
    angleWork = yawAngle & 0xFFF;
    leftViewAngle /= 2;
    coordinateAccumulator = (coordinateAccumulator / 2) + 0x50;
    textureColumn = angleWork >> 7;
    horizontalFixed = (s32) (((u32) ((-0x100) - ((angleWork >> 1) & 0x3F))) << 8);
    nearVerticalFixed = (s32) (((u32) ((-0x80) - leftViewAngle)) << 8);
    rollAngle = scratch->roll;
    farVerticalFixed = (s32) (((u32) ((-0x80) - coordinateAccumulator)) << 8);
    if (g_MirrorMode == 0)
    {
      rollAngle = -rollAngle;
    }
    sinRoll = GameSin(rollAngle);
    cosRoll = GameCos(rollAngle);
    coordinateAccumulator = cosRoll * horizontalFixed;
    rotatedX = coordinateAccumulator + (sinRoll * nearVerticalFixed);
    unroundedX = rotatedX;
    if (unroundedX < 0)
    {
      rotatedX += 0xFFF;
    }
    negativeSinRoll = -sinRoll;
    temp_lo_117 = negativeSinRoll * horizontalFixed;
    rotatedX >>= 0xC;
    leftViewAngle = 0xA000;
    panelXFixed = rotatedX + leftViewAngle;
    rotatedY = temp_lo_117 + (cosRoll * nearVerticalFixed);
    unroundedY = rotatedY;
    if (unroundedY < 0)
    {
      rotatedY += 0xFFF;
    }
    var_a3_184 = rotatedY >> 0xC;
    coordinateAccumulator += sinRoll * farVerticalFixed;
    panelYFixed = var_a3_184 + 0x7800;
    if (coordinateAccumulator < 0)
    {
      coordinateAccumulator += 0xFFF;
    }
    rotatedY = cosRoll * farVerticalFixed;
    temp_lo_117 += rotatedY;
    rotatedY = coordinateAccumulator >> 0xC;
    lowerPanelXFixed = rotatedY + 0xA000;
    if (temp_lo_117 < 0)
    {
      temp_lo_117 += 0xFFF;
    }
    leftViewAngle = temp_lo_117 >> 0xC;
    coordinateAccumulator = leftViewAngle + 0x7800;
    if (g_MirrorMode != *(s32 *)0x1F800068)
    {
      panelYFixed = 0x2400;
      panelYFixed = var_a3_184 + panelYFixed;
      coordinateAccumulator = leftViewAngle + 0x2400;
    }
    columnStepX = cosRoll * 4;
    columnStepY = negativeSinRoll * 4;
    savedSinRoll = sinRoll;
    savedCosRoll = cosRoll;
    bandOriginXFixed = panelXFixed;
    bandOriginYFixed = panelYFixed;
    rowStepX = sinRoll * 8;
    rowStepY = cosRoll * 8;
  }
  {
    s32 screenY3;
    s32 screenY2;
    s32 screenY1;
    s32 screenY0;
    s32 gridRow;
    s32 leftXWorkFixed;
    s32 rightXWorkFixed;
    SkyTileUV *tileUv;
    u8 *nextPacket;
    if (g_SkyRowBase != 0)
    {
      {
        s32 temp_s7_256;
        s32 var_a3_184;
        POLY_FT4 *quad;
        POLY_FT4 *quadRow;
        s16 tileIndex;
        s32 temp_v1_257;
        s32 temp_v1_223;
        s32 temp_a1_208;
        s32 temp_a0_248;
        s32 var_fp_181;
        s32 spF0 = 0;
        s32 spF8 = 0;
        gridRow = 0;
        do
        {
          var_fp_181 = 0;
          quadRow = (POLY_FT4 *)packetCursor;
          doubleRowStepY = spF8;
          rowOffsetYFixed = spF0;
          var_a3_184 = panelYFixed;
          cellXFixed = panelXFixed;
          do
          {
            quad = quadRow + var_fp_181;
            tileIndex = g_SkyTileMap[(gridRow % 2) + g_SkyRowBase][(textureColumn + var_fp_181) & 0xF];
            tileUv = &g_SkyTileUV[tileIndex];
            temp_a1_208 = cellXFixed - doubleRowStepY;
            screenX0 = GameRoundTerrainCoordinate(temp_a1_208);
            nextCellXFixed = cellXFixed + columnStepX;
            temp_v1_223 = nextCellXFixed - doubleRowStepY;
            screenX1 = GameRoundTerrainCoordinate(temp_v1_223);
            screenX2 = GameRoundTerrainCoordinate(temp_a1_208 + rowStepX);
            screenX3 = GameRoundTerrainCoordinate(temp_v1_223 + rowStepX);
            temp_a0_248 = var_a3_184 - rowOffsetYFixed;
            screenY0 = GameRoundTerrainCoordinate(temp_a0_248);
            temp_s7_256 = var_a3_184 + columnStepY;
            temp_v1_257 = temp_s7_256 - rowOffsetYFixed;
            screenY1 = GameRoundTerrainCoordinate(temp_v1_257);
            screenY2 = GameRoundTerrainCoordinate(temp_a0_248 + rowStepY);
            screenY3 = GameRoundTerrainCoordinate(temp_v1_257 + rowStepY);
            SetPolyFT4(packetCursor);
            SetShadeTex(packetCursor, 0);
            quad->tpage = 0x18;
            quad->uv0.packed = tileUv->corner[0].packed;
            packetCursor += 0x28;
            quad->uv1.packed = tileUv->corner[1].packed;
            quad->uv2.packed = tileUv->corner[2].packed;
            quad->uv3.packed = tileUv->corner[3].packed;
            quad->x0 = screenX0;
            quad->x1 = screenX1;
            quad->x2 = screenX2;
            quad->x3 = screenX3;
            quad->t.r0 = 0x80;
            quad->t.g0 = 0x80;
            new_var = screenY0;
            quad->t.b0 = 0x80;
            quad->y0 = new_var;
            quad->y1 = screenY1;
            quad->y2 = screenY2;
            quad->y3 = screenY3;
            quad->clut = 0x798E;
            AddPrim(&scratch->orderingTable[SKY_OT_NEAR], quad);
            var_fp_181 += 1;
            cellXFixed = nextCellXFixed;
            var_a3_184 = temp_s7_256;
          }
          while (var_fp_181 < 8);
          gridRow += 1;
          spF0 += rowStepY;
          spF8 += rowStepX;
        }
        while (gridRow < 4);
      }
      columnStepX *= 8;
      columnStepY *= 8;
    }
    else
    {
      SkyClipBounds clip;
      s32 temp_lo_117;
      POLY_FT4 *quad;
      s32 temp_fp_714;
      s32 temp_v0_431;
      s32 temp_v1_408;
      s32 temp_v1_475;
      s32 var_fp_360;
      s32 var_s7_357;
      s32 upperBandXFixed;
      s32 var_v0_941;
      s32 var_v0_947;
      panelYFixed = coordinateAccumulator;
      {
        panelXFixed = lowerPanelXFixed;
        horizonTopY = GameRoundTerrainCoordinate(panelYFixed);
        var_fp_360 = GameRoundTerrainCoordinate(panelYFixed + rowStepY);
        var_s7_357 = 0xF0;
        clip.xMinTop = (clip.xMinBottom = 0x140);
        clip.xMaxBottom = 0;
        clip.xMaxTop = 0;
        clip.yEdge0 = (columnStepY > 0) ? (0xF0) : (-0xF0);
        clip.yEdge1 = (columnStepY > 0) ? (-0xF0) : (0xF0);
        clip.yEdge2 = (columnStepY > 0) ? (0xF0) : (-0xF0);
        clip.yEdge3 = (columnStepY > 0) ? (-0xF0) : (0xF0);
        gridRow = 0;
        quad = (POLY_FT4 *)packetCursor;
        do
        {
          screenX0 = GameRoundTerrainCoordinate(panelXFixed);
          temp_v1_408 = panelXFixed + columnStepX;
          screenX1 = GameRoundTerrainCoordinate(temp_v1_408);
          screenX2 = GameRoundTerrainCoordinate(panelXFixed + rowStepX);
          temp_v0_431 = GameRoundTerrainCoordinate(temp_v1_408 + rowStepX);
          screenX3 = temp_v0_431;
          if (((((screenX0 >= 0) || (screenX1 >= 0)) || (screenX2 >= 0)) || (temp_v0_431 >= 0)) && ((((screenX0 < 0x140) || (screenX1 < 0x140)) || (screenX2 < 0x140)) || (screenX3 < 0x140)))
          {
            screenY0 = GameRoundTerrainCoordinate(panelYFixed);
            temp_v1_475 = panelYFixed + columnStepY;
            screenY1 = GameRoundTerrainCoordinate(temp_v1_475);
            screenY2 = GameRoundTerrainCoordinate(panelYFixed + rowStepY);
            screenY3 = GameRoundTerrainCoordinate(temp_v1_475 + rowStepY);
            if (horizonTopY < screenY0)
            {
              horizonTopY = screenY0;
            }
            if (horizonTopY < screenY1)
            {
              horizonTopY = screenY1;
            }
            if (screenY2 < var_fp_360)
            {
              var_fp_360 = screenY2;
            }
            if (screenY3 < var_fp_360)
            {
              var_fp_360 = screenY3;
            }
            if (screenY2 < screenY3)
            {
              if (screenY2 < var_s7_357)
              {
                var_s7_357 = screenY2;
              }
            }
            else
              if (screenY3 < var_s7_357)
            {
              var_s7_357 = screenY3;
            }
            if (screenX0 < clip.xMinTop)
            {
              clip.xMinTop = screenX0;
            }
            if (screenX2 < clip.xMinBottom)
            {
              clip.xMinBottom = screenX2;
            }
            if (clip.xMaxTop < screenX1)
            {
              clip.xMaxTop = screenX1;
            }
            if (clip.xMaxBottom < screenX3)
            {
              clip.xMaxBottom = screenX3;
            }
            if (0 < columnStepY)
            {
              if (screenY0 < clip.yEdge0)
              {
                clip.yEdge0 = screenY0;
              }
              if (clip.yEdge1 < screenY1)
              {
                clip.yEdge1 = screenY1;
              }
              if (screenY2 < clip.yEdge2)
              {
                clip.yEdge2 = screenY2;
              }
              if (clip.yEdge3 < screenY3)
              {
                clip.yEdge3 = screenY3;
              }
            }
            else
            {
              if (clip.yEdge0 < screenY0)
              {
                clip.yEdge0 = screenY0;
              }
              if (screenY1 < clip.yEdge1)
              {
                clip.yEdge1 = screenY1;
              }
              if (clip.yEdge2 < screenY2)
              {
                clip.yEdge2 = screenY2;
              }
              if (screenY3 < clip.yEdge3)
              {
                clip.yEdge3 = screenY3;
              }
            }
            {
              s32 lateTileIndex = g_SkyTileMap[0][(textureColumn + gridRow) & 0xF];
              tileUv = &g_SkyTileUV[lateTileIndex];
            }
            SetPolyFT4(packetCursor);
            SetShadeTex(packetCursor, 0);
            quad->tpage = 0x18;
            quad->uv0.packed = tileUv->corner[0].packed;
            packetCursor += 0x28;
            quad->uv1.packed = tileUv->corner[1].packed;
            quad->uv2.packed = tileUv->corner[2].packed;
            quad->uv3.packed = tileUv->corner[3].packed;
            quad->x0 = screenX0;
            quad->x1 = screenX1;
            quad->x2 = screenX2;
            quad->x3 = screenX3;
            quad->t.r0 = 0x80;
            quad->t.g0 = 0x80;
            quad->t.b0 = 0x80;
            quad->y0 = screenY0;
            quad->y1 = screenY1;
            quad->y2 = screenY2;
            quad->y3 = screenY3;
            quad->clut = 0x798E;
            AddPrim(&scratch->orderingTable[SKY_OT_NEAR], quad++);
          }
          gridRow += 1;
          panelXFixed += columnStepX;
          panelYFixed += columnStepY;
        }
        while (gridRow < 8);
      }
      panelXFixed = bandOriginXFixed;
      panelYFixed = bandOriginYFixed;
      columnStepX *= 8;
      columnStepY *= 8;
      screenX0 = GameRoundTerrainCoordinate(panelXFixed);
      temp_fp_714 = panelXFixed + columnStepX;
      screenX1 = GameRoundTerrainCoordinate(temp_fp_714);
      screenX2 = GameRoundTerrainCoordinate(panelXFixed + rowStepX);
      screenX3 = GameRoundTerrainCoordinate(temp_fp_714 + rowStepX);
      screenY0 = GameRoundTerrainCoordinate(panelYFixed);
      var_s7_357 = panelYFixed + columnStepY;
      screenY1 = GameRoundTerrainCoordinate(var_s7_357);
      {
        u8 color;
        s32 var_v0_762;
        screenY2 = GameRoundTerrainCoordinate(panelYFixed + rowStepY);
        var_v0_762 = var_s7_357 + rowStepY;
        screenY3 = var_v0_762 / 256;
        nextPacket = packetCursor + 0x24;
        SetPolyG4(packetCursor);
        ((POLY_G4 *)packetCursor)->x0 = screenX0;
        ((POLY_G4 *)packetCursor)->x1 = screenX1;
        ((POLY_G4 *)packetCursor)->x2 = screenX2;
        ((POLY_G4 *)packetCursor)->x3 = screenX3;
        ((POLY_G4 *)packetCursor)->y0 = screenY0;
        ((POLY_G4 *)packetCursor)->y1 = screenY1;
        ((POLY_G4 *)packetCursor)->y2 = screenY2;
        ((POLY_G4 *)packetCursor)->y3 = screenY3;
        color = g_EnvColor2Red.bytes.r;
        ((POLY_G4 *)packetCursor)->r1 = color;
        ((POLY_G4 *)packetCursor)->t.r0 = color;
        color = g_EnvColor3R;
        ((POLY_G4 *)packetCursor)->r3 = color;
        ((POLY_G4 *)packetCursor)->r2 = color;
        color = g_EnvColor2G;
        ((POLY_G4 *)packetCursor)->g1 = color;
        ((POLY_G4 *)packetCursor)->t.g0 = color;
        color = g_EnvColor3G;
        ((POLY_G4 *)packetCursor)->g3 = color;
        ((POLY_G4 *)packetCursor)->g2 = color;
        color = g_EnvColor2B;
        ((POLY_G4 *)packetCursor)->b1 = color;
        ((POLY_G4 *)packetCursor)->t.b0 = color;
        color = g_EnvColor3B;
        ((POLY_G4 *)packetCursor)->b3 = color;
        ((POLY_G4 *)packetCursor)->b2 = color;
        {
          POLY_G4 *firstG4 = (POLY_G4 *)packetCursor;
          packetCursor = nextPacket;
          asm("" : : "r"(packetCursor));
          AddPrim(&scratch->orderingTable[SKY_OT_NEAR], firstG4);
        }
      }
      {
        u8 color;
        POLY_G4 *g4Cursor = (POLY_G4 *)packetCursor;
        u32 *orderingTableBase;
        register POLY_G4 *primCursor asm("$5");
        xWork = panelXFixed - rowStepX;
        adjW = xWork;
        if (xWork < 0)
        {
          adjW = xWork + 0xFF;
        }
        screenX2 = adjW >> 8;
        upperBandXFixed = temp_fp_714 - rowStepX;
        screenX3 = upperBandXFixed / 256;
        asm("" : : "r"(temp_fp_714) : "memory");
        upperBandYFixed = panelYFixed - rowStepY;
        adjW = upperBandYFixed;
        if (upperBandYFixed < 0)
        {
          adjW = upperBandYFixed + 0xFF;
        }
        screenY2 = adjW >> 8;
        new_var3 = var_s7_357;
        var_v0_941 = new_var3;
        savedCourseY1 = var_v0_941 - rowStepY;
        adjW = savedCourseY1;
        if (savedCourseY1 < 0)
        {
          adjW = savedCourseY1 + 0xFF;
        }
        screenY3 = adjW >> 8;
        nextPacket = packetCursor + 0x24;
        SetPolyG4(g4Cursor);
        g4Cursor->x0 = screenX0;
        g4Cursor->x1 = screenX1;
        g4Cursor->x2 = screenX2;
        g4Cursor->x3 = screenX3;
        g4Cursor->y0 = screenY0;
        g4Cursor->y1 = screenY1;
        g4Cursor->y2 = screenY2;
        g4Cursor->y3 = screenY3;
        color = g_EnvColor2Red.bytes.r;
        g4Cursor->r1 = color;
        g4Cursor->t.r0 = color;
        color = g_EnvColor1Red.bytes.r;
        g4Cursor->r3 = color;
        g4Cursor->r2 = color;
        color = g_EnvColor2G;
        g4Cursor->g1 = color;
        g4Cursor->t.g0 = color;
        color = g_EnvColor1G;
        g4Cursor->g3 = color;
        g4Cursor->g2 = color;
        color = g_EnvColor2B;
        do
        {
          g4Cursor->b1 = color;
          g4Cursor->t.b0 = color;
        }
        while (0);
        color = g_EnvColor1B;
        primCursor = g4Cursor;
        primCursor->b3 = color;
        primCursor->b2 = color;
        orderingTableBase = scratch->orderingTable;
        AddPrim(&orderingTableBase[SKY_OT_NEAR], g4Cursor++);
        nextPacket = (u8 *)g4Cursor;
        asm("" : : "r"(upperBandXFixed));
      }
      {
        u8 packetColor;
        POLY_G4 *g4Cursor;
        s32 reloadD8;
        u16 geomX2;
        u16 geomValueX2;
        s32 x3Raw;
        s32 doubleStepX;
        asm volatile("" : : "r"(xWork));
        asm volatile("" : "=r"(screenX0) : "0"(*(s32 *)(stackPointer + 0x90)));
        doubleStepX = rowStepX * 2;
        screenX1 = screenX3;
        leftXWorkFixed = doubleStepX + rowStepX;
        rightXWorkFixed = leftXWorkFixed;
        screenX2 = GameRoundTerrainCoordinate(panelXFixed - rightXWorkFixed);

        packetCursor = nextPacket;
        g4Cursor = (POLY_G4 *)packetCursor;
        x3Raw = temp_fp_714 - leftXWorkFixed;
        if (x3Raw < 0)
        {
          x3Raw += 0xFF;
        }
        asm("" : : "r"(upperBandYFixed));
        screenX3 = x3Raw >> 8;
        asm("");
        asm("" : : "r"(savedCourseY1));
        screenY0 = screenY2;
        temp_lo_117 = rowStepY * 3;
        screenY1 = screenY3;
        screenY2 = GameRoundTerrainCoordinate(panelYFixed - temp_lo_117);
        screenY3 = GameRoundTerrainCoordinate(new_var3 - temp_lo_117);
        nextPacket = (u8 *)(g4Cursor + 1);
        SetPolyG4(g4Cursor);
        g4Cursor->x0 = screenX0;
        g4Cursor->x1 = screenX1;
        asm volatile("" : "=r"(geomValueX2) : "0"((u16) screenX2) : "$2");
        g4Cursor->x2 = geomValueX2;
        g4Cursor->x3 = screenX3;
        g4Cursor->y0 = screenY0;
        g4Cursor->y1 = screenY1;
        g4Cursor->y2 = screenY2;
        g4Cursor->y3 = screenY3;
        g4Cursor->t.r0 = (g4Cursor->r1 = g_EnvColor1Red.bytes.r);
        g4Cursor->r2 = (g4Cursor->r3 = 0);
        {
          packetColor = g_EnvColor1G;
          g4Cursor->g2 = (g4Cursor->g3 = 0);
          g4Cursor->t.g0 = (g4Cursor->g1 = packetColor);
          {
            u8 packetColor = g_EnvColor1B;
            g4Cursor->t.b0 = (g4Cursor->b1 = packetColor);
            g4Cursor->b2 = (g4Cursor->b3 = 16);
          }
          AddPrim(&scratch->orderingTable[SKY_OT_NEAR], g4Cursor);
        }
        packetCursor = nextPacket;
      }
    }
    {
      s32 temp_a0_1048;
      s32 temp_a0_1170;
      s32 temp_a1_1150;
      s32 temp_v1_1063;
      s32 temp_v1_1140;
      s32 temp_v1_1161;
      s32 var_v0_1007;
      textureColumn = rowStepX * 4;
      if (g_CourseIndex != 2)
      {
        u8 color;
        POLY_G4 *courseG4 = (POLY_G4 *)packetCursor;
        leftXWorkFixed = (panelXFixed + rowStepX) * 8;
        var_v0_1007 = leftXWorkFixed - savedSinRoll;
        screenX0 = var_v0_1007 / 2048;
        rightXWorkFixed = ((panelXFixed + columnStepX) + rowStepX) * 8;
        screenX1 = GameRoundTerrainCoordinate11(rightXWorkFixed - savedSinRoll);
        screenX2 = GameRoundTerrainCoordinate11(leftXWorkFixed + savedSinRoll);
        savedCourseX0 = screenX2;
        screenX3 = GameRoundTerrainCoordinate11(rightXWorkFixed + savedSinRoll);
        temp_a0_1048 = (panelYFixed + rowStepY) * 8;
        savedCourseX1 = screenX3;
        screenY0 = GameRoundTerrainCoordinate11(temp_a0_1048 - savedCosRoll);
        temp_v1_1063 = ((panelYFixed + columnStepY) + rowStepY) * 8;
        screenY1 = GameRoundTerrainCoordinate11(temp_v1_1063 - savedCosRoll);
        screenY2 = GameRoundTerrainCoordinate11(temp_a0_1048 + savedCosRoll);
        xWork_late = screenY2;
        screenY3 = GameRoundTerrainCoordinate11(temp_v1_1063 + savedCosRoll);
        SetPolyG4(courseG4);
        courseG4->x0 = screenX0;
        courseG4->x1 = screenX1;
        courseG4->x2 = screenX2;
        courseG4->x3 = screenX3;
        courseG4->y0 = screenY0;
        courseG4->y1 = screenY1;
        courseG4->y2 = screenY2;
        courseG4->y3 = screenY3;
        color = g_EnvColor7Red.bytes.r;
        courseG4->r1 = color;
        courseG4->t.r0 = color;
        color = g_EnvColor8R;
        courseG4->r3 = color;
        courseG4->r2 = color;
        color = g_EnvColor7G;
        courseG4->g1 = color;
        courseG4->t.g0 = color;
        courseSaveY1 = screenY3;
        color = g_EnvColor8G;
        courseG4->g3 = color;
        courseG4->g2 = color;
        color = g_EnvColor7B;
        courseG4->b1 = color;
        courseG4->t.b0 = color;
        nextPacket = (u8 *)(courseG4 + 1);
        color = g_EnvColor8B;
        courseG4->b3 = color;
        courseG4->b2 = color;
        AddPrim(&scratch->orderingTable[SKY_OT_FAR], courseG4);
        packetCursor = nextPacket;
      }
      temp_v1_1140 = rowStepX * 3;
      screenX2 = GameRoundTerrainCoordinate(panelXFixed + temp_v1_1140);
      temp_a1_1150 = panelXFixed + columnStepX;
      screenX3 = GameRoundTerrainCoordinate(temp_a1_1150 + temp_v1_1140);
      temp_v1_1161 = rowStepY * 3;
      screenY2 = GameRoundTerrainCoordinate(panelYFixed + temp_v1_1161);
      temp_a0_1170 = panelYFixed + columnStepY;
      screenY3 = GameRoundTerrainCoordinate(temp_a0_1170 + temp_v1_1161);
      if (g_CourseIndex == 2)
      {
        u8 color;
        POLY_G4 *courseG4 = (POLY_G4 *)packetCursor;
        rightXWorkFixed = panelXFixed;
        screenX0 = GameRoundTerrainCoordinate(rightXWorkFixed + rowStepX);
        screenX1 = GameRoundTerrainCoordinate(temp_a1_1150 + (rowStepX + (rowStepX - rowStepX)));
        screenY0 = GameRoundTerrainCoordinate(panelYFixed + rowStepY);
        screenY1 = GameRoundTerrainCoordinate(temp_a0_1170 + rowStepY);
        nextPacket = (u8 *)(courseG4 + 1);
        SetPolyG4(courseG4);
        courseG4->x0 = screenX0;
        courseG4->x1 = screenX1;
        courseG4->x2 = screenX2;
        courseG4->x3 = screenX3;
        courseG4->y0 = screenY0;
        courseG4->y1 = screenY1;
        courseG4->y2 = screenY2;
        courseG4->y3 = screenY3;
        color = g_EnvColor5Red.bytes.r;
        courseG4->r1 = color;
        courseG4->t.r0 = color;
        color = g_EnvColor6R;
        courseG4->r3 = color;
        courseG4->r2 = color;
        color = g_EnvColor5G;
        courseG4->g1 = color;
        courseG4->t.g0 = color;
        color = g_EnvColor6G;
        courseG4->g3 = color;
        courseG4->g2 = color;
        color = g_EnvColor5B;
        courseG4->b1 = color;
        courseG4->t.b0 = color;
        color = g_EnvColor6B;
        courseG4->b3 = color;
        courseG4->b2 = color;
        AddPrim(&scratch->orderingTable[SKY_OT_NEAR], courseG4);
        packetCursor = nextPacket;
      }
      else
      {
        POLY_F4 *courseF4 = (POLY_F4 *)packetCursor;
        screenX0 = savedCourseX0;
        screenX1 = savedCourseX1;
        screenY0 = xWork_late;
        screenY1 = courseSaveY1;
        SetPolyF4(courseF4);
        courseF4->x0 = screenX0;
        courseF4->x1 = screenX1;
        courseF4->x2 = screenX2;
        courseF4->x3 = screenX3;
        courseF4->y0 = screenY0;
        courseF4->y1 = screenY1;
        courseF4->y2 = screenY2;
        courseF4->y3 = screenY3;
        courseF4->t.r0 = (u8) g_EnvColor4Red.bytes.r;
        courseF4->t.g0 = (u8) g_EnvColor4G;
        {
          u8 *nextPacket;
          nextPacket = (u8 *)(courseF4 + 1);
          courseF4->t.b0 = (u8) g_EnvColor4B;
          AddPrim(&scratch->orderingTable[SKY_OT_NEAR], courseF4);
          packetCursor = nextPacket;
        }
      }
      scratch->packetCursor = packetCursor;
    }
  }
  return;
}

extern s32 g_CdTrackPending asm("D_8007F600");
extern s32 g_CdCommandPending asm("D_8007F604");
extern s32 g_CdTrackStep asm("D_8007F608");
extern s32 g_CdCommandStep asm("D_8007F60C");
