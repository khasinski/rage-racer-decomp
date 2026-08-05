#include "common.h"
#include "psyq/gpu.h"

typedef struct TextSprt8 {
    P_TAG tag;
    s16 x0;
    s16 y0;
    u8 u0;
    u8 v0;
    u16 clut;
} TextSprt8;

extern u8 *g_DrawBuffer;
extern u8 g_Font8x8Cells[];
extern u8 g_DrawModeEnv[];
extern u8 g_PropFontU[];
extern u8 g_PropFontV[];
extern u8 g_WordFontU[];
extern u8 g_WordFontV[];
extern u8 g_WordFontWidth[];
extern u8 g_WordFontAdvance[];
extern u8 g_HighFontU[];
extern u8 g_HighFontV[];
extern u8 g_HighFontWidth[];
extern u8 g_HighFontYOffset[];

/*
 * Keep the first font base opaque after materialising it. This empty constraint
 * makes gcc emit that base before the sprite cursor while still rematerialising
 * g_Font8x8Cells + 1 in the loop, as retail does.
 */
#define INIT_TEXT_FONT(font) \
    asm("" : "=r"(font) : "0"(g_Font8x8Cells))

void DrawText8x8(
    s32 x,
    s32 y,
    u8 *str,
    s32 clutIndex);
void GameDrawText8x8Shaded(
    s32 x,
    s32 y,
    u8 *str,
    s32 clutIndex,
    u8 intensity);
void DrawText8x8Trans(
    s32 x,
    s32 y,
    u8 *str,
    s32 clutIndex);
void SetDrawModeWide8x8(
    void *packet,
    s32 dither,
    s32 drawToDisplay,
    s32 tpage,
    void *textureWindow) asm("func_800666F4");

void DrawText8x8(s32 x, s32 y, u8 *str, s32 clutIndex) {
    u8 **scratch = (u8 **)0x1F800000;
    u8 *packet;

    packet = *scratch;
    if (*str != 0) {
        volatile TextSprt8 *sprt;
        u8 *font;

        INIT_TEXT_FONT(font);
        sprt = (TextSprt8 *)packet;
        do {
            s32 cell = *str - 0x20;

            str++;
            if (cell != 0) {
                s32 index;
                u8 *fontUCell;
                u8 *fontV;
                s32 u;
                s32 v;

                index = cell * 2;
                fontUCell = (u8 *)(index + (s32)font);
                fontV = &g_Font8x8Cells[1];
                u = *fontUCell * 8;
                v = *(u8 *)(index + (s32)fontV) * 8;

                SetSprt8(packet);
                SetShadeTex(packet, 1);
                sprt->x0 = x;
                sprt->y0 = y;
                sprt->u0 = u;
                sprt->v0 = v;
                sprt->clut = clutIndex;
                AddPrim(g_DrawBuffer + 0xCC, (void *)sprt);
                sprt++;
                packet += sizeof(TextSprt8);
            }
            x += 8;
        } while (*str != 0);
    }
    SetDrawMode((DrawPacket *)packet, 0, 1, 9, g_DrawModeEnv);
    AddPrim(g_DrawBuffer + 0xCC, packet);
    *scratch = packet + sizeof(DrawPacket);
}

void GameDrawText8x8Shaded(
    s32 x,
    s32 y,
    u8 *str,
    s32 clutIndex,
    u8 intensity) {
    u8 **scratch = (u8 **)0x1F800000;
    u8 *packet;
    u8 *prim;

    packet = *scratch;
    if (*str != 0) {
        volatile TextSprt8 *sprt;
        u8 *font;

        INIT_TEXT_FONT(font);
        sprt = (TextSprt8 *)packet;
        do {
            s32 cell = *str - 0x20;

            str++;
            if (cell != 0) {
                s32 u;
                s32 v;

                {
                    s32 index;
                    u8 *fontUCell;
                    u8 *fontV;

                    index = cell * 2;
                    fontUCell = (u8 *)(index + (s32)font);
                    fontV = &g_Font8x8Cells[1];
                    u = *fontUCell * 8;
                    v = *(u8 *)(index + (s32)fontV) * 8;
                }

                SetSprt8(packet);
                SetSemiTrans(packet, 1);
                sprt->x0 = x;
                sprt->y0 = y;
                sprt->u0 = u;
                sprt->v0 = v;
                sprt->tag.r0 = intensity;
                sprt->tag.g0 = intensity;
                sprt->tag.b0 = intensity;
                asm("");
                prim = (u8 *)sprt;
                sprt->clut = clutIndex;
                AddPrim(g_DrawBuffer + 0xCC, prim);
                sprt++;
                packet += sizeof(TextSprt8);
            }
            x += 8;
        } while (*str != 0);
    }
    SetDrawModeWide8x8(packet, 0, 1, 0x29, g_DrawModeEnv);
    AddPrim(g_DrawBuffer + 0xCC, packet);
    *scratch = packet + sizeof(DrawPacket);
}

void DrawText8x8Trans(s32 x, s32 y, u8 *str, s32 clutIndex) {
    u8 **scratch = (u8 **)0x1F800000;
    u8 *packet;

    packet = *scratch;
    if (*str != 0) {
        volatile TextSprt8 *sprt;
        u8 *font;

        INIT_TEXT_FONT(font);
        sprt = (TextSprt8 *)packet;
        do {
            s32 cell = *str - 0x20;

            str++;
            if (cell != 0) {
                s32 index;
                u8 *fontUCell;
                u8 *fontV;
                s32 u;
                s32 v;

                index = cell * 2;
                fontUCell = (u8 *)(index + (s32)font);
                fontV = &g_Font8x8Cells[1];
                u = *fontUCell * 8;
                v = *(u8 *)(index + (s32)fontV) * 8;

                SetSprt8(packet);
                SetShadeTex(packet, 1);
                SetSemiTrans(packet, 1);
                sprt->x0 = x;
                sprt->y0 = y;
                sprt->u0 = u;
                sprt->v0 = v;
                sprt->clut = clutIndex;
                AddPrim(g_DrawBuffer + 0xCC, (void *)sprt);
                sprt++;
                packet += sizeof(TextSprt8);
            }
            x += 8;
        } while (*str != 0);
    }
    SetDrawModeWide8x8(packet, 0, 1, 0x49, g_DrawModeEnv);
    AddPrim(g_DrawBuffer + 0xCC, packet);
    *scratch = packet + sizeof(DrawPacket);
}

#undef INIT_TEXT_FONT

/*
 * Local wide-parameter declarations. Retail passes x / y / clutIndex straight
 * through in full words; the s16 / u16 typing in game/render.h would make gcc
 * insert sign-extends and a truncation here, so this TU declares both the
 * callee and this function with s32 parameters instead of including the header.
 */
void GameDrawProportionalTextShaded(
    s32 x,
    s32 y,
    u8 *str,
    s32 clutIndex,
    s32 intensity);

void GameDrawProportionalTextShaded(
    s32 x,
    s32 y,
    u8 *str,
    s32 clutIndex,
    s32 intensity) {
#define OPAQUE_VALUE (t0 = 0x100)
    s32 xPos = x;
    u8 *packet = *(u8 **)0x1F800000;
    u8 *text = str;
    register s32 shade asm("$23");
    register s32 t0 asm("$8");
    s32 s1;
    u32 first;
    s32 v;
    s32 u;
    struct {
        s32 y;
        s32 pad;
        s32 clut;
    } home;

    home.y = y;
    home.clut = clutIndex;
    first = *text;
    shade = intensity;

    if (first != 0) {
        s32 height = 12;
        register SPRT *sprt asm("$16") = (SPRT *)packet;

        do {
            s32 advance;
            u32 ch = *text;

            if (ch >= 0x76) {
                s32 offset = ch - 0x76;
                s32 index = offset * 4;
                s32 width;
                void *prim;
                u8 *ot;
                s16 yOffset;

                asm(
                    "" : "=r"(offset), "=r"(index), "=r"(v) :
                    "0"(offset), "1"(index));
                text++;
                u = g_HighFontU[index];
                v = g_HighFontV[index];
                SetSprt(packet);
                if (shade == OPAQUE_VALUE) {
                    SetShadeTex(packet, 1);
                    *(volatile s16 *)((u8 *)sprt + 8) = xPos;
                } else {
                    SetSemiTrans(packet, 1);
                    sprt->t.r0 = shade;
                    sprt->t.g0 = shade;
                    sprt->t.b0 = shade;
                    sprt->x0 = xPos;
                }
                yOffset = g_HighFontYOffset[index];
                t0 = home.y;
                asm(
                    "" : "=r"(yOffset), "=r"(t0) :
                    "0"(yOffset), "1"(t0));
                packet += 20;
                sprt->y0 = yOffset + t0;
                width = g_HighFontWidth[index];
                prim = (void *)sprt;
                sprt->u0 = u;
                sprt->v0 = v;
                /* RAW() keeps this store ahead of the g_DrawBuffer load --
                 * see common.h. */
                RAW(sprt->h) = height;
                ot = g_DrawBuffer;
                t0 = (u16)home.clut;
                ot += 0xCC;
                sprt->clut = t0;
                sprt->w = width;
                AddPrim(ot, prim);
                advance = g_WordFontWidth[index];
                sprt++;
                xPos += advance;
                continue;
            }
            if (ch >= 0x61) {
                s32 offset = ch - 0x61;
                s32 width;
                void *prim;
                u8 *ot;

                s1 = offset * 4;
                text++;
                u = g_WordFontU[s1];
                v = g_WordFontV[s1];
                SetSprt(packet);
                if (shade == OPAQUE_VALUE) {
                    SetShadeTex(packet, 1);
                    *(volatile s16 *)((u8 *)sprt + 8) = xPos;
                } else {
                    SetSemiTrans(packet, 1);
                    sprt->t.r0 = shade;
                    sprt->t.g0 = shade;
                    sprt->t.b0 = shade;
                    sprt->x0 = xPos;
                }
                t0 = (u16)home.y;
                packet += 20;
                sprt->y0 = t0;
                width = g_WordFontWidth[s1];
                prim = (void *)sprt;
                sprt->u0 = u;
                sprt->v0 = v;
                /* RAW() keeps this store ahead of the g_DrawBuffer load --
                 * see common.h. */
                RAW(sprt->h) = height;
                ot = g_DrawBuffer;
                t0 = (u16)home.clut;
                ot += 0xCC;
                sprt->clut = t0;
                sprt->w = width;
                AddPrim(ot, prim);
                advance = g_WordFontAdvance[s1];
                sprt++;
                xPos += advance;
                continue;
            }
            {
                s1 = ch - 0x20;

                asm("" : "=r"(s1), "=r"(v) : "0"(s1));
                text++;
                if (s1 != 0) {
                    s32 index = s1 * 2;
                    u8 *uCell;
                    u8 *vCell;
                    void *prim;
                    u8 *ot;

                    asm volatile("" : "=r"(index) : "0"(index) : "$2");
                    t0 = (s32)g_PropFontU;
                    uCell = (u8 *)(index + t0);
                    t0 = (s32)g_PropFontV;
                    vCell = (u8 *)(index + t0);
                    asm("" : "=r"(vCell) : "0"(vCell), "r"(uCell));
                    u = *uCell;
                    v = *vCell;
                    SetSprt(packet);
                    if (shade == OPAQUE_VALUE) {
                        SetShadeTex(packet, 1);
                        *(volatile s16 *)((u8 *)sprt + 8) = xPos;
                    } else {
                        SetSemiTrans(packet, 1);
                        sprt->t.r0 = shade;
                        sprt->t.g0 = shade;
                        sprt->t.b0 = shade;
                        sprt->x0 = xPos;
                    }
                    t0 = (u16)home.y;
                    asm("" : "=r"(t0) : "0"(t0));
                    prim = (void *)sprt;
                    asm("" : "=r"(prim) : "0"(prim));
                    sprt->u0 = u;
                    sprt->v0 = v;
                    ot = g_DrawBuffer;
                    packet += 20;
                    sprt->w = height;
                    sprt->h = height;
                    sprt->y0 = t0;
                    t0 = (u16)home.clut;
                    ot += 0xCC;
                    sprt->clut = t0;
                    asm volatile("" ::: "memory");
                    sprt++;
                    AddPrim(ot, prim);
                }
                xPos += 12;
            }
            ;
        } while (*text != 0);
    }
    SetDrawModeWide8x8(packet, 0, 1, 0x29, g_DrawModeEnv);
    AddPrim(g_DrawBuffer + 0xCC, packet);
    *(u8 **)0x1F800000 = ({
        u8 *next = packet + 12;

        next;
    });
#undef OPAQUE_VALUE
}

void DrawProportionalText(s32 x, s32 y, u8 *str, s32 clutIndex);

/* Opaque wrapper over GameDrawProportionalTextShaded: intensity 0x100 selects
 * the raw-texture (SetShadeTex) path instead of a modulated, semi-transparent
 * one. */
void DrawProportionalText(s32 x, s32 y, u8 *str, s32 clutIndex) {
    GameDrawProportionalTextShaded(x, y, str, clutIndex, 0x100);
}


/*
 * Local wide-parameter declaration: retail passes every coordinate, texel and
 * CLUT index as a full word (the stack arguments are read with `lw`), so the
 * narrow documentation types in game/render.h would make gcc shrink the loads.
 */
u8 *GameQueueSprite(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 u,
    s32 v,
    s32 clutIndex);

/* SPRT, 20 bytes: a raw (SetShadeTex) textured sprite linked into `ot`.
 * Returns the advanced packet cursor. */
u8 *GameQueueSprite(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 u,
    s32 v,
    s32 clutIndex) {
    SPRT *sprt = (SPRT *)prim;

    SetSprt(sprt);
    SetShadeTex(sprt, 1);
    sprt->x0 = x;
    sprt->y0 = y;
    sprt->w = w;
    sprt->h = h;
    sprt->u0 = u;
    sprt->v0 = v;
    sprt->clut = clutIndex;
    prim += 20;
    AddPrim(ot, sprt);
    return prim;
}

/* Local wide-parameter declaration; see GameQueueSprite.c. `clutIndex` really
 * is the narrow one here - retail reads it back out of its argument slot with
 * `lhu` at the point of use. */
u8 *GameQueueShadedSprite(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 u,
    s32 v,
    s32 clutIndex,
    s32 intensity) asm("func_80016F8C");

/* SPRT, 20 bytes: a textured sprite modulated by `intensity` on all three
 * channels (no SetShadeTex, so the texel is shaded). */
u8 *GameQueueShadedSprite(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 u,
    s32 v,
    s32 clutIndex,
    s32 intensity) {
    SPRT *sprt = (SPRT *)prim;

    SetSprt(sprt);
    sprt->x0 = x;
    sprt->y0 = y;
    sprt->w = w;
    sprt->h = h;
    sprt->u0 = u;
    sprt->v0 = v;
    sprt->t.r0 = intensity;
    sprt->t.g0 = intensity;
    sprt->t.b0 = intensity;
    sprt->clut = clutIndex;
    prim += 20;
    AddPrim(ot, sprt);
    return prim;
}

/* Local wide-parameter declaration; see the definition below. */
u8 *GameQueueShadedSpriteTrans(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 u,
    s32 v,
    s32 clutIndex,
    s32 intensity) asm("func_8001705C");

/* SPRT, 20 bytes: GameQueueShadedSprite plus SetSemiTrans. */
u8 *GameQueueShadedSpriteTrans(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 u,
    s32 v,
    s32 clutIndex,
    s32 intensity) {
    SPRT *sprt = (SPRT *)prim;

    SetSprt(sprt);
    SetSemiTrans(sprt, 1);
    sprt->x0 = x;
    sprt->y0 = y;
    sprt->w = w;
    sprt->h = h;
    sprt->u0 = u;
    sprt->v0 = v;
    sprt->t.r0 = intensity;
    sprt->t.g0 = intensity;
    sprt->t.b0 = intensity;
    sprt->clut = clutIndex;
    prim += 20;
    AddPrim(ot, sprt);
    return prim;
}

/* Local wide-parameter declaration; see GameQueueSprite.c. */
u8 *GameQueueSpriteTrans(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 u,
    s32 v,
    s32 clutIndex) asm("func_80017138");

/* SPRT, 20 bytes: GameQueueSprite plus SetSemiTrans. */
u8 *GameQueueSpriteTrans(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 u,
    s32 v,
    s32 clutIndex) {
    SPRT *sprt = (SPRT *)prim;

    SetSprt(sprt);
    SetSemiTrans(sprt, 1);
    SetShadeTex(sprt, 1);
    sprt->x0 = x;
    sprt->y0 = y;
    sprt->w = w;
    sprt->h = h;
    sprt->u0 = u;
    sprt->v0 = v;
    sprt->clut = clutIndex;
    prim += 20;
    AddPrim(ot, sprt);
    return prim;
}

/* Local wide-parameter declaration; see GameQueueSprite.c. */
u8 *GameQueueTileTrans(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 r,
    s32 g,
    s32 b) asm("func_8001720C");

/* TILE, 16 bytes: a semi-transparent solid rectangle linked into `ot`.
 * Returns the advanced packet cursor. */
u8 *GameQueueTileTrans(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 r,
    s32 g,
    s32 b) {
    TILE *tile = (TILE *)prim;

    SetTile(tile);
    SetSemiTrans(tile, 1);
    tile->x0 = x;
    tile->y0 = y;
    tile->w = w;
    tile->h = h;
    tile->t.r0 = r;
    tile->t.g0 = g;
    tile->t.b0 = b;
    prim += 16;
    AddPrim(ot, tile);
    return prim;
}

/*
 * Local wide-parameter declaration. Retail passes every coordinate and colour
 * component as a full word - the stack arguments are read with `lw` - so the
 * s16 / u8 typing that game/render.h uses for documentation would make gcc
 * narrow the loads here. Only this TU needs the wide view, so it declares the
 * symbol itself rather than changing the shared header.
 */
u8 *GameQueueLine(
    void *ot,
    u8 *prim,
    s32 x0,
    s32 y0,
    s32 x1,
    s32 y1,
    s32 r,
    s32 g,
    s32 b) asm("func_800172D4");

/* LINE_F2, 16 bytes: one flat-shaded line, linked into `ot`. Returns the
 * advanced packet cursor. */
u8 *GameQueueLine(
    void *ot,
    u8 *prim,
    s32 x0,
    s32 y0,
    s32 x1,
    s32 y1,
    s32 r,
    s32 g,
    s32 b) {
    LINE_F2 *line = (LINE_F2 *)prim;

    SetLineF2(line);
    line->x0 = x0;
    line->y0 = y0;
    line->x1 = x1;
    line->y1 = y1;
    line->t.r0 = r;
    line->t.g0 = g;
    line->t.b0 = b;
    prim += 16;
    AddPrim(ot, line);
    return prim;
}
