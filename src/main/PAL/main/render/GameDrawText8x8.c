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

extern u8 *g_DrawBuffer asm("D_8019C900");
extern u8 g_Font8x8Cells[] asm("D_8007C2F8");
extern u8 g_DrawModeEnv[] asm("D_8007BED0");
extern u8 g_PropFontU[] asm("D_8007C3B8");
extern u8 g_PropFontV[] asm("D_8007C3B9");
extern u8 g_WordFontU[] asm("D_8007C438");
extern u8 g_WordFontV[] asm("D_8007C439");
extern u8 g_WordFontWidth[] asm("D_8007C43A");
extern u8 g_WordFontAdvance[] asm("D_8007C43B");
extern u8 g_HighFontU[] asm("D_8007C460");
extern u8 g_HighFontV[] asm("D_8007C461");
extern u8 g_HighFontWidth[] asm("D_8007C462");
extern u8 g_HighFontYOffset[] asm("D_8007C463");

/*
 * Keep the first font base opaque after materialising it. This empty constraint
 * makes gcc emit that base before the sprite cursor while still rematerialising
 * g_Font8x8Cells + 1 in the loop, as retail does.
 */
#define INIT_TEXT_FONT(font) \
    asm("" : "=r"(font) : "0"(g_Font8x8Cells))

void GameDrawText8x8(
    s32 x,
    s32 y,
    u8 *str,
    s32 clutIndex) asm("func_80016754");
void GameDrawText8x8Shaded(
    s32 x,
    s32 y,
    u8 *str,
    s32 clutIndex,
    u8 intensity) asm("func_800168AC");
void GameDrawText8x8Trans(
    s32 x,
    s32 y,
    u8 *str,
    s32 clutIndex) asm("func_80016A18");
void SetDrawModeWide8x8(
    void *packet,
    s32 dither,
    s32 drawToDisplay,
    s32 tpage,
    void *textureWindow) asm("func_800666F4");

void GameDrawText8x8(s32 x, s32 y, u8 *str, s32 clutIndex) {
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
                register u8 *fontV asm("$8");
                s32 u;
                s32 v;

                /*
                 * v0/v1 now allocate naturally. The remaining t0 pin and
                 * empty constraint keep fontV tied to the independently
                 * materialised second font base.
                 */
                index = cell * 2;
                fontUCell = (u8 *)(index + (s32)font);
                asm(
                    "" : "=r"(fontV) :
                    "0"(&g_Font8x8Cells[1]),
                    "r"(fontUCell), "r"(index));
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
                register u8 *sprtArg asm("$4");
                s32 u;
                s32 v;

                sprtArg = packet;
                {
                    s32 index;
                    u8 *fontUCell;
                    register u8 *fontV asm("$8");

                    index = cell * 2;
                    fontUCell = (u8 *)(index + (s32)font);
                    asm(
                        "" : "=r"(fontV) :
                        "0"(&g_Font8x8Cells[1]),
                        "r"(fontUCell), "r"(index));
                    u = *fontUCell * 8;
                    v = *(u8 *)(index + (s32)fontV) * 8;
                }

                SetSprt8(sprtArg);
                SetSemiTrans(packet, 1);
                sprt->x0 = x;
                sprt->y0 = y;
                sprt->u0 = u;
                sprt->v0 = v;
                sprt->tag.r0 = intensity;
                sprt->tag.g0 = intensity;
                sprt->tag.b0 = intensity;
                asm("" ::: "memory");
                prim = (u8 *)sprt;
                {
                    register u16 clutValue asm("$8");

                    clutValue = clutIndex;
                    sprt->clut = clutValue;
                }
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

void GameDrawText8x8Trans(s32 x, s32 y, u8 *str, s32 clutIndex) {
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
                register u8 *fontV asm("$8");
                s32 u;
                s32 v;

                index = cell * 2;
                fontUCell = (u8 *)(index + (s32)font);
                asm(
                    "" : "=r"(fontV) :
                    "0"(&g_Font8x8Cells[1]),
                    "r"(fontUCell), "r"(index));
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
void DrawProportionalTextShadedWide(
    s32 x,
    s32 y,
    u8 *str,
    s32 clutIndex,
    s32 intensity) asm("func_80016B7C");

void DrawProportionalTextShadedWide(
    s32 x,
    s32 y,
    u8 *str,
    s32 clutIndex,
    s32 intensity) {
#define OPAQUE_VALUE ({ \
    register s32 opaque asm("$8") = 0x100; \
 \
    opaque; \
})
    register s32 xPos;
    register u8 *packet = *(u8 **)0x1F800000;
    register u8 *text = ({
        asm("" : "=r"(packet), "=r"(xPos) : "0"(packet));
        str;
    });
    register s32 shade asm("$23");
    u32 first;
    struct {
        s32 y;
        s32 pad;
        s32 clut;
    } home;

    home.y = y;
    home.clut = clutIndex;
    first = *text;
    shade = intensity;
    xPos = x;

    if (first != 0) {
        s32 height = 12;
        register SPRT *sprt asm("$16") = (SPRT *)packet;

        do {
            s32 advance;
            register u32 ch = *text;

            if (ch >= 0x76) {
                register s32 offset = ch - 0x76;
                register s32 index = offset * 4;
                register s32 v;
    s32 u;
                register s32 width;
                register void *prim;
                register u8 *ot;
                register u16 clut asm("$8");
                s16 yOffset;
                register s32 yValue asm("$8");

                asm(
                    "" : "=r"(offset), "=r"(index) :
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
                yValue = home.y;
                asm(
                    "" : "=r"(yOffset), "=r"(yValue) :
                    "0"(yOffset), "1"(yValue));
                packet += 20;
                sprt->y0 = yOffset + yValue;
                width = g_HighFontWidth[index];
                asm volatile("" : "=r"(width) : "0"(width));
                prim = (void *)sprt;
                asm("" : "=r"(prim) : "0"(prim));
                sprt->u0 = u;
                sprt->v0 = v;
                sprt->h = height;
                asm volatile("" ::: "memory");
                ot = g_DrawBuffer;
                asm("" : "=r"(ot) : "0"(ot));
                clut = home.clut;
                asm("" : "=r"(clut) : "0"(clut));
                ot += 0xCC;
                sprt->clut = clut;
                sprt->w = width;
                AddPrim(ot, prim);
                advance = g_WordFontWidth[index];
                goto advance_sprite;
            }
            if (ch >= 0x61) {
                register s32 offset = ch - 0x61;
                register s32 index asm("$17") = offset * 4;
                register s32 u asm("$21");
                register s32 v;
                register s32 width;
                register void *prim;
                register u8 *ot;
                register u16 clut asm("$8");
                register u16 yValue asm("$8");

                asm(
                    "" : "=r"(offset), "=r"(index) :
                    "0"(offset), "1"(index));
                text++;
                asm("" : : "r"(ch));
                u = g_WordFontU[index];
                v = g_WordFontV[index];
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
                yValue = home.y;
                packet += 20;
                sprt->y0 = yValue;
                width = g_WordFontWidth[index];
                asm volatile("" : "=r"(width) : "0"(width));
                prim = (void *)sprt;
                asm("" : "=r"(prim) : "0"(prim));
                sprt->u0 = u;
                sprt->v0 = v;
                sprt->h = height;
                asm volatile("" ::: "memory");
                ot = g_DrawBuffer;
                asm("" : "=r"(ot) : "0"(ot));
                clut = home.clut;
                asm("" : "=r"(clut) : "0"(clut));
                ot += 0xCC;
                sprt->clut = clut;
                sprt->w = width;
                AddPrim(ot, prim);
                advance = g_WordFontAdvance[index];
advance_sprite:
                sprt++;
                xPos += advance;
                continue;
            }
            {
                register s32 cell asm("$17") = ch - 0x20;

                asm("" : "=r"(cell) : "0"(cell));
                text++;
                if (cell != 0) {
                    register s32 index asm("$3") = cell * 2;
                    register u8 *base asm("$8");
                    register u8 *uCell;
                    register u8 *vCell;
                    register s32 u asm("$21");
                    register s32 v asm("$19");
                    register u16 yValue asm("$8");
                    register void *prim;
                    register u8 *ot;
                    register u16 clut asm("$8");

                    asm("" : "=r"(index) : "0"(index));
                    base = g_PropFontU;
                    uCell = (u8 *)(index + (s32)base);
                    base = g_PropFontV;
                    vCell = (u8 *)(index + (s32)base);
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
                    yValue = home.y;
                    asm("" : "=r"(yValue) : "0"(yValue));
                    prim = (void *)sprt;
                    asm("" : "=r"(prim) : "0"(prim));
                    sprt->u0 = u;
                    sprt->v0 = v;
                    ot = g_DrawBuffer;
                    packet += 20;
                    sprt->w = height;
                    sprt->h = height;
                    sprt->y0 = yValue;
                    clut = home.clut;
                    ot += 0xCC;
                    sprt->clut = clut;
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
        register u8 *next = packet + 12;

        next;
    });
#undef OPAQUE_VALUE
}

void GameDrawProportionalText(s32 x, s32 y, u8 *str, s32 clutIndex) asm("func_80016EA0");

/* Opaque wrapper over GameDrawProportionalTextShaded: intensity 0x100 selects
 * the raw-texture (SetShadeTex) path instead of a modulated, semi-transparent
 * one. */
void GameDrawProportionalText(s32 x, s32 y, u8 *str, s32 clutIndex) {
    DrawProportionalTextShadedWide(x, y, str, clutIndex, 0x100);
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
    s32 clutIndex) asm("func_80016EC4");

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
