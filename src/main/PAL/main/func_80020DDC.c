#include "common.h"
#include "game/state.h"
#include "game/menu.h"
#include "game/race.h"

typedef struct {
    u8 pad[0x10];
    s32 elapsedTime;
} RaceProg;

typedef struct {
    u8 raw[8];
} UnkCopyChunk;

extern s32 D_8019CB74;
extern s32 D_801E6DA0;
extern s32 D_801E6C78;
extern s32 D_801F17B0;
extern s32 D_8019CE0C;
extern RaceProg *D_801E4FAC;
extern s32 D_801E4D0C;
extern s32 D_801E4B94;
extern s32 D_8019C8EC;
void func_80033AA0(s32, s32);
void func_800204F4(s32);
void func_800206B8(s32);
void func_8005D6EC(s32);
void func_80020D90(void);
void func_80018410(void);
void func_80020B08(void);
void func_800201D4(void);
extern s32 D_8019C704;
extern s32 D_801E8A50;
extern s32 D_801E6C70;
void func_8005BD84(s32 arg0);
void func_8005BDD4(s32 arg0);
void func_8005BE24(void);
void func_8005BE58(void);
extern UnkCopyChunk D_8007BE68[];
extern u8 D_801E4388;
extern s16 D_8019CB40;
extern s16 D_8019CB42;
extern s32 D_801E4DA8;
extern s32 D_8019C980;
extern s32 D_8019C984;
extern s32 D_8019C988;
extern s32 D_8019C98C;
extern s32 D_8019C990;
extern u8 D_801E4F44;
extern s32 D_801E4094;
extern u8 D_8019C914;
extern s32 D_801E6E7C;
extern u8 D_8009E874;
extern u8 D_801E42EC;
extern u8 *D_8009E67C;
extern s32 D_801E40A8;
extern s32 D_801E42CC;
void func_80021224(void);
void func_80021288(void *arg0, s32 *arg1);
void func_800212F0(s32 arg0);
void func_8001B488(void);

void func_80020DDC(void) {
    s32 lim1 = D_801E6DA0;
    s32 lim0 = D_801E6C78;
    s32 st;
    s32 t;

    if (g_PadHeld & 0x860) {
        lim1 <<= 2;
        lim0 <<= 2;
    }

    switch (D_8019CB74) {
    case 0:
        g_SceneTimer -= 8;
        func_80033AA0(g_SceneTimer, 0x49);
        if (g_SceneTimer == 0) D_8019CB74 = 1;
        func_800204F4(0);
        goto L428;
    case 1:
        func_800204F4(0);
        if (g_PadEdge2 & 0x860) {
            D_8019CB74 = 2;
            g_SceneTimer = 0;
        }
        goto L428;
    case 2:
        g_SceneTimer += 8;
        func_800204F4(g_SceneTimer);
        if ((u32)g_SceneTimer >= 129) D_8019CB74 = 3;
        goto L428;
    case 3:
        g_SceneTimer -= 8;
        func_800206B8(g_SceneTimer);
        if (g_SceneTimer == 0) D_8019CB74 = 4;
        goto L428;
    case 4:
        g_SceneTimer += 1;
        if ((u32)g_SceneTimer < 121) goto L234;
        if (D_801F17B0 == 0) goto L248;
        func_8005D6EC((g_PadHeld & 0x860) ? 0x10 : 0xf);
        t = D_801F17B0;
        if (t >= lim1) {
            D_801F17B0 = t - lim1;
            D_801E4FAC->elapsedTime += lim1;
        } else {
            s32 e = D_801E4FAC->elapsedTime;
            D_801F17B0 = 0;
            D_801E4FAC->elapsedTime = e + t;
        }
    L234:
        if (D_801F17B0 != 0) goto L420;
    L248:
        g_SceneTimer = 0;
        if (D_8019CE0C == 0) goto Lstore7;
        st = 5;
        goto Lstore;
    case 5:
        func_8005D6EC(0x11);
        if (!(g_PadEdge2 & 0x860)) goto L420;
        st = 6;
    Lstore:
        D_8019CB74 = st;
        goto L420;
    case 6:
        func_80020D90();
        if (D_8019CE0C == 0) { st = 7; goto Lstore; }
        func_8005D6EC((g_PadHeld & 0x860) ? 0x10 : 0xf);
        t = D_8019CE0C;
        if (t >= lim0) {
            D_8019CE0C = t - lim0;
            D_801E4FAC->elapsedTime += lim0;
        } else {
            s32 e = D_801E4FAC->elapsedTime;
            D_8019CE0C = 0;
            D_801E4FAC->elapsedTime = e + t;
        }
        if (D_8019CE0C != 0) goto L420;
    Lstore7:
        st = 7;
        goto Lstore;
    case 7:
        func_80020D90();
        func_8005D6EC(0x11);
        if (!(g_PadEdge2 & 0x860)) goto L420;
        if (D_801E4D0C != 0) goto L420;
        if (D_801E4B94 != 0) { st = 8; goto Lstore; }
        func_80018410();
        st = 8;
        goto Lstore;
    case 8:
        if (D_8019C8EC != 0)
            g_SceneTimer += 1;
        else
            g_SceneTimer += 2;
        func_80033AA0(g_SceneTimer, 0x49);
        if ((u32)g_SceneTimer < 0x100) goto L420;
        break;
    default:
        goto L428;
    }
    func_80020B08();
L420:
    func_800206B8(0);
L428:
    func_800201D4();
}

void func_80021224(void) {
    func_8005BD84(D_8019C704);
    func_8005BDD4(D_801E8A50);
    if (D_801E6C70 == 0) {
        func_8005BE24();
    } else {
        func_8005BE58();
    }
}

void func_80021288(void *arg0, s32 *arg1) {
    UnkCopyChunk *dst;
    UnkCopyChunk *src;
    s32 i;

    i = 0;
    dst = arg0;
    src = D_8007BE68;
    do {
        *dst = *src;
        dst++;
        i++;
    } while (src++, i < 13);

    arg1[1] = 3;
    arg1[0] = 0;
    arg1[2] = 0;
    arg1[3] = -1;
    arg1[4] = 0;
}

void func_800212F0(s32 arg0) {
    u8 *ptr = D_8009E67C;

    *(s16 *)(ptr + 6) = 5;
    ptr[3] = 0;
    ptr[2] = 0;
    ptr[1] = 0;
    ptr[0] = 0;

    if (arg0 < 2) {
        D_8009E67C[3] = 0xFF;
    }

    *(s16 *)(D_8009E67C + 4) = 0;
}

void func_80021338(void) {
    u8 *src;
    u8 *dst;
    s32 i;
    s32 offset;
    s32 emptySlot;

    i = 0;
    dst = &D_801E4388;
    src = &D_8007BE68;
    do {
        __builtin_memcpy(dst, src, sizeof(UnkCopyChunk));
        dst += sizeof(UnkCopyChunk);
        i++;
        src += sizeof(UnkCopyChunk);
    } while (i < 13);

    D_8019CB40 = 0;
    D_8019CB42 = 0;
    D_801E4DA8 = 0;

    emptySlot = -1;
    for (offset = 4; offset < 0x2C; offset += 4) {
        *(s16 *)((u8 *)&D_8019CB40 + offset) = emptySlot;
        *(s16 *)((u8 *)&D_8019CB42 + offset) = 0;
    }

    D_8019C980 = 0;
    D_8019C984 = 3;
    D_8019C988 = 0;
    D_8019C98C = 0;
    D_8019C990 = 0;
    func_80021288(&D_801E4F44, &D_801E4094);
    func_80021288(&D_8019C914, &D_801E6E7C);

    D_8009E67C = &D_8009E874;
    func_800212F0(0);
    D_8009E67C = &D_801E42EC;
    func_800212F0(0);

    g_MaxClassReached[1] = 0;
    g_MaxClassReached[0] = 0;
    D_801E40A8 = 9;
    D_801E42CC = 0;
    func_8001B488();
    D_8019C704 = 0xF;
    D_801E8A50 = 0xF;
    D_801E6C70 = 0;
    func_80021224();
}

s32 func_800214B8(void) {
    u8 *ptr;
    s32 value;
    u8 *end;
    u8 extra;

    ptr = D_8009E67C;
    value = 0;
    if (*(s16 *)(ptr + 4) != 0) {
        return 0;
    }

    end = ptr + 3;
    do {
        value += *ptr++;
    } while ((s32)ptr < (s32)end);

    extra = D_8009E67C[3];
    if (extra == 0xFF) {
        value++;
    } else {
        value += extra;
    }

    value -= 3;
    if (value >= 4) {
        value = 0;
    }
    return value;
}
