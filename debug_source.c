
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
extern u32 *g_FinalSkyOrderingTable asm("D_1F800004");
void AddPrim(void *, void *) asm("func_80064DDC");
void SetShadeTex(void *, s32) asm("func_80064EB8");
void SetPolyF4(void *) asm("func_80064F30");
void SetPolyFT4(void *) asm("func_80064F44");
void SetPolyG4(void *) asm("func_80064F58");
s32 GameSin(s32) asm("func_80068568");
s32 GameCos(s32) asm("func_80068634");
extern s16 g_SkyTileMap[][16] asm("D_8007F470");
extern s32 D_1F800068;
extern s32 g_MirrorMode asm("D_8019CACC");
extern s32 g_CourseIndex asm("D_801E428C");
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
inline static s32 DivideSigned32(s32 value)
{
  s32 adjustedValue = value;
  s32 divisionInput = value;
  s32 isNonNegative = divisionInput >= 0;
  if (!isNonNegative)
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
void func_800418D4(void)
{
  SkyRenderScratchpad *scratch = (SkyRenderScratchpad *) 0x1F800000;
  s32 panelXFixed;
  s32 panelYFixed;
  s32 columnStepX;
  s32 columnStepY;
  s32 rowStepX;
  s32 rowStepY;
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
  s32 savedCourseX1;
  s32 xWork;
  s32 savedCourseY1;
  s32 doubleRowStepY;
  s32 nextCellXFixed;
  s32 rowOffsetYFixed;
  s32 lowerPanelXFixed;
  s32 cellXFixed;
  s32 coordinateAccumulator;
  u8 *packetCursor = scratch->packetCursor;
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
    s32 xIsNonNegative;
    s32 yIsNonNegative;
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
    xIsNonNegative = unroundedX >= 0;
    if (!xIsNonNegative)
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
    yIsNonNegative = unroundedY >= 0;
    if (!yIsNonNegative)
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
    if (g_MirrorMode != D_1F800068)
    {
      panelYFixed = var_a3_184 + 0x2400;
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
    s32 temp_s7_747;
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
          quadRow = (POLY_FT4 *) packetCursor;
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
            quad->t.b0 = 0x80;
            quad->y0 = screenY0;
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
      s32 upperBandYFixed;
      s32 rightBandYFixed;
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
        quad = (POLY_FT4 *) packetCursor;
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
            if (columnStepY > 0)
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
            temp_lo_117 = g_SkyTileMap[0][(textureColumn + gridRow) & 0xF];
            tileUv = &g_SkyTileUV[temp_lo_117];
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
      temp_s7_747 = panelYFixed + columnStepY;
      screenY1 = GameRoundTerrainCoordinate(temp_s7_747);
      {
        u8 color;
        POLY_G4 *packetColorBase;
        s32 var_v0_762;
        screenY2 = GameRoundTerrainCoordinate(panelYFixed + rowStepY);
        var_v0_762 = temp_s7_747 + rowStepY;
        screenY3 = var_v0_762 / 256;
        nextPacket = packetCursor + 0x24;
        packetColorBase = (POLY_G4 *) packetCursor;
        SetPolyG4(packetCursor);
        packetColorBase->x0 = screenX0;
        packetColorBase->x1 = screenX1;
        packetColorBase->x2 = screenX2;
        packetColorBase->x3 = screenX3;
        packetColorBase->y0 = screenY0;
        packetColorBase->y1 = screenY1;
        packetColorBase->y2 = screenY2;
        packetColorBase->y3 = screenY3;
        color = g_EnvColors[2].cur.bytes.r;
        packetColorBase->r1 = color;
        packetColorBase->t.r0 = color;
        color = g_EnvColors[3].cur.bytes.r;
        packetColorBase->r3 = color;
        packetColorBase->r2 = color;
        color = g_EnvColors[2].cur.bytes.g;
        packetColorBase->g1 = color;
        packetColorBase->t.g0 = color;
        color = g_EnvColors[3].cur.bytes.g;
        packetColorBase->g3 = color;
        packetColorBase->g2 = color;
        color = g_EnvColors[2].cur.bytes.b;
        packetColorBase->b1 = color;
        packetColorBase->t.b0 = color;
        color = g_EnvColors[3].cur.bytes.b;
        packetCursor = nextPacket;
        packetColorBase->b3 = color;
        packetColorBase->b2 = color;
        AddPrim(&scratch->orderingTable[SKY_OT_NEAR], packetColorBase);
      }
      {
        u8 color;
        POLY_G4 *g4Cursor = (POLY_G4 *) packetCursor;
        u32 *orderingTableBase;
        upperBandYFixed = panelYFixed - rowStepY;
        screenY2 = upperBandYFixed / 256;
        screenX3 = (temp_fp_714 - rowStepX) / 256;
        nextPacket = packetCursor + 0x24;
        xWork = panelXFixed - rowStepX;
        screenX2 = xWork / 256;
        var_v0_941 = temp_s7_747;
        rightBandYFixed = var_v0_941 - rowStepY;
        screenY3 = rightBandYFixed / 256;
        SetPolyG4(g4Cursor);
        g4Cursor->x0 = screenX0;
        g4Cursor->x1 = screenX1;
        g4Cursor->x2 = screenX2;
        g4Cursor->x3 = screenX3;
        g4Cursor->y0 = screenY0;
        g4Cursor->y1 = screenY1;
        g4Cursor->y2 = screenY2;
        g4Cursor->y3 = screenY3;
        color = g_EnvColors[1].cur.bytes.b;
        g4Cursor->b2 = color;
        g4Cursor->b3 = color;
        color = g_EnvColors[2].cur.bytes.r;
        g4Cursor->r1 = color;
        g4Cursor->t.r0 = color;
        color = g_EnvColors[1].cur.bytes.r;
        g4Cursor->r3 = color;
        g4Cursor->r2 = color;
        color = g_EnvColors[2].cur.bytes.g;
        g4Cursor->g1 = color;
        g4Cursor->t.g0 = color;
        color = g_EnvColors[1].cur.bytes.g;
        g4Cursor->g3 = color;
        g4Cursor->g2 = color;
        screenX1 = screenX3;
        color = g_EnvColors[2].cur.bytes.b;
        do
        {
          g4Cursor->b1 = color;
        }
        while (0);
        g4Cursor->t.b0 = color;
        orderingTableBase = scratch->orderingTable;
        AddPrim(&orderingTableBase[SKY_OT_NEAR], g4Cursor++);
        packetCursor = (u8 *) g4Cursor;
      }
      {
        u8 packetColor;
        u8 lowerRed;
        POLY_G4 *g4Cursor = (POLY_G4 *) packetCursor;
        screenX0 = screenX2;
        leftXWorkFixed = rowStepX * 3;
        screenX3 = GameRoundTerrainCoordinate(temp_fp_714 - leftXWorkFixed);
        screenX2 = GameRoundTerrainCoordinate(panelXFixed - leftXWorkFixed);
        screenY0 = screenY2;
        screenY1 = screenY3;
        screenY2 = GameRoundTerrainCoordinate(upperBandYFixed - (rowStepY * 2));
        screenY3 = GameRoundTerrainCoordinate(rightBandYFixed - (rowStepY * 2));
        nextPacket = (u8 *) (g4Cursor + 1);
        SetPolyG4(g4Cursor);
        g4Cursor->x0 = screenX0;
        g4Cursor->x1 = screenX1;
        g4Cursor->x2 = screenX2;
        g4Cursor->x3 = screenX3;
        g4Cursor->y0 = screenY0;
        g4Cursor->y1 = screenY1;
        g4Cursor->y2 = screenY2;
        g4Cursor->y3 = screenY3;
        lowerRed = g_EnvColors[1].cur.bytes.r;
        g4Cursor->r3 = 0;
        g4Cursor->r1 = lowerRed;
        g4Cursor->t.r0 = lowerRed;
        g4Cursor->r2 = 0;
        {
          packetColor = g_EnvColors[1].cur.bytes.g;
          g4Cursor->g3 = 0;
          g4Cursor->g2 = 0;
          g4Cursor->t.g0 = (g4Cursor->g1 = packetColor);
          {
            u8 packetColor = g_EnvColors[1].cur.bytes.b;
            g4Cursor->t.b0 = (g4Cursor->b1 = packetColor);
            g4Cursor->b3 = 16;
            g4Cursor->b2 = 16;
          }
          AddPrim(&g_FinalSkyOrderingTable[SKY_OT_NEAR], g4Cursor);
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
        POLY_G4 *courseG4 = (POLY_G4 *) packetCursor;
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
        xWork = screenY2;
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
        color = g_EnvColors[7].cur.bytes.r;
        courseG4->r1 = color;
        courseG4->t.r0 = color;
        color = g_EnvColors[8].cur.bytes.r;
        courseG4->r3 = color;
        courseG4->r2 = color;
        color = g_EnvColors[7].cur.bytes.g;
        courseG4->g1 = color;
        courseG4->t.g0 = color;
        savedCourseY1 = screenY3;
        color = g_EnvColors[8].cur.bytes.g;
        courseG4->g3 = color;
        courseG4->g2 = color;
        color = g_EnvColors[7].cur.bytes.b;
        courseG4->b1 = color;
        courseG4->t.b0 = color;
        nextPacket = (u8 *) (courseG4 + 1);
        color = g_EnvColors[8].cur.bytes.b;
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
        POLY_G4 *courseG4 = (POLY_G4 *) packetCursor;
        rightXWorkFixed = panelXFixed;
        screenX0 = GameRoundTerrainCoordinate(rightXWorkFixed + rowStepX);
        screenX1 = GameRoundTerrainCoordinate(temp_a1_1150 + (rowStepX + (rowStepX - rowStepX)));
        screenY0 = GameRoundTerrainCoordinate(panelYFixed + rowStepY);
        screenY1 = GameRoundTerrainCoordinate(temp_a0_1170 + rowStepY);
        nextPacket = (u8 *) (courseG4 + 1);
        SetPolyG4(courseG4);
        courseG4->x0 = screenX0;
        courseG4->x1 = screenX1;
        courseG4->x2 = screenX2;
        courseG4->x3 = screenX3;
        courseG4->y0 = screenY0;
        courseG4->y1 = screenY1;
        courseG4->y2 = screenY2;
        courseG4->y3 = screenY3;
        color = g_EnvColors[5].cur.bytes.r;
        courseG4->r1 = color;
        courseG4->t.r0 = color;
        color = g_EnvColors[6].cur.bytes.r;
        courseG4->r3 = color;
        courseG4->r2 = color;
        color = g_EnvColors[5].cur.bytes.g;
        courseG4->g1 = color;
        courseG4->t.g0 = color;
        color = g_EnvColors[6].cur.bytes.g;
        courseG4->g3 = color;
        courseG4->g2 = color;
        color = g_EnvColors[5].cur.bytes.b;
        courseG4->b1 = color;
        courseG4->t.b0 = color;
        color = g_EnvColors[6].cur.bytes.b;
        courseG4->b3 = color;
        courseG4->b2 = color;
        AddPrim(&scratch->orderingTable[SKY_OT_NEAR], courseG4);
        packetCursor = nextPacket;
      }
      else
      {
        POLY_F4 *courseF4 = (POLY_F4 *) packetCursor;
        screenX0 = savedCourseX0;
        screenX1 = savedCourseX1;
        screenY0 = xWork;
        screenY1 = savedCourseY1;
        SetPolyF4(courseF4);
        courseF4->x0 = screenX0;
        courseF4->x1 = screenX1;
        courseF4->x2 = screenX2;
        courseF4->x3 = screenX3;
        courseF4->y0 = screenY0;
        courseF4->y1 = screenY1;
        courseF4->y2 = screenY2;
        courseF4->y3 = screenY3;
        courseF4->t.r0 = (u8) g_EnvColors[4].cur.bytes.r;
        courseF4->t.g0 = (u8) g_EnvColors[4].cur.bytes.g;
        {
          u8 *nextPacket;
          nextPacket = (u8 *) (courseF4 + 1);
          courseF4->t.b0 = (u8) g_EnvColors[4].cur.bytes.b;
          AddPrim(&scratch->orderingTable[SKY_OT_NEAR], courseF4);
          packetCursor = nextPacket;
        }
      }
      scratch->packetCursor = packetCursor;
    }
  }
  return;
}
