#include <sys/types.h>

#include "common.h"

typedef struct {
    void (*cb)(long, long);
    long arg;
    long tag;
    long params[21];
} QEntry;

extern QEntry D_801E5024[];
extern volatile long D_800942EC;
extern long D_800942F0;
extern long D_800942F4;
extern u_char D_800942DC[];
extern long D_800942E0;
extern long D_800942E4;
extern volatile u_long *D_800942C8;
extern volatile u_long *D_800942BC;
extern u_char D_800941F0[];
extern volatile u_char D_800941E9;
extern long D_800941F4;

extern void Gpu_ArmTimeout(void) asm("func_80067F04");
extern long Gpu_CheckTimeout(void) asm("func_80067F38");
extern void func_80067984(void);
extern long func_8006E0B0(long);
extern void DMACallback(long, void *) asm("func_8006DF94");

/* Driver-table slot +0x08, the `send` entry every libgpu call goes through:
 * runs the worker immediately when the queue is empty and the GPU idle,
 * otherwise copies `size` bytes of parameters into the 64-entry, 96-byte
 * queue at D_801E5024 and arms the DMA2 callback Gpu_ExecuteQueue. */
long Gpu_AddQueue(void (*worker)(long, long), long param, long size, long data) asm("func_800676A0");
long Gpu_AddQueue(void (*cb)(long, long), long arg, long size, long tag) {
    long i;
    long *src;
    long ret;

    Gpu_ArmTimeout();
    while ((((D_800942EC + 1) & 0x3f)) == D_800942F0) {
        if (Gpu_CheckTimeout() != 0) {
            return -1;
        }
        func_80067984();
    }

    ret = func_8006E0B0(0);
    *(volatile long *)D_800941F0 = 1;
    D_800942F4 = ret;

    if (D_800941E9 != 0) {
        if (D_800942EC != D_800942F0) goto enqueue;
        if ((*D_800942C8 & 0x01000000) != 0) goto enqueue;
        if (D_800941F4 != 0) goto enqueue;
    }

    do {
    } while ((*D_800942BC & 0x04000000) == 0);

    cb(arg, tag);
    *(void (*volatile *)(long, long))D_800942DC = cb;
    D_800942E0 = arg;
    D_800942E4 = tag;
    func_8006E0B0(D_800942F4);
    return 0;

enqueue:
    DMACallback(2, (void *)func_80067984);

    if (size != 0) {
        u_char *pbase = (u_char *)D_801E5024 + 12;
        i = 0;
        src = (long *)arg;
        while (i < size / 4) {
            *(long *)(pbase + D_800942EC * 96 + i * 4) = *src;
            src++;
            i++;
            asm("");
        }
        D_801E5024[D_800942EC].arg = (long)D_801E5024[D_800942EC].params;
    } else {
        D_801E5024[D_800942EC].arg = arg;
    }

    D_801E5024[D_800942EC].tag = tag;
    asm("");
    D_801E5024[D_800942EC].cb = cb;
    asm("");

    D_800942EC = (D_800942EC + 1) & 0x3f;
    func_8006E0B0(D_800942F4);
    func_80067984();
    return (D_800942EC - D_800942F0) & 0x3f;
}
