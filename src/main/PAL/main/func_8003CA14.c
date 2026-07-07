#include "common.h"
#include "game/track.h"

extern GameTrackPoint *D_8009E688;

extern u32 D_801E3E14[];
extern s32 D_801E3E18;
extern s32 D_801E3E1C;
extern s32 D_801E3E38;
extern s32 D_801E3EB4;
extern s32 D_801E3EB8;
extern s32 D_801E3F60;

typedef struct CopyBlock8003CA14 {
    u32 word0;
    u32 word1;
    u32 word2;
    u32 word3;
} CopyBlock8003CA14;

void func_8003CA14(void *arg0) {
    register u32 word0 asm("$2");
    register u32 word1 asm("$3");
    register u32 word2 asm("$4");
    register CopyBlock8003CA14 *src asm("$6");
    register CopyBlock8003CA14 *dst asm("$7");
    register CopyBlock8003CA14 *end asm("$9");
    register u32 *base asm("$8");
    register GameTrackPoint *track asm("$5");
    register GameTrackPoint *point asm("$2");
    register s32 index asm("$3");
    register s32 lastIndex asm("$4");
    register s32 value asm("$3");

    base = arg0;
    asm("" : "=r"(base) : "0"(base));
    dst = (CopyBlock8003CA14 *)D_801E3E14;
    src = (CopyBlock8003CA14 *)base;
    end = (CopyBlock8003CA14 *)((s32)base + 0x190);
    do {
        *dst = *src;
        src++;
        dst++;
    } while (src != end);

    word0 = ((u32 *)src)[0];
    word1 = ((u32 *)src)[1];
    word2 = ((u32 *)src)[2];
    ((u32 *)dst)[0] = word0;
    ((u32 *)dst)[1] = word1;
    ((u32 *)dst)[2] = word2;

    index = *(s32 *)((s32)base + 0x30);
    track = D_8009E688;
    point = (GameTrackPoint *)((index * 3) << 3);
    asm("addu %0,%0,%1" : "=r"(point) : "r"(track), "0"(point));
    D_801E3E14[0] = point->x;

    index = *(s32 *)((s32)base + 0x30);
    point = (GameTrackPoint *)((index * 3) << 3);
    asm("addu %0,%0,%1" : "=r"(point) : "r"(track), "0"(point));
    D_801E3E1C = point->z;

    index = *(s32 *)((s32)base + 0x30);
    point = (GameTrackPoint *)((index * 3) << 3);
    asm("addu %0,%0,%1" : "=r"(point) : "r"(track), "0"(point));
    value = D_801E3EB8;
    word0 = point->y;
    value += 0x40;
    word0 -= 0x40;
    D_801E3EB8 = value;
    D_801E3E18 = word0;

    value = *(s16 *)((s32)base + 0xB8);
    lastIndex = *(s32 *)((s32)base + 0x30);
    value <<= 11;
    point = (GameTrackPoint *)((lastIndex * 3) << 3);
    asm("addu %0,%0,%1" : "=r"(point) : "r"(track), "0"(point));
    value += 0xC00;
    value -= point->angle;
    D_801E3EB4 = value;
    D_801E3F60 = value;
    D_801E3E38 = value;
}
