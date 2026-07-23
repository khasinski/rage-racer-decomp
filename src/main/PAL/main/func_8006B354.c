#include "common.h"

typedef void (*CdCallback)(s32, u8 *);

typedef struct CdIntr {
    u8 sync;
    u8 ready;
    u8 command;
} CdIntr;

typedef struct CdAlarm {
    s32 deadline;
    s32 count;
    char *name;
} CdAlarm;

extern char *D_80099060[];
extern char *D_800990E0[];
extern CdCallback D_8009903C;
extern CdCallback D_80099040;
extern u8 D_8009905D;
extern volatile u8 *D_80099300;
extern volatile CdIntr D_80099318;
extern u8 D_8009BAF0[];
extern u8 D_8009BAF8[];
extern u8 D_8009BB00[];
extern s32 D_8009BB08;
extern s32 D_8009BB0C;
extern char *D_8009BB10;
extern char D_80013814[];
extern char D_80013824[];
extern char D_800138A4[];

s32 func_8001674C(char *format, ...);
void func_80063C38(char *text);
s32 func_8006AB5C(void);
void func_8006BAF0(void);
s32 func_8006DD30(s32 mode);
s32 func_8006E088(void);

static __inline__ void copy8(u8 *dst, u8 *src) {
    s32 count;
    s32 end;

    if (dst == 0) {
        return;
    }

    count = 7;
    end = -1;
    do {
        *dst++ = *src++;
    } while (--count != end);
}

s32 func_8006B354(s32 mode, u8 *result) {
    s32 savedStatus;
    s32 interrupt;
    s32 status;
    s32 alarmStatus;

    D_8009BB08 = func_8006DD30(-1) + 0x3C0;
    D_8009BB0C = 0;
    D_8009BB10 = D_800138A4;

    for (;;) {
        if (D_8009BB08 < func_8006DD30(-1) ||
            D_8009BB0C++ > 0x3C0000) {
            func_80063C38(D_80013814);
            func_8001674C(D_80013824, ((CdAlarm *)&D_8009BB08)->name,
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
