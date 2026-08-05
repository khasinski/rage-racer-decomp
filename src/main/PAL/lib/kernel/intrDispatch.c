#include "common.h"
#include "psyq/kernel.h"

typedef void (*Callback)(void);

/* The kernel interrupt block at 0x80099430: [0] installed flag,
 * [1] g_IntrInDispatch, [2..] g_IntrCallbacks (11 handler slots),
 * +0x30 g_IntrCallbackMask (which slots are filled), +0x38 the saved
 * frame HookEntryInt is pointed at. */
extern u_short g_IntrState[];
extern u_short g_IntrInDispatch;
extern u_short g_IntrCallbackMask;
extern volatile u_short *g_IrqStatus;
extern volatile u_short *g_IrqMask;
extern long g_IntrStuckCount;
extern u_char D_80013B70[];
extern u_char D_80013B8C[];

void intrDispatch(void) {
    u_short *state;
    u_short s0;
    long i;
    long c;
    long one;
    Callback *p;
    Callback *base;

    state = g_IntrState;
    if (state[0] == 0) {
        DebugPrintf(D_80013B70, *g_IrqStatus);
        ReturnFromException();
    }
    g_IntrInDispatch = 1;
    s0 = (g_IntrCallbackMask & *g_IrqStatus) & *g_IrqMask;
    if (s0 != 0) {
        one = 1;
        base = (Callback *)&state[2];
        do {
            i = 0;
            if (s0 != 0) {
                p = base;
                while ((s0 != 0) && (i < 0xB)) {
                    if (s0 & 1) {
                        *g_IrqStatus = ~(one << i);
                        if (*p != 0) {
                            (*p)();
                        }
                    }
                    p++;
                    s0 >>= 1;
                    i++;
                }
            }
            s0 = (g_IntrCallbackMask & *g_IrqStatus) & *g_IrqMask;
        } while (s0 != 0);
    }
    if ((*g_IrqStatus & *g_IrqMask) != 0) {
        c = g_IntrStuckCount;
        g_IntrStuckCount = c + 1;
        if (c >= 0x801) {
            DebugPrintf(D_80013B8C, *g_IrqStatus, *g_IrqMask);
            g_IntrStuckCount = 0;
            *g_IrqStatus = 0;
        }
    } else {
        g_IntrStuckCount = 0;
    }
    g_IntrInDispatch = 0;
    ReturnFromException();
}
