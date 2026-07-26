#include "common.h"
#include "game/race.h"
#include "game/state.h"

extern s32 D_8009E6D4;

extern s32 D_8009E704;

extern s16 D_801E4DAC;

extern s32 D_801F179C;

extern s32 D_801F1854;

extern s32 D_801F1884;

void func_8001F8D0(s32 arg0, void *arg1, void *arg2);

void func_8002BF68(void *arg0, s32 arg1);

void func_8002C168(void *arg0);

s32 func_80030EB4(void *arg0, s32 arg1);

void func_80032280(void *arg0);

void GameInitShuttleScenery(void) asm("func_8003F0F8");

extern s16 D_8009E74C;

void func_80019EFC(s32 arg0);

typedef struct TrackZone {
    s32 start;
    s32 end;
    s16 code;
    s16 value;
} TrackZone;

extern s32 D_801E408C;

extern s32 D_801E40D8;

extern u8 *D_801E4150;

extern s16 D_8019CE34;

extern s16 D_8019C78C;

extern s16 D_8019CAB0;


extern u8 D_8001147C;

void func_8005E4A4(s32 arg0);

void func_8005B190(s32 arg0, s32 arg1);

void func_80018410(void);

void func_8001674C(void *arg0);

extern s16 D_8009AFA4;

extern s16 D_8009AFA8;

extern s16 D_8009AFAC;

extern s16 D_8009E83C;

extern s32 D_8009AF78;

extern s32 D_8009AF7C;

extern s32 D_8009AF80[];

extern s32 D_8009AF8C;

extern s32 D_8009AF90[];

extern s32 D_8009AFB0;

extern s32 D_801E4148;


extern s32 D_801E4364;

extern s32 D_801E4BCC;

extern s32 D_801E4D64;

extern s32 D_801E4D98[];

extern s32 D_801E41E8[][4][3];

extern s32 D_8019C70C[][4][2];

void func_80033308(s32 arg0, s32 arg1);

void func_80033D50(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

void func_8005D6EC(s32 arg0);

void func_80034F74(void) {
    void *primary;
    void *secondary;

    GameInitShuttleScenery();

    primary = &D_8009E6D4;
    secondary = &D_801F1854;
    func_8001F8D0(D_801F179C, primary, secondary);

    D_8009E704 = func_80030EB4(primary, D_8009E704);
    func_8002BF68(primary, 1);
    func_8002C168(primary);
    func_80032280(primary);

    if (D_801E4DAC == 1) {
        D_801F1884 = func_80030EB4(secondary, D_801F1884);
        func_8002BF68(secondary, 1);
        func_8002C168(secondary);
        func_80032280(secondary);
    }
}

void func_80035040(void) {
    void *ptr = &D_8009E6D4;

    func_8002C168(ptr);
    func_80032280(ptr);

    if (D_801E4DAC == 1) {
        ptr = &D_801F1854;
        func_8002C168(ptr);
        func_80032280(ptr);
    }

    func_80019EFC(D_8009E74C);
}

s32 func_800350B4(s32 position) {
    register u8 *base asm("$2");
    register s32 scene asm("$3");
    register TrackZone *first asm("$9");
    register TrackZone *zone asm("$6");
    register s32 status asm("$5");
    register s32 two asm("$10");
    register s32 start asm("$7");
    register s32 finish asm("$8");
    register s32 code asm("$3");
    register u16 rawCode asm("$2");

    base = D_801E4150;
    scene = D_801E408C;
    first = (TrackZone *)(base + 0xA74);
    if (scene != 0) {
        position = D_801E40D8 - position;
    }

    status = 0;
    two = 2;
    zone = first;
    D_8019CE34 = 0;
    D_8019C78C = 0;
    D_8019CAB0 = 0;

    do {
    start = zone->start;
    finish = zone->end;
    if (start == -1) {
        goto done;
    }

    if ((start < position) && (position < finish)) {
        if (position < start + 0x100) {
            status = 1;
        } else if (finish - 0x100 < position) {
            status = 2;
        } else {
            status = 3;
        }

        rawCode = zone->code;
        D_8019CE34 = rawCode;
        asm("" : : : "memory");
        code = (s16)rawCode;
        if (code == 0) {
            goto code_zero;
        }
        if (code > 0) {
            goto code_positive;
        }
        if (code == -3) {
            goto code_minus_three;
        }
        goto normalize_code;

code_positive:
        if (code == two) {
            goto code_two;
        }
        goto normalize_code;

code_zero:
        D_8019CAB0 = 3;
        goto store_value;

code_two:
        status = 4;
code_minus_three:
        if (status == two) {
            status = 3;
        }
        D_8019CE34 = 1;
        goto store_value;

normalize_code:
        if (D_8019CE34 < 0) {
            D_8019CE34 = -D_8019CE34;
            status = 3;
        }
store_value:
        D_8019C78C = zone->value;
    }

    if (status > 0) {
        goto done;
    }
    zone++;
    } while ((s32)zone < (s32)first + 0xF0);

done:
    switch (status) {
    case 1:
        return position - start;
    case 2:
        return finish - position;
    case 3:
        return 0x100;
    default:
        return 0;
    }
}

void func_80035258(s32 arg0) {
    g_SceneId = arg0;
    func_8005E4A4(0);
    func_8005B190(0, 0);
    if (g_SceneId == 6) {
        func_80018410();
    }
    func_8001674C(&D_8001147C);
}

void func_800352B8(void *arg0, s32 arg1, s32 arg2) {
    s32 slot;
    s32 nextSlot;
    s32 delta;
    s32 value;
    s32 tile;
    s32 timeout;
    s32 threshold;
    u8 *route;

    route = (u8 *)arg0 + 0xBC;

    if (arg2 == 2 || arg1 != 0) {
        return;
    }

    slot = D_801E4148;
    if (slot >= 0) {
        if ((*(s16 *)((u8 *)arg0 + 0x168) - 1) * D_801E40D8 + D_801E4D98[slot] <=
                (*(s32 *)((u8 *)arg0 + 0x6C) + *(s32 *)((u8 *)arg0 + 0x68)) ||
            arg2 != 0) {
            D_8009AF80[slot] = D_801E4D64;
            if (D_801E4D64 <= 0x927BE) {
                if (arg2 != 0) {
                    delta = D_8009AF8C - D_801E4D64;
                } else {
                    delta = D_8009AF90[slot] - D_801E4D64;
                }

                D_8009AFAC = 1;
                if (delta < 0) {
                    D_8009AFAC = -1;
                    delta = -delta;
                    if (arg2 == 0) {
                        func_8005D6EC(0x3F);
                    }
                } else if (delta > 0 && arg2 == 0) {
                    func_8005D6EC(0x3E);
                }
                D_8009AF7C = delta;
            } else {
                D_8009AFAC = 0;
            }

            D_8009AFA8 = 0;
            nextSlot = D_801E4148;
            nextSlot++;
            nextSlot %= 3;
            D_801E4148 = nextSlot;

            if (arg2 != 0) {
                D_8009AFA4 = 2;
                D_8009AFB0 = D_8009AF8C;
                D_8009AF8C = D_801E4BCC;
            } else {
                nextSlot += 2;
                nextSlot %= 3;
                D_8009AFA4 = nextSlot;
                D_8009AFB0 = D_8009AF90[nextSlot];
            }

            nextSlot = D_801E4148;
            nextSlot += 2;
            nextSlot %= 3;
            nextSlot <<= 2;
            D_8009AF78 = *(s32 *)((u8 *)D_8009AF80 + nextSlot);
            goto draw;
        }
    }

    if (D_801E4148 == -2 && arg2 != 0) {
        D_801E4148 = 0;
        D_8009AFAC = 0;
        D_8009AFB0 = D_801E41E8[D_801E408C][g_CourseIndex][0];
        D_8009AFA8 = 0x3C;
        D_8009AFA4 = (u16)D_801E4148;
        goto draw;
    }

    nextSlot = D_801E4148;
    if (nextSlot >= 0 && D_801E4364 >= *(s16 *)(route + 0xAC)) {
        if (D_8009AFA8 < 0x3C) {
            D_8009AFA8++;
            if (D_8009AFA8 == 0x3C) {
                D_8009AFB0 = D_8009AF90[nextSlot];
                D_8009AFAC = 0;
                D_8009AFA4 = (u16)D_801E4148;
            }
        }
    } else {
        D_8009AFA4 = 0;
        D_8009AFA8 = 0;
        D_8009AFAC = 0;
        D_8009AFB0 = D_8009AF90[0];
    }

draw:
    if (D_8009AFA8 >= 0x3C) {
        threshold = 0x927BE;
        value = D_801E4D64;
        goto compare_first;
    } else if (D_801E4148 >= 0) {
        if (D_8009AFAC != 0) {
            if (D_801E4364 >= D_8009E83C) {
                value = D_8009AF7C;
                if (D_8009AFAC > 0) {
                    tile = 0x7810;
                } else {
                    tile = 0x780F;
                }
                func_80033D50(0x80, 0x50, value, tile, 0x3E8);
            }
        }
        threshold = 0x927BE;
        value = D_8009AF78;
    } else {
        goto skip_first;
    }

compare_first:
    if (value <= threshold) {
        tile = 0x78CC;
    } else {
        tile = 0x7890;
    }
    func_80033D50(0x12, 0x2A, value, tile, 0x3E8);

skip_first:
    timeout = 0x3E8;
    func_80033D50(0x12, 0x20, D_8009AFB0, 0x78CC, timeout);
    func_80033308(D_8009AFA4, D_8009AFAC);

    func_80033D50(
        0xFA,
        0x7C,
        D_8019C70C[D_801E408C][g_CourseIndex][arg1],
        0x78CC,
        timeout);
}
