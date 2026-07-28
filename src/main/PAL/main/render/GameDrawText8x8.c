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

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/render/GameDrawText8x8", func_80016B7C);

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
void GameDrawProportionalText(s32 x, s32 y, u8 *str, s32 clutIndex) asm("func_80016EA0");

/* Opaque wrapper over GameDrawProportionalTextShaded: intensity 0x100 selects
 * the raw-texture (SetShadeTex) path instead of a modulated, semi-transparent
 * one. */
void GameDrawProportionalText(s32 x, s32 y, u8 *str, s32 clutIndex) {
    DrawProportionalTextShadedWide(x, y, str, clutIndex, 0x100);
}
