#include "common.h"

typedef struct {
    u8 pad0[5];
    u8 enabled;
    u8 pad6[2];
} CarEntry49B20;

typedef struct {
    u8 pad0[0xC];
    s32 progression;
} RuntimeEntry49B20;

extern s32 D_8009B33C;
extern CarEntry49B20 *D_8019C7C8;
extern s16 D_8019CA18;
extern s16 D_801E41A4;
extern s32 D_801E4B88;
extern RuntimeEntry49B20 *D_801E4FAC;

s32 func_8001785C(s32 arg0);

void func_80059320(void) {
    s32 index;
    CarEntry49B20 *entry;

    if (D_8009B33C != 0) {
        D_8019CA18 = -1;
        index = D_801E4B88 - 1;
        if (index >= 0) {
            entry = &D_8019C7C8[index];
            while (index >= 0) {
                if (entry->enabled == 0) {
                    D_8019CA18 = index;
                    break;
                }
                index--;
                entry--;
            }
        }
    } else {
        D_8019CA18 = -1;
        index = D_801E4B88 - 1;
        if (index >= 0) {
        backward_loop:
            {
                s32 value = func_8001785C(index);
                if (D_8019C7C8[index].enabled == 0) {
                    s32 progression = D_801E4FAC->progression;
                    if (progression < 4) {
                        if ((progression + 1) < value) {
                            index--;
                            goto backward_check;
                        }
                        D_8019CA18 = index;
                        goto backward_done;
                    }
                    if (progression >= value) {
                        D_8019CA18 = index;
                        goto backward_done;
                    }
                }
                index--;
            }
        backward_check:
            if (index >= 0) {
                goto backward_loop;
            }
        }
    }

backward_done:
    if (D_8009B33C != 0) {
        D_801E41A4 = -1;
        index = D_801E4B88 + 1;
        if (index < 13) {
            entry = &D_8019C7C8[index];
            while (index < 13) {
                if (entry->enabled == 0) {
                    D_801E41A4 = index;
                    break;
                }
                index++;
                entry++;
            }
        }
    } else {
        D_801E41A4 = -1;
        index = D_801E4B88 + 1;
        if (index < 13) {
        forward_loop:
            {
                s32 value = func_8001785C(index);
                if (D_8019C7C8[index].enabled == 0) {
                    s32 progression = D_801E4FAC->progression;
                    if (progression < 4) {
                        if ((progression + 1) < value) {
                            index++;
                            goto forward_check;
                        }
                        D_801E41A4 = index;
                        goto forward_done;
                    }
                    if (progression >= value) {
                        D_801E41A4 = index;
                        goto forward_done;
                    }
                }
                index++;
            }
        forward_check:
            if (index < 13) {
                goto forward_loop;
            }
        }
    }

forward_done:
    return;
}
