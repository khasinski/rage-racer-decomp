#include "common.h"

extern s32 D_8007BED8;
extern u32 D_8009E87C;
extern u8 *D_8019C7C8;
extern s32 D_801E40D4;
extern u8 *D_801E4090;

s32 func_80017848(s32 arg0, s32 arg1);
s32 func_80017C78(s32 arg0, void *arg1);
void func_80017B94(void *arg0, s32 arg1);
void func_80017948(void *arg0, s32 arg1);
void func_80017B44(void *arg0, s32 arg1);
void func_8001D748(s32 arg0, s32 arg1);
void func_8001D900(s32 arg0, s32 arg1);

void func_80018A70(s32 arg0) {
    register u8 *ptr asm("$16");
    register s32 index asm("$17");
    u8 *entry;
    register s32 offset asm("$2");
    register s32 temp asm("$4");
    register u32 mode asm("$3");
    s32 flag;

    if (D_8007BED8 == 1) {
        index = arg0 << 3;
        entry = (u8 *)(index + (s32)D_8019C7C8);
        offset = func_80017848(arg0, entry[0] + 1) << 1;
        mode = D_8009E87C;
        ptr = D_801E4090;

        temp = offset + 0xA;
        if (mode == 0) {
            ptr += 0x20000;
        }

        if (func_80017C78(temp, ptr) != 0) {
            func_80017B94(ptr, D_8009E87C < 1);

            temp = *(s32 *)(ptr + 0x20);
            flag = D_8009E87C < 1;
            temp = (s32)ptr + temp;
            *(s32 *)(ptr + 0x20) = temp;
            func_80017948((void *)temp, flag);

            temp = *(s32 *)(ptr + 0x24);
            flag = D_8009E87C < 1;
            temp = (s32)ptr + temp;
            *(s32 *)(ptr + 0x24) = temp;
            func_80017B44((void *)temp, flag);

            if (D_801E40D4 < 10) {
                entry = (u8 *)(index + (s32)D_8019C7C8);
                func_8001D748(entry[3], *(s32 *)(ptr + 0x24));
                entry = (u8 *)(index + (s32)D_8019C7C8);
                func_8001D900(entry[4], *(s32 *)(ptr + 0x24));
            }

            D_8007BED8 = 0;
        }
    }
}
