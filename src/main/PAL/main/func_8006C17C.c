#include "common.h"

typedef void (*Callback)(s32, void *);

extern Callback D_8009903C;
extern Callback D_80099040;
extern u8 *D_80099300;
extern u8 D_80099318;
extern u8 D_80099319;
extern u8 D_8009BAF0[];
extern u8 D_8009BAF8[];

s32 func_8006AB5C(void);

void func_8006C17C(void) {
    register u8 *statusByte asm("$18");
    s32 status;
    s32 saved;

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
            register u8 *resultByte asm("$2");

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
