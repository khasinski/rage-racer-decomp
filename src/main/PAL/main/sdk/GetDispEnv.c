#include "common.h"
#include "psyq/gpu.h"

extern u_char g_DispEnvCache;
void MemCopy(long arg0, void *arg1, long arg2);
long GetDispEnv(long arg0);
long GetDispEnv(long arg0) { MemCopy(arg0, &g_DispEnvCache, 0x14); return arg0; }

extern GpuCallbacks *g_GpuFuncs;

u_long GetODE(void) {
    u_long ret;

    ret = g_GpuFuncs->status();
    return ret >> 31;
}

u_long Gpu_BuildDisplayMode(long arg0, long arg1, u_long arg2) asm("_get_mode");
u_long Gpu_BuildDrawAreaTopLeftCmd(long x, long y);
u_long Gpu_BuildDrawAreaBottomRightCmd(long x, long y);
u_long Gpu_BuildDrawOffsetCmd(long x, long y);
u_long Gpu_BuildTexWindowCmd(void *arg0);

void SetTexWindow(DrawPacket *pkt, void *arg1) {
    pkt->code = 2;
    pkt->x0y0 = Gpu_BuildTexWindowCmd(arg1);
    pkt->x1y1 = 0;
}

void SetDrawArea(DrawPacket *prim, Rect *rect);
void SetDrawArea(DrawPacket *pkt, Rect *rect) {
    pkt->code = 2;
    pkt->x0y0 = Gpu_BuildDrawAreaTopLeftCmd(rect->x, rect->y);
    pkt->x1y1 = Gpu_BuildDrawAreaBottomRightCmd((short)(rect->x + rect->w - 1), (short)(rect->y + rect->h - 1));
}

void SetDrawOffset(DrawPacket *pkt, short *arg1) {
    pkt->code = 2;
    pkt->x0y0 = Gpu_BuildDrawOffsetCmd(arg1[0], arg1[1]);
    pkt->x1y1 = 0;
}

void SetDrawStp(DrawPacket *pkt, long arg1, u_long arg2) {
    u_long cmd;

    pkt->code = 2;
    cmd = 0xE6000000;
    if (arg1 != 0) {
        cmd = 0xE6000002;
    }
    pkt->x0y0 = cmd | (arg2 != 0);
    pkt->x1y1 = 0;
}

void SetDrawMode(DrawPacket *pkt, long arg1, long arg2, u_short arg3, void *arg4) {
    pkt->code = 2;
    pkt->x0y0 = Gpu_BuildDisplayMode(arg1, arg2, arg3);
    pkt->x1y1 = Gpu_BuildTexWindowCmd(arg4);
}

extern u_short g_VramWidth[];
extern u_short g_VramHeight[];

typedef struct DrawEnvPacketSource {
    Rect clip;
    short ofs[2];
    GpuTexWindow tw;
    u_short tpage;
    u_char dtd;
    u_char dfe;
    u_char isbg;
    u_char r0;
    u_char g0;
    u_char b0;
} DrawEnvPacketSource;

u_long Gpu_BuildDrawAreaTopLeftCmd(long x, long y);
u_long Gpu_BuildDrawAreaBottomRightCmd(long x, long y);
u_long Gpu_BuildDrawOffsetCmd(long x, long y);
u_long Gpu_BuildDrawModeCmd(long dfe, long dtd, u_long tpage) asm("_get_mode");

void Gpu_BuildDrawEnvCmds(u_long *packet, DrawEnvPacketSource *env) {
    u_long *out = packet;
    DrawEnvPacketSource *src = env;
    long count;
    long value;
    long coord;
    long limit;
    Rect clipped;

    asm("" : "=r"(src) : "0"(src));
    out[1] = Gpu_BuildDrawAreaTopLeftCmd(src->clip.x, src->clip.y);
    out[2] = Gpu_BuildDrawAreaBottomRightCmd(
        (short)(src->clip.w + src->clip.x - 1),
        (short)(src->clip.y + src->clip.h - 1));
    out[3] = Gpu_BuildDrawOffsetCmd(src->ofs[0], src->ofs[1]);
    out[4] = Gpu_BuildDrawModeCmd(src->dfe, src->dtd, src->tpage);
    out[5] = Gpu_BuildTexWindowCmd(&src->tw);
    out[6] = 0xE6000000;

    count = 7;
    if (src->isbg != 0) {
        value = *(u_short *)&src->clip.x;
        *(u_short *)&clipped.x = value;
        value = *(u_short *)&src->clip.y;
        *(u_short *)&clipped.y = value;
        value = *(u_short *)&src->clip.w;
        *(u_short *)&clipped.w = value;
        coord = *(u_short *)&src->clip.h;
        value <<= 16;
        *(u_short *)&clipped.h = coord;
        coord = value >> 16;

        if (coord >= 0) {
            volatile u_short *width = g_VramWidth;

            value = *width;
            value = (short)value;
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
            volatile u_short *height = g_VramHeight;

            value = *height;
            value = (short)value;
            limit = value - 1;
            value = limit < coord;
            if (value != 0) {
                coord = limit;
            }
            value = coord;
        } else {
            value = 0;
        }
        coord = (u_short)clipped.x;
        clipped.h = value;

        value = coord & 0x3F;
        if ((value != 0) || (((u_short)clipped.w & 0x3F) != 0)) {
            register long commandOffset asm("$6");
            register long positionOffset asm("$5");
            u_long blue;
            u_long green;
            u_long red;

            commandOffset = count * 4;
            count++;
            positionOffset = count * 4;
            count++;
            value = (u_short)src->ofs[0];
            asm("" : "=r"(value) : "0"(value));
            commandOffset += (long)out;
            asm("" : "=r"(commandOffset) : "0"(commandOffset));
            value = coord - value;
            clipped.x = value;
            value = (u_short)clipped.y;
            coord = (u_short)src->ofs[1];
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
            *(u_long *)commandOffset = blue;
            value = *(u_long *)&clipped.x;
            positionOffset += (long)out;
            *(u_long *)positionOffset = value;
            out[count] = *(u_long *)&clipped.w;
            clipped.x += src->ofs[0];
            clipped.y += src->ofs[1];
            count++;
        } else {
            long commandOffset;
            long positionOffset;
            long sizeOffset;
            u_long blue;
            u_long green;
            register u_long red asm("$4");

            commandOffset = count << 2;
            count++;
            positionOffset = count << 2;
            count++;
            sizeOffset = count << 2;
            count++;
            commandOffset += (long)out;
            red = 0x02000000;
            blue = src->b0;
            green = src->g0;
            blue <<= 16;
            green <<= 8;
            green |= red;
            red = src->r0;
            blue |= green;
            blue |= red;
            *(u_long *)commandOffset = blue;
            value = *(u_long *)&clipped.x;
            positionOffset += (long)out;
            *(u_long *)positionOffset = value;
            value = *(u_long *)&clipped.w;
            sizeOffset += (long)out;
            *(u_long *)sizeOffset = value;
        }
    }

    value = count - 1;
    ((u_char *)out)[3] = value;
}
