#include "common.h"
#include "psyq/kernel.h"

typedef void (*Callback)(void);

extern u_short D_80099430[];
extern u_short D_80099432;
extern u_short D_80099460;
extern volatile u_short *D_8009A4BC;
extern volatile u_short *D_8009A4C0;
extern long D_8009A4C8;
extern u_char D_80013B70[];
extern u_char D_80013B8C[];

void func_8006E654(void);

void intrDispatch(void) {
    u_short *state;
    u_short s0;
    long i;
    long c;
    long one;
    Callback *p;
    Callback *base;

    state = D_80099430;
    if (state[0] == 0) {
        GameDebugPrintf(D_80013B70, *D_8009A4BC);
        func_8006E654();
    }
    D_80099432 = 1;
    s0 = (D_80099460 & *D_8009A4BC) & *D_8009A4C0;
    if (s0 != 0) {
        one = 1;
        base = (Callback *)&state[2];
        do {
            i = 0;
            if (s0 != 0) {
                p = base;
                while ((s0 != 0) && (i < 0xB)) {
                    if (s0 & 1) {
                        *D_8009A4BC = ~(one << i);
                        if (*p != 0) {
                            (*p)();
                        }
                    }
                    p++;
                    s0 >>= 1;
                    i++;
                }
            }
            s0 = (D_80099460 & *D_8009A4BC) & *D_8009A4C0;
        } while (s0 != 0);
    }
    if ((*D_8009A4BC & *D_8009A4C0) != 0) {
        c = D_8009A4C8;
        D_8009A4C8 = c + 1;
        if (c >= 0x801) {
            GameDebugPrintf(D_80013B8C, *D_8009A4BC, *D_8009A4C0);
            D_8009A4C8 = 0;
            *D_8009A4BC = 0;
        }
    } else {
        D_8009A4C8 = 0;
    }
    D_80099432 = 0;
    func_8006E654();
}
