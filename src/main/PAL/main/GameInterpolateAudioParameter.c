#include "common.h"

extern s16 D_80082F28[];
extern s16 D_80082F2A[];
extern s32 D_801E6CB8;
extern s32 D_801E6CBC;
extern s32 D_801E6CC4;
extern s32 D_801E6CC8[];
extern s32 D_801E446C[];

void func_8005B2F0(s32 slot, s32 tone, s32 vab_slot);
void func_8005D7D4(s32 slot, s32 left, s32 right, s32 bank, s32 mode);
void func_8005BF30(void);
void func_8005C6C0(void);
void func_8005C168(void);
void func_8005CDB0(void);
s32 func_8005D8EC(s32 parameter, s32 position, s32 bank) {
    s32 value = position;
    s32 index;
    s32 index_offset;
    s32 *base = D_801E446C;
    s32 row_offset;
    s32 bank_offset;
    s32 *base_minus;
    s32 *entry;
    s32 *scan;
    s32 *lower_position;
    s32 lower_value_base;
    s32 lower_value_indexed;
    s32 lower_value_address;
    s32 upper_value_base;
    s32 upper_value_banked;
    s32 upper_value_address;
    s32 lower_position_value;
    s32 lower_value_value;
    s32 upper_value_value;
    s32 numerator;
    s32 denominator;
    s32 raw_result;
    s32 result;

    index = 1;
    row_offset = (parameter * 9) << 3;
    bank_offset = ((((bank * 8) - bank) * 4) - bank) << 5;
    bank = row_offset + bank_offset;
    entry = (s32 *)((s32)base + bank);
    scan = entry + 1;
    while (index < 9) {
        raw_result = *scan;
        if (value < raw_result) {
            break;
        }
        index++;
        scan++;
    }

    base_minus = base - 1;
    index_offset = index << 2;
    lower_position = (s32 *)(index_offset + (s32)base_minus);
    lower_position = (s32 *)((s32)lower_position + bank);
    lower_value_base = (s32)(base + 8);
    lower_value_indexed = index_offset + lower_value_base;
    lower_value_address = lower_value_indexed + bank;
    upper_value_base = (s32)(base + 9);
    upper_value_banked = bank + upper_value_base;
    upper_value_address = index_offset + upper_value_banked;
    lower_value_value = *(s32 *)lower_value_address;
    upper_value_value = *(s32 *)upper_value_address;
    lower_position_value = *lower_position;
    numerator =
        (upper_value_value - lower_value_value) *
        (value - lower_position_value);
    denominator =
        *(s32 *)(index_offset + (s32)entry) - lower_position_value;
    raw_result = numerator / denominator + lower_value_value;

    if (raw_result >= 0) {
        result = raw_result;
        if (result >= 0x80) {
            result = 0x7F;
        }
    } else {
        result = 0;
    }

    return result;
}

void func_8005D9F8(s32 value, s32 bank) {
    s32 odd_parameter;
    s32 index;
    s32 tone_offset;
    s32 second;
    s32 scaled;
    s32 *scale_base;
    s32 *slot;
    s32 *slot_base;
    s32 first;

    value = ((value * 5) << 11) / *(scale_base = &D_801E6CC4);

    if (bank != D_801E6CBC) {
        index = 0;
        slot = scale_base + 1;
        tone_offset = 0;
        do {
            if (*slot++ != 0 &&
                *(s16 *)((s32)D_80082F28 + tone_offset) !=
                    *(s16 *)((s32)D_80082F2A + tone_offset)) {
                func_8005B2F0(index, bank, 3);
            }
            index++;
            tone_offset += 4;
        } while (index < 6);
        D_801E6CBC = bank;
    }

    index = 0;
    odd_parameter = 1;
    scale_base = (slot_base = D_801E6CC8);
    slot = scale_base;
    do {
        if (*slot != 0) {
            first = func_8005D8EC(index * 2, value, bank);
            second = func_8005D8EC(odd_parameter, value, bank);
            scaled = second * slot_base[6];
            if (scaled < 0) {
                scaled += 0x7F;
            }
            func_8005D7D4(index, first, scaled >> 7, bank, 3);
        }
        odd_parameter += 2;
        index++;
        slot++;
    } while (index < 6);

    D_801E6CB8 = value;
    func_8005BF30();
    func_8005C6C0();
    func_8005C168();
    func_8005CDB0();
}
