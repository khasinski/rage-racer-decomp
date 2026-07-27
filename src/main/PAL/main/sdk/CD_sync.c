#include <sys/types.h>

#include "common.h"

typedef void (*CdCallback)(long, u_char *);

typedef struct CdIntr {
    u_char sync;
    u_char ready;
    u_char command;
} CdIntr;

typedef struct CdAlarm {
    long deadline;
    long count;
    char *name;
} CdAlarm;

extern char *D_80099060[];
extern char *D_800990E0[];
extern CdCallback D_8009903C;
extern CdCallback D_80099040;
extern u_char D_8009905D;
extern volatile u_char *D_80099300;
extern volatile CdIntr D_80099318;
extern u_char D_8009BAF0[];
extern u_char D_8009BAF8[];
extern long D_8009BB08;
extern long D_8009BB0C;
extern char *D_8009BB10;
extern char D_80013814[];
extern char D_80013824[];
extern char D_8001389C[];

void func_80063C38(char *text);
long func_8006AB5C(void);
void func_8006BAF0(void);
long VSync(long mode) asm("func_8006DD30");
long func_8006E088(void);

static __inline__ void copy8(u_char *dst, u_char *src) {
    register u_char *dstReg asm("$5");
    register u_char *srcReg asm("$4");
    register long count asm("$3");
    register long end asm("$7");

    dstReg = dst;
    srcReg = src;

    if (dstReg == 0) {
        return;
    }

    count = 7;
    end = -1;
    do {
        *dstReg++ = *srcReg++;
    } while (--count != end);
}

long CD_sync(long mode, u_char *result) asm("func_8006B0D4");
long CD_sync(long mode, u_char *result) {
    register long modeReg asm("$21");
    register u_char *resultReg asm("$22");
    register char **statusNames asm("$19");
    register volatile CdIntr *intr asm("$18");
    register u_char *ready asm("$20");
    register long savedStatus asm("$17");
    register long interrupt asm("$16");
    register long alarmStatus asm("$2");

    modeReg = mode;
    asm("" : "=r"(modeReg) : "0"(modeReg));
    resultReg = result;
    asm("" : "=r"(resultReg) : "0"(resultReg));
    D_8009BB08 = VSync(-1) + 0x3C0;
    statusNames = D_800990E0;
    intr = &D_80099318;
    ready = (u_char *)&intr->ready;
    D_8009BB0C = 0;
    D_8009BB10 = D_8001389C;

    for (;;) {
        if (D_8009BB08 < VSync(-1) ||
            D_8009BB0C++ > 0x3C0000) {
            func_80063C38(D_80013814);
            GameDebugPrintf(D_80013824, ((CdAlarm *)&D_8009BB08)->name,
                          D_80099060[D_8009905D],
                          statusNames[intr->sync], statusNames[intr->ready]);
            func_8006BAF0();
            alarmStatus = -1;
        } else {
            alarmStatus = 0;
        }

        if (alarmStatus != 0) {
            return -1;
        }

        if (func_8006E088()) {
            register u_long rawStatus asm("$2");

            rawStatus = *D_80099300;
            asm("" : "=r"(rawStatus) : "0"(rawStatus));
            savedStatus = rawStatus & 3;
            for (;;) {
                register long readyBit asm("$2");
                register long syncBit asm("$2");

                interrupt = func_8006AB5C();
                if (interrupt == 0) {
                    break;
                }
                readyBit = interrupt & 4;
                if (readyBit != 0) {
                    if (D_80099040 != 0) {
                        D_80099040(*ready, D_8009BAF8);
                    }
                    asm("");
                    syncBit = interrupt & 2;
                } else {
                    syncBit = interrupt & 2;
                }
                if (syncBit && D_8009903C != 0) {
                    D_8009903C(intr->sync, D_8009BAF0);
                }
            }
            *D_80099300 = savedStatus;
        }

        {
        register long sync asm("$6");
        register u_long syncRaw asm("$2");

        syncRaw = intr->sync;
        asm("" : "=r"(syncRaw) : "0"(syncRaw));
        sync = syncRaw & 0xFF;
        if (sync == 2 || sync == 5) {
            intr->sync = 2;
            copy8(resultReg, (u_char *)D_8009BAF0);
            asm(".globl func_8006B330\nfunc_8006B330 = CD_sync + 0x25c");
            return sync;
        }
        }

        if (modeReg != 0) {
            return 0;
        }
    }
}
