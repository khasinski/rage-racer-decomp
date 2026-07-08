#include "common.h"
#include "psyq/gpu.h"

void func_80066604(void *packet, void *rect);
void func_80064DDC(void *ot, void *packet);

void func_800468FC(void *ot, s32 x, s32 y, s32 w, s32 h) {
    register void *otReg asm("$17");
    register u8 **scratch asm("$18");
    register u8 *packet asm("$16");
    register s32 xReg asm("$9");
    register s32 yReg asm("$10");
    register s32 wReg asm("$11");
    register s32 hReg asm("$8");
    register u8 *oldPacket asm("$5");
    register s32 tmp asm("$2");
    Rect rect;

    otReg = ot;
    asm("" : "=r"(otReg) : "0"(otReg));
    xReg = x;
    yReg = y;
    wReg = w;
    scratch = (u8 **)0x1F800000;
    hReg = h;
    packet = *scratch;

    if ((s16)x + (s16)w > 0) {
        if ((s16)xReg < 0) {
            wReg = w + x;
            xReg = 0;
        }

        if ((s16)xReg < 0x140) {
            if ((s16)xReg + (s16)wReg >= 0x140) {
                tmp = 0x140;
                wReg = tmp - xReg;
            }

            if ((s16)y + (s16)h > 0) {
                if ((s16)yReg < 0) {
                    hReg = h + y;
                    yReg = 0;
                }

                if ((s16)yReg < 0x1E0) {
                    if ((s16)yReg + (s16)hReg >= 0x1E0) {
                        tmp = 0x1E0;
                        hReg = tmp - yReg;
                    }

                    rect.x = xReg;
                    rect.y = yReg;
                    rect.w = wReg;
                    rect.h = hReg;
                    func_80066604(packet, &rect);
                    oldPacket = packet;
                    asm("addiu %0,%0,12" : "=r"(packet) : "0"(packet));
                    func_80064DDC(otReg, oldPacket);
                    *scratch = packet;
                }
            }
        }
    }
}
