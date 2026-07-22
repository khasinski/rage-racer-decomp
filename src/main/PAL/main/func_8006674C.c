#include "common.h"
#include "psyq/gpu.h"

extern u16 D_800941EC[];
extern u16 D_800941EE[];

typedef struct {
    Rect clip;
    s16 ofs[2];
    GpuTexWindow tw;
    u16 tpage;
    u8 dtd;
    u8 dfe;
    u8 isbg;
    u8 r0;
    u8 g0;
    u8 b0;
} DrawEnvPacketSource;

u32 Gpu_BuildDrawAreaTopLeftCmd(s32 x, s32 y) asm("func_80066A4C");
u32 Gpu_BuildDrawAreaBottomRightCmd(s32 x, s32 y) asm("func_80066B18");
u32 Gpu_BuildDrawOffsetCmd(s32 x, s32 y) asm("func_80066BE4");
u32 Gpu_BuildDrawModeCmd(s32 dfe, s32 dtd, u32 tpage) asm("func_800669F0");
u32 Gpu_BuildTexWindowCmd(GpuTexWindow *tw) asm("func_80066C2C");

void func_8006674C(u32 *packet, DrawEnvPacketSource *env) {
    register u32 *out asm("$17") = packet;
    register DrawEnvPacketSource *src asm("$16") = env;
    register s32 count asm("$8");
    register s32 value asm("$2");
    register s32 coord asm("$3");
    register s32 limit asm("$4");
    Rect clipped;

    asm("" : "=r"(src) : "0"(src));
    out[1] = Gpu_BuildDrawAreaTopLeftCmd(src->clip.x, src->clip.y);
    out[2] = Gpu_BuildDrawAreaBottomRightCmd(
        (s16)(src->clip.w + src->clip.x - 1),
        (s16)(src->clip.y + src->clip.h - 1));
    out[3] = Gpu_BuildDrawOffsetCmd(src->ofs[0], src->ofs[1]);
    out[4] = Gpu_BuildDrawModeCmd(src->dfe, src->dtd, src->tpage);
    out[5] = Gpu_BuildTexWindowCmd(&src->tw);
    out[6] = 0xE6000000;

    count = 7;
    if (src->isbg != 0) {
        value = *(u16 *)&src->clip.x;
        *(u16 *)&clipped.x = value;
        value = *(u16 *)&src->clip.y;
        *(u16 *)&clipped.y = value;
        value = *(u16 *)&src->clip.w;
        *(u16 *)&clipped.w = value;
        coord = *(u16 *)&src->clip.h;
        value <<= 16;
        *(u16 *)&clipped.h = coord;
        coord = value >> 16;

        if (coord >= 0) {
            volatile u16 *width = D_800941EC;

            value = *width;
            value = (s16)value;
            limit = value - 1;
            value = limit < coord;
            if (value != 0) {
                coord = limit;
            }
            value = coord;
        } else {
            value = 0;
        }
        clipped.w = value;

        coord = clipped.h;
        if (coord >= 0) {
            volatile u16 *height = D_800941EE;

            value = *height;
            value = (s16)value;
            limit = value - 1;
            value = limit < coord;
            if (value != 0) {
                coord = limit;
            }
            value = coord;
        } else {
            value = 0;
        }
        coord = (u16)clipped.x;
        clipped.h = value;

        value = coord & 0x3F;
        if ((value != 0) || (((u16)clipped.w & 0x3F) != 0)) {
            register s32 commandOffset asm("$6");
            register s32 positionOffset asm("$5");
            register u32 blue asm("$2");
            register u32 green asm("$3");
            register u32 red asm("$4");

            commandOffset = count << 2;
            asm("" : "=r"(commandOffset) : "0"(commandOffset));
            count++;
            positionOffset = count << 2;
            asm("" : "=r"(positionOffset) : "0"(positionOffset));
            count++;
            value = (u16)src->ofs[0];
            asm("" : "=r"(value) : "0"(value));
            commandOffset += (s32)out;
            asm("" : "=r"(commandOffset) : "0"(commandOffset));
            value = coord - value;
            clipped.x = value;
            value = (u16)clipped.y;
            coord = (u16)src->ofs[1];
            red = 0x60000000;
            value -= coord;
            clipped.y = value;
            blue = src->b0;
            green = src->g0;
            blue <<= 16;
            green <<= 8;
            green |= red;
            red = src->r0;
            blue |= green;
            blue |= red;
            *(u32 *)commandOffset = blue;
            value = *(u32 *)&clipped.x;
            positionOffset += (s32)out;
            *(u32 *)positionOffset = value;
            out[count] = *(u32 *)&clipped.w;
            clipped.x += src->ofs[0];
            clipped.y += src->ofs[1];
            count++;
        } else {
            register s32 commandOffset asm("$5");
            register s32 positionOffset asm("$6");
            register s32 sizeOffset asm("$7");
            register u32 blue asm("$2");
            register u32 green asm("$3");
            register u32 red asm("$4");

            commandOffset = count << 2;
            asm("" : "=r"(commandOffset) : "0"(commandOffset));
            count++;
            positionOffset = count << 2;
            asm("" : "=r"(positionOffset) : "0"(positionOffset));
            count++;
            sizeOffset = count << 2;
            asm("" : "=r"(sizeOffset) : "0"(sizeOffset));
            count++;
            commandOffset += (s32)out;
            asm("" : "=r"(commandOffset) : "0"(commandOffset));
            red = 0x02000000;
            blue = src->b0;
            green = src->g0;
            blue <<= 16;
            green <<= 8;
            green |= red;
            red = src->r0;
            blue |= green;
            blue |= red;
            *(u32 *)commandOffset = blue;
            value = *(u32 *)&clipped.x;
            positionOffset += (s32)out;
            *(u32 *)positionOffset = value;
            value = *(u32 *)&clipped.w;
            sizeOffset += (s32)out;
            *(u32 *)sizeOffset = value;
        }
    }

    value = count - 1;
    ((u8 *)out)[3] = value;
}
