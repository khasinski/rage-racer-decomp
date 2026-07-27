#include <sys/types.h>

#include "common.h"

typedef struct {
    void (*cb)(long, long);
    long arg;
    long tag;
    long params[21];
} QEntry;

typedef struct {
    void (*cb)(long, long);
    long arg;
    long tag;
} LastCb;

extern volatile QEntry D_801E5024[];
extern volatile long D_800942EC;
extern volatile long D_800942F0;
extern long D_800942F8;
extern volatile LastCb D_800942DC;
extern volatile u_long *D_800942C8;
extern volatile u_long *D_800942BC;
extern u_char D_800941F0[];
extern volatile long D_800941F4;

extern long func_8006E0B0(long);
extern void DMACallback(long, void *) asm("func_8006DF94");

/* Driver-table slot +0x24, and the DMA2 completion callback: drains the
 * Gpu_AddQueue ring, then fires the DrawSyncCallback when it empties.
 * Returns the number of entries still queued. */
long Gpu_ExecuteQueue(void) asm("func_80067984");
long Gpu_ExecuteQueue(void) {
    if (*D_800942C8 & 0x01000000) {
        return 1;
    }

    D_800942F8 = func_8006E0B0(0);

    if (D_800942EC != D_800942F0) {
        while ((*D_800942C8 & 0x01000000) == 0) {
            if ((((D_800942F0 + 1) & 0x3f) == D_800942EC) &&
                (*(volatile long *)(u_char *)&D_800941F4 == 0)) {
                DMACallback(2, 0);
            }

            while ((*D_800942BC & 0x04000000) == 0) {
                ;
            }

            D_801E5024[D_800942F0].cb(D_801E5024[D_800942F0].arg,
                                      D_801E5024[D_800942F0].tag);
            D_800942DC.cb = D_801E5024[D_800942F0].cb;
            D_800942DC.arg = D_801E5024[D_800942F0].arg;
            D_800942DC.tag = D_801E5024[D_800942F0].tag;

            D_800942F0 = (D_800942F0 + 1) & 0x3f;
            if (D_800942EC == D_800942F0) {
                break;
            }
        }
    }

    func_8006E0B0(D_800942F8);

    if ((D_800942EC == D_800942F0) && ((*D_800942C8 & 0x01000000) == 0) &&
        (*(volatile long *)D_800941F0 != 0) && (D_800941F4 != 0)) {
        *(volatile long *)D_800941F0 = 0;
        ((void (*)(void))D_800941F4)();
    }

    return (D_800942EC - D_800942F0) & 0x3f;
}
