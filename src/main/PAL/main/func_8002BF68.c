#include "common.h"

extern s32 D_801E408C;
extern s32 *D_801E4150;
extern s32 D_8009E6A8;
extern u8 *D_8009E688;

void func_8002BF68(u8 *arg0, s32 arg1) {
    register u8 *obj asm("$4") = arg0;
    register s32 state asm("$2") = D_801E408C;
    register s32 cur asm("$9") = *(s32 *)(obj + 0x30);
    register s32 total asm("$7") = 0;
    register s32 index asm("$6");

    *(s32 *)(obj + 0x68) = 0;
    if (state != 0) {
        index = *D_801E4150;
        if (arg1 == 1) {
            register s32 count asm("$5");
            register u8 *table asm("$8");
            s32 wrapped;

            count = D_8009E6A8;
            table = D_8009E688;
advance_forward_add:
            index++;
            wrapped = index % count;
            if (cur == wrapped) {
                goto done;
            }
            total += *(s16 *)(table + (wrapped * 24) + 0x16);
            goto advance_forward_add;
        } else {
            register s32 count asm("$8");
            register u8 *table asm("$10");
            register s32 wrapped asm("$5");
            s32 mod;

            count = D_8009E6A8;
            table = D_8009E688;
advance_backward_sub:
            if (index < 0) {
                wrapped = index + count;
            } else {
                wrapped = index;
            }
            mod = wrapped % count;
            total -= *(s16 *)(table + (mod * 24) + 0x16);
            if (cur == wrapped) {
                goto done;
            }
            index--;
            goto advance_backward_sub;
        }
    } else {
        index = *D_801E4150;
        if (arg1 == 0) {
            register s32 count asm("$5");
            register u8 *table asm("$8");
            s32 wrapped;

            count = D_8009E6A8;
            table = D_8009E688;
advance_forward_sub:
            do {
                index++;
                wrapped = index % count;
                total -= *(s16 *)(table + (wrapped * 24) + 0x16);
            } while (cur != wrapped);
            goto done;
        } else {
            register s32 count asm("$8");
            register u8 *table asm("$10");
            register s32 wrapped asm("$5");
            s32 mod;

            count = D_8009E6A8;
            table = D_8009E688;
advance_backward_add:
            do {
                if (index < 0) {
                    wrapped = index + count;
                } else {
                    wrapped = index;
                }
                if (cur == wrapped) {
                    break;
                }
                mod = wrapped % count;
                total += *(s16 *)(table + (mod * 24) + 0x16);
                index--;
            } while (1);
        }
    }
done:
    *(s32 *)(obj + 0x68) = total;
}
