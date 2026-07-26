#include "common.h"

typedef struct {
    u8 sync;
    u8 ready;
    u8 command;
} CdState;

typedef struct {
    s32 deadline;
    s32 count;
    char *name;
} CdAlarm;

typedef void (*CdCallback)(u8 status, u8 *result);

extern CdCallback D_8009903C;
extern CdCallback D_80099040;
extern s32 D_80099048;
extern u8 D_80099058[4];
extern u8 D_8009905C;
extern u8 D_8009905D;
extern char *D_80099060[];
extern char *D_800990E0[];
extern s32 D_80099180[];
extern s32 D_80099280[];
extern volatile u8 *D_80099300;
extern volatile u8 *D_80099304;
extern volatile u8 *D_80099308;
extern volatile CdState D_80099318;
extern u8 D_8009BAF0[8];
extern u8 D_8009BAF8[8];
extern CdAlarm D_8009BB08;
extern char D_80013814[];
extern char D_80013824[];
extern char D_800138B0[];
extern char D_800138B8[];
extern char D_800138C8[];

void func_80063C38(char *text);
s32 func_8006AB5C(void);
s32 func_8006B0D4(s32 mode, u8 *result);
void func_8006BAF0(void);
s32 VSync(s32 mode) asm("func_8006DD30");
s32 func_8006E088(void);

static inline void setAlarm(char *name) {
    D_8009BB08.deadline = VSync(-1) + 0x3C0;
    D_8009BB08.count = 0;
    D_8009BB08.name = name;
}

static inline s32 getAlarm(void) {
    if (D_8009BB08.deadline < VSync(-1) ||
        D_8009BB08.count++ > 0x3C0000) {
        func_80063C38(D_80013814);
        GameDebugPrintf(D_80013824, D_8009BB08.name, D_80099060[D_8009905D],
                      D_800990E0[D_80099318.sync], D_800990E0[D_80099318.ready]);
        func_8006BAF0();
        return -1;
    }
    return 0;
}

s32 CD_cw(u8 command, u8 *params, u8 *result, s32 async) asm("func_8006B620");
s32 CD_cw(u8 command, u8 *params, u8 *result, s32 async) {
    s32 i;
    s32 interrupt;
    s32 remaining;
    u8 interruptState;
    u8 *destination;
    u8 *source;

    if (D_80099048 >= 2) {
        GameDebugPrintf(D_800138B0, D_80099060[command]);
    }

    if (D_80099280[command] != 0 && params == 0) {
        if (D_80099048 > 0) {
            GameDebugPrintf(D_800138B8, D_80099060[command]);
        }
        return -2;
    }

    func_8006B0D4(0, 0);

    if (command == 2) {
        for (i = 0; i < 4; i++) {
            D_80099058[i] = params[i];
        }
    }

    D_80099318.sync = 0;
    if (D_80099180[command] != 0) {
        D_80099318.ready = 0;
    }
    *D_80099300 = 0;

    for (i = 0; i < D_80099180[0x40 + command]; i++) {
        *D_80099308 = params[i];
    }

    D_8009905D = command;
    *D_80099304 = command;

    if (async != 0) {
        return 0;
    }

    setAlarm(D_800138C8);

    while (D_80099318.sync == 0) {
        if (getAlarm() != 0) {
            return -1;
        }

        if (func_8006E088() != 0) {
            interruptState = *D_80099300 & 3;
            while ((interrupt = func_8006AB5C()) != 0) {
                if ((interrupt & 4) != 0 && D_80099040 != 0) {
                    D_80099040(D_80099318.ready, D_8009BAF8);
                }
                if ((interrupt & 2) != 0 && D_8009903C != 0) {
                    D_8009903C(D_80099318.sync, D_8009BAF0);
                }
            }
            *D_80099300 = interruptState;
        }
    }

    destination = result;
    if (D_80099318.sync == 2 && command == 0xE) {
        D_8009905C = *params;
    }

    source = D_8009BAF0;
    if (destination != 0) {
        remaining = 7;
        do {
            *destination++ = *source++;
            remaining--;
        } while (remaining != -1);
    }

    return -(D_80099318.sync == 5);
}
