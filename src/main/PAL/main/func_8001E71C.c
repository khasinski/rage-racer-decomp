#include "common.h"

extern s32 D_8009F094;
extern u32 D_8019C904;

void func_8001E79C(u32 arg0);
void func_8001E8A4(void);
void func_8001EA34(void);

void func_8001E71C(void) {
    s32 state = D_8009F094;

    if (state == 1) {
        goto state_1;
    }
    if (state < 2) {
        if (state == 0) {
            goto state_0;
        }
        goto done;
    }
    if (state == 2) {
        goto state_2;
    }
    goto done;

state_0:
    func_8001E79C(D_8019C904);
state_1:
    func_8001E8A4();
    goto done;

state_2:
    func_8001EA34();

done:
}
