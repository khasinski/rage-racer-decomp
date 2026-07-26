#include "common.h"
#include "game/state.h"
#include "game/asset.h"

extern u8 *D_8019C904;
extern u8 *D_801F17A8;
extern u8 *D_801E8AB0;
extern u8 *D_801E42D0;
extern u8 *D_8019CAFC;
void func_8001A3C0(void *arg0);
void func_8001A2E0(void *arg0);
void func_8001A40C(void *arg0);
void func_80019EBC(void);

void func_80019730(void) {
    u8 *base;
    s32 offset0;
    s32 offset1;

    D_801F17A8 = D_8019C904 + *(s32 *)(D_8019C904 + 0);
    func_8001A3C0(D_801F17A8);

    D_801F17A8 = D_8019C904 + *(s32 *)(D_8019C904 + 4);
    func_8001A3C0(D_801F17A8);

    D_801F17A8 = D_8019C904 + *(s32 *)(D_8019C904 + 8);
    func_8001A2E0(D_801F17A8);

    base = D_8019C904;
    offset0 = *(s32 *)(base + 0xC);
    offset1 = *(s32 *)(base + 0x10);
    D_801F17A8 = base + offset0;
    D_801E8AB0 = base + offset1;
    func_8001A3C0(D_801F17A8);

    func_8001A40C(D_8019C904);

    D_801E42D0 = D_8019C904;
    func_8001A3C0(D_801E8AB0);
    func_80019EBC();
    D_8019CAFC = D_8019C904 + 0x38000;
}

s32 func_80019844(void) {
    if (g_AssetLoadState != 0) {
        return 1;
    }

    if (g_MainState == 0xC) {
        g_MainState = 0;
        return 0;
    }

    g_MainState = 0xC;
    g_AssetLoadState = 1;
    return 1;
}
