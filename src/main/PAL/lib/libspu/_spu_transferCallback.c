#include "psyq/spu.h"

extern volatile u_short *g_SpuRegBase asm("D_8009AB7C");
extern volatile long g_SpuTransferCallback asm("D_8009ABB0");
extern long g_SpuTransferIsRead asm("D_8009ABC8");

void DeliverEvent(long arg0, long arg1) asm("func_8006A3D8");

void _spu_transferCallback(void) {
    volatile long i;
    volatile long delay;
    u_short cnt;
    long wait;
    void (*callback)(void);

    if (g_SpuTransferIsRead == 0) {
        delay = 0xD;
        i = 0;
        while (i < 0xF0) {
            delay *= 3;
            i++;
        }

        delay = 0xD;
        i = 0;
        while (i < 0xF0) {
            delay *= 3;
            i++;
        }

        delay = 0xD;
        i = 0;
        while (i < 0xF0) {
            delay *= 3;
            i++;
        }
    }

    cnt = g_SpuRegBase[0xD5];
    g_SpuRegBase[0xD5] = cnt & 0xFFCF;

    wait = 0;
    if ((g_SpuRegBase[0xD5] & 0x30) != 0) {
        while (1) {
            wait++;
            if (wait >= 0xF01) {
                break;
            }
            if ((g_SpuRegBase[0xD5] & 0x30) == 0) {
                wait--;
                break;
            }
        }
    }

    if (g_SpuTransferCallback != 0) {
        callback = (void (*)(void))g_SpuTransferCallback;
        callback();
    } else {
        DeliverEvent(0xF0000009, 0x20);
    }
}
