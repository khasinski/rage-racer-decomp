#include "common.h"
#include "game/asset.h"

extern s32 D_8007C704;
extern s32 D_8009F0B8;

void func_80017E8C(void);
void func_80017E48(s32 arg0, void *arg1);
void func_8001A3C0(void *arg0);

void func_80018038(void) {
    void *ptr;

    func_80017E8C();
    ptr = &D_8009F0B8;
    func_80017E48(0, ptr);
    func_8001A3C0(ptr);
}

s32 func_80018078(void) {
    if (g_AssetLoadState != 0) {
        return 1;
    }

    if (D_8007C704 == 1) {
        D_8007C704 = 0;
        return 0;
    }

    D_8007C704 = 1;
    g_AssetLoadState = 1;
    return 1;
}
