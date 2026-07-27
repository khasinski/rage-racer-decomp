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
extern u_char D_8009BB00[];
extern long D_8009BB08;
extern long D_8009BB0C;
extern char *D_8009BB10;
extern char D_80013814[];
extern char D_80013824[];
extern char D_800138A4[];

void func_80063C38(char *text);
long func_8006AB5C(void);
void func_8006BAF0(void);
long VSync(long mode) asm("func_8006DD30");
long func_8006E088(void);

static __inline__ void copy8(u_char *dst, u_char *src) {
    long count;
    long end;

    if (dst == 0) {
        return;
    }

    count = 7;
    end = -1;
    do {
        *dst++ = *src++;
    } while (--count != end);
}

long CD_ready(long mode, u_char *result) asm("func_8006B354");
long CD_ready(long mode, u_char *result) {
    long savedStatus;
    long interrupt;
    long status;
    long alarmStatus;

    D_8009BB08 = VSync(-1) + 0x3C0;
    D_8009BB0C = 0;
    D_8009BB10 = D_800138A4;

    for (;;) {
        if (D_8009BB08 < VSync(-1) ||
            D_8009BB0C++ > 0x3C0000) {
            func_80063C38(D_80013814);
            GameDebugPrintf(D_80013824, ((CdAlarm *)&D_8009BB08)->name,
                          D_80099060[D_8009905D],
                          D_800990E0[D_80099318.sync],
                          D_800990E0[D_80099318.ready]);
            func_8006BAF0();
            alarmStatus = -1;
        } else {
            alarmStatus = 0;
        }

        if (alarmStatus != 0) {
            return -1;
        }

        if (func_8006E088()) {
            savedStatus = *D_80099300 & 3;
            while ((interrupt = func_8006AB5C()) != 0) {
                if ((interrupt & 4) != 0 && D_80099040 != 0) {
                    D_80099040(D_80099318.ready, D_8009BAF8);
                }
                if ((interrupt & 2) != 0 && D_8009903C != 0) {
                    D_8009903C(D_80099318.sync, D_8009BAF0);
                }
            }
            *D_80099300 = savedStatus;
        }

        status = D_80099318.command;
        if (status != 0) {
            D_80099318.command = 0;
            copy8(result, D_8009BB00);
            return status;
        }

        status = D_80099318.ready;
        if (status != 0) {
            D_80099318.ready = 0;
            copy8(result, D_8009BAF8);
            return status;
        }

        if (mode != 0) {
            return 0;
        }
    }
}
