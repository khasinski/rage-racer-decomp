#include "common.h"
#include "game/race.h"

extern s32 D_801E4150;
extern s32 D_801E408C;
extern s32 D_801E40D8;

typedef struct {
    s32 start;
    s32 end;
    u16 unk8;
    u16 flags;
} UnkFunc80040DB4Entry;

void func_8005C31C(s32 arg0, s32 arg1, s32 arg2);

void func_80040DB4(s32 arg0) {
    register s32 position asm("$9");
    register s32 base asm("$10");
    register s32 mode asm("$4");
    s32 value;
    register s32 finalValue asm("$5");
    register s32 i asm("$7");
    register s32 sentinel asm("$11");
    register s32 entryBaseValue asm("$2");
    register UnkFunc80040DB4Entry *entryBase asm("$6");
    register UnkFunc80040DB4Entry *entry asm("$3");
    register s32 selector asm("$4");

    position = arg0;
    selector = g_GrandPrixClass;
    entryBaseValue = D_801E4150;
    entryBase = (UnkFunc80040DB4Entry *)(entryBaseValue + 0x1C9C);
    selector = selector % 5;

    switch (selector) {
    default:
    case 0:
    case 5:
        base = 0;
        mode = 0;
        break;
    case 1:
    case 2:
        base = 0x60;
        mode = 0;
        break;
    case 3:
    case 4:
        base = 0x60;
        mode = 1;
        break;
    }

    if (D_801E408C != 0) {
        position = D_801E40D8 - position;
    }

    value = 0;
    i = 0;
    sentinel = -1;
    entry = entryBase;
    while (i < 4) {
        register s32 start asm("$6");
        register s32 end asm("$8");

        start = entry->start;
        end = entry->end;
        if (start == sentinel) {
            break;
        }
        if (!(position < start) && !(end < position)) {
            register s32 flags asm("$3");
            register s32 delta asm("$2");
            register s32 product asm("$3");
            register s32 flagBit asm("$2");

            flags = entry->flags;
            if (position < start + 0x320 && (flags & 1) > 0) {
                delta = position - start;
                product = base * delta;
                value = product / 800;
            } else if (end - 0x320 < position && ((flagBit = flags & 2), flagBit > 0)) {
                delta = end - position;
                product = base * delta;
                value = product / 800;
            } else {
                value = base;
            }
            break;
        }
        i++;
        entry++;
    }

    finalValue = value;
    finalValue = (s16)finalValue;
    asm("" : "=r"(finalValue) : "0"(finalValue));
    func_8005C31C(mode, finalValue, finalValue);
}
