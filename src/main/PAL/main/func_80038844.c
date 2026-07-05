#include "common.h"

extern s32 D_801E42E4;
extern s32 D_801E7740;
extern s32 D_801E408C;
extern s16 D_8019CABC;
extern u8 D_801F1854[];
extern s32 D_8007E074[];
extern s32 D_8007E0A4[];

void func_80038AB8(void *arg0);
void func_800383A8(void *arg0, s32 arg1, s32 *arg2);
void func_800385FC(void *arg0, s32 arg1, s32 *arg2);
void func_80039644(void);

void func_80038844(void) {
    register u8 *entryBase asm("s2");
    register s32 *table asm("s3");
    register s32 i asm("s0");
    register s16 *flagPtr asm("s1");
    register s32 one asm("s5");
    register s32 *cursor asm("s4");
    register s32 state asm("v1");
    register u16 track asm("v0");

    entryBase = D_801F1854;
    state = D_801E42E4;
    D_801E7740 = 3;

    if (state == 0xB) {
        table = D_8007E074;
        i = 0;
        one = 1;
        flagPtr = (s16 *)(entryBase + 0xAC);
        D_801E408C = D_8019CABC;
        cursor = table;
        do {
            track = *(u16 *)&D_801E408C;
            *(s16 *)flagPtr = 0;
            *(s16 *)(flagPtr + 6) = track;
            if (*cursor >= 0) {
                func_80038AB8(entryBase);
                *(s16 *)flagPtr = one;
                func_800383A8(entryBase, i, table);
                func_800385FC(entryBase, i, table);
            } else {
                *(s16 *)flagPtr = 0;
            }
            cursor++;
            i++;
            flagPtr = (s16 *)((u8 *)flagPtr + 0x19C);
            entryBase += 0x19C;
        } while (i < 0xB);
    } else {
        table = D_8007E0A4;
        i = 0;
        one = 1;
        flagPtr = (s16 *)(entryBase + 0xAC);
        D_801E408C = D_8019CABC;
        cursor = table;
        do {
            track = *(u16 *)&D_801E408C;
            *(s16 *)flagPtr = 0;
            *(s16 *)(flagPtr + 6) = track;
            if (*cursor >= 0) {
                func_80038AB8(entryBase);
                *(s16 *)flagPtr = one;
                func_800383A8(entryBase, i, table);
                func_800385FC(entryBase, i, table);
            } else {
                *(s16 *)flagPtr = 0;
            }
            cursor++;
            i++;
            flagPtr = (s16 *)((u8 *)flagPtr + 0x19C);
            entryBase += 0x19C;
        } while (i < 0xB);
    }

    func_80039644();
}
