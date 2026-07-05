#include "common.h"

extern s32 D_8009E858[];
extern s32 D_801E428C;
extern s16 D_8019CABC;
extern s32 D_8019CAC8;
extern s32 D_801E8A48;
extern s32 D_8019CE10;
extern volatile s32 D_801E4BA8;
extern volatile u16 D_801E40D4;
extern s32 D_801E7744[];
extern s32 D_801E774C;
extern u16 D_801E7750[];
extern s32 D_8019CB78[];
extern s32 D_8019CB80;
extern u16 D_8019CB84[];
extern volatile u8 D_801E417C[];
extern volatile u8 D_801F17FC[];

void func_80022324(void) {
    s32 count;
    register s32 i asm("$8");
    register s32 row_offset asm("$9");
    register s32 best asm("$10");
    register s32 j asm("$7");
    register s32 *score_ptr asm("$4");
    register s32 entry_addr asm("$5");
    s32 fill_offset;
    register s32 fill_addr asm("$3");
    s32 copy0;
    s32 copy1;
    s32 copy2;
    s32 copy3;
    register s32 score_offset asm("$3");
    s32 score_value;
    s32 *entry;
    register s32 mode asm("$4");
    register s32 base_addr asm("$2");
    register s32 course_addr asm("$3");
    register u8 *name_base asm("$11");
    register u8 *name_base2 asm("$10");
    register s32 letter asm("$13");
    register s32 code asm("$12");
    register s32 letter2 asm("$12");
    register s32 code2 asm("$11");

    count = 3;
    if (D_801E428C == 3) {
        count = 6;
    }

    best = 0x927C0;
    i = 0;
    if (i < count) {
        score_ptr = D_8009E858;
        while (i < count) {
            if (*score_ptr < best) {
                best = *score_ptr;
                D_8019CAC8 = i;
            }
            i++;
            score_ptr++;
        }
    }

    i = 0;
    name_base = (u8 *)D_801E7744;
    letter = 0x41;
    code = 0xB;
    row_offset = 0;
    while (i < 5) {
        score_offset = row_offset + (D_801E428C * 0x50);
        score_offset += D_8019CABC * 0x140;
        if (best < *((s32 *)((u8 *)&D_801E774C + score_offset))) {
            if (i < 4) {
                j = 4;
                do {
                    entry_addr = j * 0x10;
                    j--;
                    mode = D_801E428C;
                    course_addr = (D_8019CABC * 0x140) + (s32)name_base;
                    base_addr = (mode * 0x50) + course_addr;
                    entry_addr += base_addr;
                    entry = (s32 *)entry_addr;
                    asm volatile("" : : "r"(j));
                    copy0 = entry[-4];
                    copy1 = entry[-3];
                    copy2 = entry[-2];
                    copy3 = entry[-1];
                    entry[0] = copy0;
                    entry[1] = copy1;
                    entry[2] = copy2;
                    entry[3] = copy3;
                    asm volatile("" : : : "memory");
                } while (i < j);
            }
            score_offset = row_offset + (D_801E428C * 0x50);
            score_offset += D_8019CABC * 0x140;
            *((s32 *)((u8 *)&D_801E774C + score_offset)) = best;
            j = 0;
            fill_offset = row_offset;
            for (; j < 6; j++) {
                fill_addr = fill_offset + (D_801E428C * 0x50);
                fill_addr += D_8019CABC * 0x140;
                *((volatile u8 *)((fill_addr + (s32)name_base) + j)) = letter;
                D_801E417C[j] = code;
            }

            score_offset = row_offset + (D_801E428C * 0x50);
            score_offset += D_8019CABC * 0x140;
            *((u16 *)((u8 *)D_801E7750 + score_offset)) = D_801E40D4;
            break;
        }
        i++;
        row_offset += 0x10;
    }

    D_801E8A48 = i;
    i = 0;
    name_base2 = (u8 *)D_8019CB78;
    letter2 = 0x41;
    code2 = 0xB;
    row_offset = 0;
    while (i < 5) {
        score_offset = row_offset + (D_801E428C * 0x50);
        score_offset += D_8019CABC * 0x140;
        score_value = *((s32 *)((u8 *)&D_8019CB80 + score_offset));
        if (D_801E4BA8 < score_value) {
            if (i < 4) {
                j = 4;
                do {
                    entry_addr = j * 0x10;
                    j--;
                    mode = D_801E428C;
                    course_addr = (D_8019CABC * 0x140) + (s32)name_base2;
                    base_addr = (mode * 0x50) + course_addr;
                    entry_addr += base_addr;
                    entry = (s32 *)entry_addr;
                    asm volatile("" : : "r"(j));
                    copy0 = entry[-4];
                    copy1 = entry[-3];
                    copy2 = entry[-2];
                    copy3 = entry[-1];
                    entry[0] = copy0;
                    entry[1] = copy1;
                    entry[2] = copy2;
                    entry[3] = copy3;
                    asm volatile("" : : : "memory");
                } while (i < j);
            }
            score_offset = row_offset + (D_801E428C * 0x50);
            score_offset += D_8019CABC * 0x140;
            *((s32 *)((u8 *)&D_8019CB80 + score_offset)) = D_801E4BA8;
            j = 0;
            fill_offset = row_offset;
            for (; j < 6; j++) {
                fill_addr = fill_offset + (D_801E428C * 0x50);
                fill_addr += D_8019CABC * 0x140;
                *((volatile u8 *)((fill_addr + (s32)name_base2) + j)) = letter2;
                D_801F17FC[j] = code2;
            }

            score_offset = row_offset + (D_801E428C * 0x50);
            score_offset += D_8019CABC * 0x140;
            *((u16 *)((u8 *)D_8019CB84 + score_offset)) = D_801E40D4;
            break;
        }
        i++;
        row_offset += 0x10;
    }

    D_8019CE10 = i;
}
