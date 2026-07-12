#include "common.h"
#include "game/asset.h"
#include "game/car.h"

extern u32 D_8009E87C;
extern GameCarEntry *D_8019C7C8;
extern u8 *D_801E4090;

s32 func_80017848(s32 arg0, s32 arg1);
s32 func_80017C78(s32 arg0, void *arg1);
void func_80017B94(void *arg0, s32 arg1);
void func_80017948(void *arg0, s32 arg1);
void func_80017B44(void *arg0, s32 arg1);
void func_8001D748(s32 arg0, s32 arg1);
void func_8001D900(s32 arg0, s32 arg1);

void func_800188B8(s32 arg0) {
    register u8 *ptr asm("$16");
    register s32 index asm("$18");
    register s32 arg asm("$17");
    s32 offset;

    arg = arg0;
    index = arg << 3;
    offset = (func_80017848(arg, ((GameCarEntry *)(index + (s32)D_8019C7C8))->modelVariant) * 2) + 0xA;

    if (g_AssetLoadState == 1) {
        ptr = D_801E4090;
        if (D_8009E87C == 0) {
            ptr += 0x20000;
        }

        if (func_80017C78(offset, ptr) != 0) {
            register s32 fixed asm("$4");
            register u32 flag asm("$5");
            register s32 test asm("$2");
            register u8 *entry asm("$2");

            func_80017B94(ptr, D_8009E87C < 1);
            fixed = *(volatile s32 *)(ptr + 0x20);
            flag = D_8009E87C;
            fixed = (s32)ptr + fixed;
            flag = flag < 1;
            *(s32 *)(ptr + 0x20) = fixed;
            func_80017948((void *)fixed, flag);
            fixed = *(volatile s32 *)(ptr + 0x24);
            flag = D_8009E87C;
            fixed = (s32)ptr + fixed;
            flag = flag < 1;
            *(s32 *)(ptr + 0x24) = fixed;
            func_80017B44((void *)fixed, flag);

            test = arg < 10;
            if (test != 0) {
                entry = (u8 *)(index + (s32)D_8019C7C8);
                func_8001D748(((GameCarEntry *)entry)->shapeIndex, *(s32 *)(ptr + 0x24));
                entry = (u8 *)(index + (s32)D_8019C7C8);
                func_8001D900(((GameCarEntry *)entry)->textureIndex, *(s32 *)(ptr + 0x24));
            }

            g_AssetLoadState = 0;
        }
    }
}
