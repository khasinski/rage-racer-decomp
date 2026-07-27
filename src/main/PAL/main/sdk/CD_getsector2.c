#include "common.h"
#include "psyq/cd.h"

extern volatile u_char *D_80099300;
extern volatile u_char *D_8009930C;
extern volatile u_long *D_80099310;
extern volatile u_long *D_80099334;
extern volatile u_long *D_80099338;
extern volatile u_long *D_8009933C;
extern volatile u_long *D_80099340;
extern volatile u_long *D_80099344;
extern long D_800992E4;

typedef void (*Callback)(long, void *);

extern Callback D_8009903C;
extern Callback D_80099040;
extern u_char D_80099318;
extern u_char D_80099319;
extern u_char D_8009BAF0[];
extern u_char D_8009BAF8[];

long func_8006AB5C(void);

long CD_getsector2(long arg0, u_long arg1) {
    volatile u_char *status;
    volatile u_long *temp_v1;
    volatile u_long *temp_a0;

    *D_80099300 = 0;
    *D_8009930C = 0x80;
    *D_80099334 = 0x20943;
    *D_80099310 = 0x1323;

    temp_v1 = D_80099338;
    *temp_v1 |= 0x8000;

    *D_8009933C = arg0;
    *D_80099340 = arg1 | 0x10000;

    status = D_80099300;
    while ((*status & 0x40) == 0) {
    }

    *D_80099344 = 0x11000000;

    temp_a0 = D_80099344;
    if ((*temp_a0 & 0x1000000) != 0) {
        temp_v1 = temp_a0;
        do {
        } while ((*temp_v1 & 0x1000000) != 0);
    }

    *D_80099310 = 0x1325;
    return 0;
}

void func_8006C16C(long arg0) {
    D_800992E4 = arg0;
}

void func_8006C17C(void) {
    register u_char *statusByte asm("$18");
    long status;
    long saved;

    saved = *D_80099300 & 3;
    statusByte = &D_80099319;

    while ((status = func_8006AB5C()) != 0) {
        if ((status & 4) != 0) {
            if (D_80099040 != 0) {
                D_80099040(*statusByte, D_8009BAF8);
            }
        }

        if ((status & 2) != 0) {
            register Callback doneCallback asm("$3");
            register u_char *resultByte asm("$2");

            doneCallback = D_8009903C;
            if (doneCallback != 0) {
                resultByte = &D_80099318;
                asm volatile("" : "=r"(resultByte) : "0"(resultByte));
                doneCallback(*resultByte, D_8009BAF0);
            }
        }
    }

    *D_80099300 = saved;
}
