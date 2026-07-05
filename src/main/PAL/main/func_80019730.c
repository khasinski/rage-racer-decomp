#include "common.h"

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
    register u8 *base_a0 asm("$4");
    u8 *base;
    s32 offset0;
    s32 offset1;

    base_a0 = D_8019C904;
    D_801F17A8 = base_a0 + *(s32 *)(base_a0 + 0);
    func_8001A3C0(D_801F17A8);

    base_a0 = D_8019C904;
    D_801F17A8 = base_a0 + *(s32 *)(base_a0 + 4);
    func_8001A3C0(D_801F17A8);

    base_a0 = D_8019C904;
    D_801F17A8 = base_a0 + *(s32 *)(base_a0 + 8);
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
