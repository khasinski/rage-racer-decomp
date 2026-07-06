#include "common.h"

typedef struct UnkRaceList {
    s16 count;
    u8 pad2[0x1A];
    s32 values[1];
} UnkRaceList;

extern u8 *D_8019C900;
extern s32 D_801E4364;
extern s32 D_801E4BCC;
extern UnkRaceList D_8009E83C;
extern s16 D_8009E836;

void AddPrim(void *ot, void *prim) asm("func_80064DDC");
void func_80033D50(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

void func_80033090(void) {
    register s32 i __asm("$17");
    register s32 visibleCount __asm("$22");
    register s32 activeIndex __asm("$23");
    register s32 tile __asm("$16");
    register s32 y __asm("$19");
    register s32 primOffset __asm("$20");
    register s32 baseOffset __asm("$21");
    register s32 *valuePtr __asm("$18");
    register UnkRaceList *list __asm("$3");
    register u8 *base __asm("$2");
    register void *ot __asm("$4");
    register void *prim __asm("$5");
    s32 framePad[2];
    s32 value;

    list = &D_8009E83C;
    visibleCount = list->count;
    if (visibleCount > D_801E4364) {
        visibleCount = D_801E4364;
    }

    i = 0;
    activeIndex = D_8009E836;
    asm volatile("" : "=m"(framePad[0]), "=m"(framePad[1]));
    if (D_801E4364 > 0) {
        baseOffset = 0x236F8;
        primOffset = 0;
        y = 0x2E;
        valuePtr = list->values;

        do {
            if (i == activeIndex) {
                tile = 0x780F;
            } else if (valuePtr[0] > 0x927BE) {
                tile = 0x7890;
            } else {
                tile = 0x78CC;
            }

            if (i < visibleCount) {
                value = valuePtr[0];
            } else {
                value = -1;
            }

            func_80033D50(0xFA, y, value, tile, 0x3E8);
            y += 0xA;
            valuePtr++;
            asm volatile("" : "=r"(valuePtr) : "0"(valuePtr));
            base = D_8019C900;
            ot = base + 0xCC;
            prim = (void *)(baseOffset + (s32)base);
            *(s16 *)(primOffset + (s32)base + 0x23706) = tile;
            AddPrim(ot, prim);
            i++;
            baseOffset += 0x14;
            primOffset += 0x14;
        } while (i < D_801E4364);
    }

    func_80033D50(0xFA, 0x20, D_801E4BCC, 0x78CC, 0x3E8);
}
