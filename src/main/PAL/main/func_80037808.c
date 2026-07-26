#include "common.h"
#include "game/audio.h"
#include "game/waypoint.h"
#include "psyq/gte.h"

extern s32 D_8009E6D4;

extern s32 D_8009E6DC;

/*
 * Waypoint proximity test: returns 1 if the waypoint's (x,y) lies within a
 * +/-0x40 box around the car centre (D_8009E6D4 / D_8009E6DC), else 0.
 */

extern s32 D_8019C700;

extern TrackWaypointRuntime D_801E4DF4[];

extern s32 D_801E43F8;

extern s32 D_8009E798[];

s32 func_80037808(TrackWaypointRuntime *waypoint);

/*
 * Per-frame waypoint spawn/update state machine over the 6 slots. An idle slot
 * (active==0) that the car is near (func_80037808) spawns: increments the spawn
 * counter D_801E43F8, plays cue 0xA, marks the slot active and seeds its
 * velocity from D_8009E798. An active slot integrates position from velocity
 * with 15/16 per-frame damping and grows its scale toward 0x400, retiring to
 * state 2 once motion decays to zero. Register pins and raw tail-relative field
 * offsets are match-load-bearing.
 */

extern s32 D_801E4168;

void func_80017A10(s32 arg0);

void func_80017794(void *arg0, void *arg1, Matrix *mtx);

void func_8001A4C0(Matrix *mtx, s32 angle);

void func_8001A530(Matrix *mtx, s32 angle);

void func_80028DEC(void *arg0, s32 arg1);

void func_80069458(Matrix *dst, Matrix *src);

void func_80069568(Matrix *lhs, Matrix *rhs);

extern u32 D_1F800084;

/* Counts how many of the 6 waypoint slots are active (active != 0). */

extern s16 D_8009E83C;

extern u8 *D_8019C900;

void *func_80017390(void *ot, void *packet, s32 arg2);

void func_80064DDC(void *ot, void *prim);

void SetShadeTex(u8 *prim, s32 enabled) asm("func_80064EB8");

void SetSprt(u8 *prim) asm("func_80064FA8");

extern u32 D_801E40B8;

extern s16 D_8009EC88;

extern s32 D_801E40D8;

extern s32 D_8009E73C;

extern s32 D_8009E740;

extern s16 D_801E6E74;

extern s16 D_801E43FC;

extern s32 D_8009E694;

extern s32 D_801E4030;

extern s32 D_801E428C;

extern s32 D_8009E744;

extern s16 D_8019C78C;

extern s16 D_8009E74C;

extern u8 D_80011494;

void func_80033AA0(s32 a, s32 b);

void func_80037714(void);

void func_800218A0(s32 a);

void func_80035258(s32 a);

void func_80016754(s32 a, s32 b, void *c, s32 d);

void func_8002BE18(void *a);

void func_8003C508(void *a);

void func_8002DEFC(void *a);

void func_8005D9F8(s32 a, s32 b);

void func_80037C04(void);

void func_80043BCC(s32 a, void *b);

void func_80019EFC(s32 a);

void func_80045CD4(void);

void func_800418D4(void);

void func_80041840(void);

void func_8004123C(void);

void func_8003E1A4(s32 a, s32 b, s32 c);

void func_800350B4(s32 a);

void func_8005B190(s32 a, s32 b);

void func_8002F458(void);

void func_8004087C(s32 a);

void func_8003425C(u32 a);

void func_800410BC(u32 a);

void func_80037860(void);

void func_80037AAC(void);

extern s32 D_801E408C;

extern s32 D_8007E054;

extern s32 D_8007E058;

/*
 * HANDWRITTEN_ASM - excluded from progress (see docs/ASM_AND_GTE_POLICY.md).
 *
 * Symbol:   func_80038288
 * Address:  0x80038288 (PAL/main)
 * Reason:   Tunnel/proximity audio-pitch helper. The surrounding arithmetic is
 *           ordinary C, but the routine ends with a multiply whose product is
 *           never consumed before the function returns.
 * Evidence:
 *   - Dead `mult v0, v1` at the tail (v1 = -0x40): the retail epilogue
 *     (lw ra / lw s0 / addiu sp / jr ra) follows immediately with no mflo/mfhi
 *     and no fall-through consumer. The HI/LO product is discarded.
 *   - No -O2 C compiler emits a pure dead multiply: GCC dead-code-eliminates
 *     any multiply whose result is unused and expressible in C. The retail
 *     bytes therefore cannot be produced from byte-exact plain C - the dead
 *     multiply only exists because it was written by hand.
 * Why C+PSYQ macros are insufficient: a dead multiply is not expressible in C
 *   without an inline-asm `mult` crutch; keeping it as C would require the very
 *   inline assembly this policy forbids for a plain-C match.
 * Current representation: register-pinned C with a single inline-asm `mult`
 *   reproducing the deliberate dead multiply. Byte-exact (tucheck DIFFS=0).
 * Revisit condition: evidence that the product is consumed (e.g. a caller that
 *   reads HI/LO), which would make it ordinary C.
 */

extern s32 D_801E40D8;

s32 func_8001A6AC(s32 arg0, s32 arg1);

s32 func_80068568(s32 arg0);

extern u8 *D_801E4150;

extern u8 *D_8009E688;

extern s32 func_80030EB4(u8 *ent, s32 arg);

extern void func_8002BF68(u8 *ent, s32 arg);

extern void func_80031298(u8 *ent, s32 arg, void *arg2);

typedef struct {
    s32 x;
    s32 y;
    s32 z;
    s32 w;
} Vec4;

/*
 * Initializes/spawns a route render object `ent`: reads a start entry from the
 * per-scene table (`arr` indexed by `pos`, D_801E4150 base), sets the model id
 * (+0xAE / +0x122), start angle (0xC00 - track angle), zeroes the motion state
 * block, resolves the containing track point (func_80030EB4) and builds the
 * initial marker geometry (func_80031298). `ent` is a render/route object
 * accessed by raw byte offset (its first 0xE8 mirror GameRenderObject).
 */

s32 func_80037808(TrackWaypointRuntime *arg0) {
    s32 center_x = D_8009E6D4;
    s32 x = arg0->x;
    s32 ret = 0;

    if ((center_x - 0x40) < x) {
        s32 max_x = center_x + 0x40;

        if (x < max_x) {
            s32 center_y = D_8009E6DC;
            s32 y = arg0->y;

            if ((center_y - 0x40) < y) {
                s32 max_y = center_y + 0x40;

                ret = y < max_y;
            }
        }
    }

    return ret;
}

void func_80037860(void) {
    register TrackWaypointRuntime *waypoint asm("$17");
    register s32 i asm("$18");
    register s32 activeState asm("$19");
    register char *tail asm("$16");

    if (D_8019C700 != 0) {
        D_8019C700--;
    }

    waypoint = D_801E4DF4;
    i = 0;
    activeState = 1;
    tail = (char *)waypoint + 0x34;
    do {
        if (waypoint->active == 0) {
            if (func_80037808(waypoint) != 0) {
                register volatile s32 *src asm("$5");
                register s32 src0 asm("$2");
                register s32 src1 asm("$3");
                register s32 src2 asm("$4");

                D_801E43F8++;
                GamePlaySoundCue(0xA);

                waypoint->active = activeState;
                asm volatile("" ::: "memory");
                src = D_8009E798;
                asm volatile("" : "=r"(src) : "0"(src));
                src0 = src[0];
                src1 = src[1];
                src2 = src[2];
                *(s32 *)(tail - 0x10) = src0;
                *(s32 *)(tail - 0xC) = src1;
                *(s32 *)(tail - 0x8) = src2;
                src0 = src[3];
                *(s32 *)(tail - 0x4) = src0;
                asm volatile("" ::: "memory");

                *(s32 *)(tail - 0x10) *= 2;
                *(s32 *)(tail - 0x8) *= 2;
                *(s32 *)tail =
                    ((*(s32 *)(tail - 0x10) * *(s32 *)(tail - 0x10)) + (*(s32 *)(tail - 0x8) * *(s32 *)(tail - 0x8))) /
                    0x2000;
            }
        } else if (waypoint->active == activeState) {
            *(s32 *)(tail - 0x30) += *(s32 *)(tail - 0x10) / 0x100;
            *(s32 *)(tail - 0x28) += *(s32 *)(tail - 0x8) / 0x100;
            *(s32 *)(tail - 0x10) = (*(s32 *)(tail - 0x10) * 15) / 16;
            *(s32 *)(tail - 0x8) = (*(s32 *)(tail - 0x8) * 15) / 16;
            *(s32 *)(tail - 0x1C) += *(s32 *)tail / 0x100;
            *(s32 *)tail = (*(s32 *)tail * 15) / 16;

            if (*(s32 *)(tail - 0x20) < 0x400) {
                *(s32 *)(tail - 0x20) += 0x80;
            } else {
                *(s32 *)(tail - 0x20) = 0x400;
            }

            if ((*(s32 *)(tail - 0x10) == 0) && (*(s32 *)(tail - 0x8) == 0) && (*(s32 *)tail == 0)) {
                waypoint->active = 2;
            }
        }

        i++;
        tail += sizeof(*waypoint);
        waypoint++;
    } while (i < 6);
}

static inline void ClearScratchRenderMode37AAC(void) {
    D_1F800084 = 0;
}

/*
 * Renders the 6 waypoints. For each active-shaped slot it builds a rotation
 * matrix from the waypoint's angle (point+0x14) and tilt (point+0x10) and emits
 * two GTE draw primitives (func_80028DEC) into the scratchpad OT: the second is
 * the same billboard rotated by 0x800 (180 degrees). `point` walks the
 * TrackWaypointRuntime array D_801E4DF4 via raw offsets. Register pins are
 * match-load-bearing.
 */
void func_80037AAC(void) {
    Matrix mtx0;
    Matrix mtx1;
    register s32 drawId asm("$19");
    register s32 i asm("$18");
    register Matrix *mtx1Ptr asm("$17");
    register char *point asm("$16");
    s32 frameValue;
    s32 drawArg;

    drawId = 2;
    func_80017A10(0);
    i = 0;
    mtx1Ptr = &mtx1;
    point = (char *)&D_801E4DF4[0].x;

    do {
        func_8001A530(&mtx0, *(s32 *)(point + 0x14));
        func_80069568((Matrix *)0x1F800028, &mtx0);
        func_8001A4C0(mtx1Ptr, *(s32 *)(point + 0x10));
        func_80069458(&mtx0, mtx1Ptr);
        func_80017794((void *)0x1F80011C, point, &mtx0);
        frameValue = D_801E4168;
        ClearScratchRenderMode37AAC();
        drawArg = 1;
        if (drawId < frameValue) {
            drawArg = drawId;
        }
        func_80028DEC((void *)0x1F800000, drawArg);

        func_8001A530(mtx1Ptr, 0x800);
        func_80069568(&mtx0, mtx1Ptr);
        func_80017794((void *)0x1F80011C, point, mtx1Ptr);
        frameValue = D_801E4168;
        ClearScratchRenderMode37AAC();
        drawArg = 1;
        if (drawId < frameValue) {
            drawArg = drawId;
        }
        func_80028DEC((void *)0x1F800000, drawArg);

        i++;
        point += sizeof(TrackWaypointRuntime);
    } while (i < 6);
}

s32 func_80037BD4(void) {
    TrackWaypointRuntime *ptr = D_801E4DF4;
    s32 count = 0;
    s32 i = 5;

    do {
        i--;
        count += ptr->active != 0;
        ptr++;
    } while (i >= 0);

    return count;
}

void func_80037C04(void) {
    register u8 *scratch asm("$18");
    register s32 track asm("$22");
    register s32 divisor asm("$19");
    register s32 digitsDrawn asm("$21");
    register s32 xOffset asm("$20");
    register s32 quotient asm("$17");
    register u8 *packet asm("$16");

    scratch = *(u8 **)0x1F800000;
    track = D_8009E83C;
    divisor = 1;
    digitsDrawn = 0;
    xOffset = 0;
    packet = scratch;

    while (1) {
        quotient = track / divisor;
        if (quotient != 0) {
            goto drawDigit;
        }
        if (digitsDrawn > 0) {
            goto done;
        }

drawDigit:
        {
            register s32 y asm("$3");
            register s32 x asm("$2");
            register s32 magic asm("$4");
            register u8 *oldPacket asm("$5");

            SetSprt(scratch);
            SetShadeTex(scratch, 1);

            magic = 0x66666667;
            asm volatile("" : : "r"(magic));
            divisor *= 10;
            y = 0x120 - xOffset;
            xOffset += 0x18;
            digitsDrawn++;
            scratch += 0x14;
            oldPacket = packet;
            asm volatile("" : : "r"(oldPacket));
            x = 0x48;
            asm volatile("mult %0,%1" : : "r"(quotient), "r"(magic));
            packet[0x0D] = x;
            x = 0x18;
            *(s16 *)(packet + 0x10) = x;
            x = 0x20;
            *(s16 *)(packet + 0x12) = x;
            x = 0x10;
            *(s16 *)(packet + 0x0A) = x;
            x = 0x780B;
            *(s16 *)(packet + 0x0E) = x;
            x = quotient >> 31;
            *(s16 *)(packet + 0x08) = y;

            asm volatile("mfhi %0" : "=r"(y));
            y >>= 2;
            y -= x;
            x = (y << 2) + y;
            x <<= 1;
            x = quotient - x;
            y = x << 1;
            y += x;
            y <<= 3;
            packet[0x0C] = y;

            packet += 0x14;
            func_80064DDC(D_8019C900 + 0xCC, oldPacket);
        }
    }

done:
    {
        register void *ot asm("$4");
        register u8 *finalScratch asm("$5");
        register s32 arg2 asm("$6");

        finalScratch = scratch;
        packet = (u8 *)0x1F800000;
        ot = D_8019C900 + 0xCC;
        arg2 = 9;
        *(u8 **)packet = finalScratch;
        *(u8 **)packet = func_80017390(ot, finalScratch, arg2);
    }
}

void func_80037D90(void) {
    s16 *p;
    register u32 a asm("$5");
    s32 x = 0;

    D_801E40B8 = D_801E40B8 + 1;
    if (D_801E40B8 < 61) {
        func_80033AA0(255 - (D_801E40B8 - 6) * 11, 0x49);
    }
    if (D_801E40B8 >= 571 && D_8009EC88 == 0) {
        D_8009EC88 = 1;
    }

    if (D_8009E740 + D_8009E73C >= D_8009E83C * D_801E40D8) {
        if (D_8009E83C < 257) {
            D_8009E83C = D_8009E83C + 1;
            func_80037714();
        }
    }
    if (D_8009E83C >= 257) {
        if (D_801E6E74 == 2) {
            D_801E6E74 = 4;
            D_801E43FC = 0;
        }
    }

    if (D_801E6E74 == 5) {
        if (D_801E43FC > 0) {
            func_800218A0(D_801E43FC * 3);
            func_80033AA0(D_801E43FC * 3, 0x49);
            x = 6;
        }
        if (D_801E43FC >= 101) {
            func_80035258(x);
        }
        D_801E43FC = D_801E43FC + 1;
    } else if (D_801E6E74 == 4) {
        func_80016754(0x5c, 0x78, &D_80011494, 0x7811);
        func_80033AA0(D_801E43FC * 2, 0x29);
        D_801E43FC = D_801E43FC + 1;
        if (D_801E43FC < 201) {
            D_801E43FC = D_801E43FC + 1;
        } else {
            func_80035258(6);
        }
    }

    a = D_801E40B8;
    D_8009E694 = D_8009E694 + 1;
    asm volatile("");
    if (a >= 90) {
        asm volatile("" :: "r"(a));
        if (D_801E6E74 == 0) {
            D_801E6E74 = 1;
            goto Lend;
        }
    } else {
        if (D_801E6E74 == 0) {
            func_8003C508(&D_8009E6D4);
            D_8009EC88 = 0;
            D_801E43F8 = 0;
            goto Lend;
        }
    }
    if (D_801E6E74 == 1) {
        if (D_801E40B8 >= 211) {
            func_8002BE18(&D_8009E6D4);
            D_801E6E74 = 2;
        }
    }
Lend:

    if (D_801E6E74 < 4) {
        func_8003425C(D_801E40B8);
        func_800410BC(D_801E40B8);
    }

    if (D_801E6E74 > 0) {
        func_8002DEFC(&D_8009E6D4);
    } else if (D_801E6E74 == 0) {
        func_8005D9F8(0, 1);
    }
    func_80037C04();

    if (D_801E6E74 > 0) {
        func_80043BCC(0, &D_8009E6D4);
    }

    p = &D_8009E74C;
    func_80019EFC(*p);
    func_80045CD4();
    func_800418D4();
    *(s32 *)0x1F800084 = D_801E4030;
    func_80041840();
    func_8004123C();
    func_8003E1A4(D_801E428C & 3, D_801E40B8, 1);
    func_800350B4(D_8009E744);
    func_8005B190(D_8019C78C, D_8019C78C);
    func_8002F458();
    func_8004087C(*p);
    if (D_801E6E74 < 3) {
        func_80037860();
        func_80037AAC();
    }
}

void func_800381EC(s32 arg0) {
    register s32 result asm("$6");
    register s32 i asm("$5");
    register s32 offset asm("$3");
    register s32 arg asm("$4");
    register s32 scene asm("$2");

    result = 0;
    if (arg0 < 0) {
        arg0 += D_801E40D8;
    }

    scene = D_801E408C;
    i = 0;
    offset = scene << 4;
loop:
    if (*(s32 *)((u8 *)&D_8007E054 + offset) >= arg0) {
        goto next;
    }

    if (arg0 >= *(s32 *)((u8 *)&D_8007E058 + offset)) {
        i++;
        goto check;
    }

    result = 0x46;
    goto done;

next:
    i++;

check:
    if (i < 2) {
        offset += 8;
        goto loop;
    }

done:
    arg = result;
    asm("" : "=r"(arg) : "0"(arg));
    func_8005B190(arg, arg);
}

s32 func_80038288(s32 arg0) {
    register s32 trackLength asm("a1");
    register s32 value asm("v1");
    register s32 temp asm("v0");
    register s32 angle asm("s0");
    register s32 remainder asm("a0");
    register s32 scale asm("v1");

    trackLength = D_801E40D8;
    temp = 0xB875;
    value = trackLength + temp;
    value -= arg0;
    remainder = value % trackLength;
    value = remainder;

    if (value < 0x1A97) {
        temp = value / 100;
        value = 0x80 - temp;
    } else {
        value = trackLength - remainder;
        if (value < 0x801) {
            temp = value / 10;
            value = 0x80 - temp;
        } else {
            value = 0;
        }
    }

    if (value != 0) {
        temp = func_8001A6AC(0x29DD - *(s32 *)0x1F800008, 0x6EF3 - *(s32 *)0x1F800010);
        value = 0xC00 - temp;
        temp = *(s32 *)0x1F80001C;
        value &= 0xFFF;
        temp -= value;
        angle = temp & 0xFFF;
        temp = func_80068568(angle);
        temp = func_80068568(angle);
        scale = -0x40;
        __asm__ volatile("mult %0,%1" : : "r"(temp), "r"(scale));
        asm(".globl func_80038368\nfunc_80038368 = func_80038288 + 0xE0");
    }
}

void func_800383A8(u8 *ent, s32 pos, s32 *arr) {
    u8 *base;
    s32 sub;
    u8 *p;
    u16 val122;
    s32 scene;
    u16 av;

    *(s32 *)(ent + 0xB0) = 1;
    av = *(u16 *)&arr[pos];
    sub = (pos + 1) * 12;
    {
        u8 *baseValue = D_801E4150;
        base = baseValue;
    }
    *(s16 *)(ent + 0x8A) = 0;
    *(s32 *)(ent + 0xBC) = 1;
    *(s16 *)(ent + 0xAE) = av;
    val122 = *(u16 *)&arr[pos];
    scene = D_801E408C;
    *(s16 *)(ent + 0x122) = val122;
    {
        u8 *p1;

        p1 = base + (sub + scene * 144);
        *(s32 *)(ent + 0x30) = *(s16 *)(p1 + 0x35C);
        *(s32 *)(ent + 0x00) = *(s32 *)(p1 + 0x354);
        *(s32 *)(ent + 0x08) = *(s32 *)(p1 + 0x358);
        *(s32 *)(ent + 0x04) = 0;
    }
    {
        s32 ret = func_80030EB4(ent, *(s32 *)(ent + 0x30));
        s32 lev = D_801E408C;
        s32 idx;
        s32 levShift;
        s32 acc;
        s32 angle;

        *(s32 *)(ent + 0x30) = ret;
        *(s32 *)(ent + 0x20) = 0;
        idx = *(s32 *)(ent + 0x30);
        acc = 0xC00;
        levShift = lev << 11;
        angle = *(s16 *)(D_8009E688 + idx * 24 + 0xA);
        acc -= levShift;
        *(s32 *)(ent + 0x24) = (acc - angle) & 0xFFF;

        *(s32 *)(ent + 0x28) = 0;
        *(s32 *)(ent + 0x64) = 0;
        *(s32 *)(ent + 0x6C) = 0;
        *(s32 *)(ent + 0x68) = 0;
        *(s32 *)(ent + 0x70) = 0;
        *(s32 *)(ent + 0xA4) = 0;
        *(s32 *)(ent + 0xA8) = 0;
        *(s32 *)(ent + 0xD0) = 0;
        *(s32 *)(ent + 0xCC) = 0;
        *(s32 *)(ent + 0xC8) = 0;
        *(s32 *)(ent + 0xE0) = 0;
        *(s32 *)(ent + 0xDC) = 0;
        *(s32 *)(ent + 0xD8) = 0;
        *(s32 *)(ent + 0x18) = 0;
        *(s32 *)(ent + 0x14) = 0;
        *(s32 *)(ent + 0x10) = 0;
        *(s32 *)(ent + 0x100) = 0;
        *(s16 *)(ent + 0x116) = 0;
        *(s16 *)(ent + 0x110) = 0;
        *(s32 *)(ent + 0xF4) = 0;
        *(s16 *)(ent + 0x13A) = 0;
        *(s32 *)(ent + 0xF0) = 0;
        *(s32 *)(ent + 0x108) = *(s32 *)(ent + 0x24);
        p = base + (sub + lev * 144);
        *(s32 *)(ent + 0xEC) = *(s32 *)(ent + 0x24);
        *(s32 *)(ent + 0xA0) = *(s32 *)(ent + 0x24);
        *(s32 *)(ent + 0xF8) = 0;
        *(s16 *)(ent + 0x104) = 0;
        *(s32 *)(ent + 0xC4) = 0;
        *(s16 *)(ent + 0x138) = 0;
        func_8002BF68(ent, *(s16 *)(p + 0x35E));
    }

    sub += D_801E408C * 144;
    base += sub;
    {
        u16 model;

        model = *(u16 *)(base + 0x35E);
        *(s16 *)(ent + 0xAC) = model;
        if ((s16)model != -1) {
            struct {
                s32 pad[4];
                s16 a;
                s16 b;
            } pair;

            pair.a = 20;
            pair.b = -20;
            func_80031298(ent, *(s32 *)(ent + 0x30), &pair.a);
            *(s32 *)(ent + 0x60) = *(s32 *)(ent + 0x04);
            *(s32 *)(ent + 0x74) = *(s32 *)(ent + 0x70);
        }
    }

    {
        s32 height;

        height = *(s32 *)(ent + 0x34);
        *(s16 *)(ent + 0x120) = 0;
        *(s32 *)(ent + 0xFC) = height;
        *(s16 *)(ent + 0x11E) = height;
        *(s16 *)(ent + 0x11C) = height;
    }
    *(Vec4 *)(ent + 0x50) = *(Vec4 *)(ent + 0x20);
    {
        s32 lateral;

        lateral = *(s32 *)(ent + 0x04);
        *(s32 *)(ent + 0x40) = 0;
        *(s32 *)(ent + 0x44) = 0;
        *(s32 *)(ent + 0x48) = 0;
        *(s32 *)(ent + 0x60) = lateral;
    }
}

void func_800385FC(u8 *ent, s32 pos, s32 *arr)
{
  register s32 pos2_R10 asm("$10");
  register s32 idx_R8 asm("$8");
  register u8 *base_R9 asm("$9");
  register u8 *ent2_R7 asm("$7");
  register u8 *sub_R6 asm("$6");
  s32 c;
  u16 w;
  pos2_R10 = pos;
  __asm__("" : "=r"(pos2_R10) : "0"(pos2_R10));
  idx_R8 = arr[pos2_R10];
  base_R9 = D_801E4150;
  ent2_R7 = ent;
  if (!(idx_R8 < 12))
  {
    idx_R8 = 0;
  }
  {
    register s32 lev1_R3 asm("$3");
    unsigned int idxoff1_R4;
    register u8 *p1_R4 asm("$4");
    lev1_R3 = D_801E408C;
    idxoff1_R4 = idx_R8;
    idxoff1_R4 = idxoff1_R4 * 16;
    p1_R4 = base_R9 + (idxoff1_R4 + (lev1_R3 * 192));
    *((s16 *) (ent2_R7 + 0x124)) = ((*((s16 *) (p1_R4 + 0x8F4))) * 1168) / 160;
    *((u16 *) (ent2_R7 + 0x126)) = *((u16 *) (p1_R4 + 0x8F6));
    *((u16 *) (ent2_R7 + 0x128)) = *((u16 *) (p1_R4 + 0x8F8));
    *((u16 *) (ent2_R7 + 0x12A)) = *((u16 *) (p1_R4 + 0x8FA));
    *((u16 *) (ent2_R7 + 0x12C)) = *((u16 *) (p1_R4 + 0x8FC));
  }
  __asm__ volatile("");
  c = *((s16 *) (ent2_R7 + 0x128));
  sub_R6 = ent2_R7 + 0xBC;
  *((s16 *) (ent2_R7 + 0x12E)) = 0;
  if (c < 0)
  {
    *((s16 *) (ent2_R7 + 0x128)) = 0;
  }
  else
    if (!(c < 11))
  {
    *((s16 *) (ent2_R7 + 0x128)) = 10;
  }
  if ((*((s16 *) (sub_R6 + 0x6E))) < 0)
  {
    *((s16 *) (sub_R6 + 0x6E)) = 0;
  }
  c = *((s16 *) (sub_R6 + 0x70));
  if (c <= 0)
  {
    *((s16 *) (sub_R6 + 0x70)) = 0;
  }
  else
    if (!(c < 16))
  {
    *((s16 *) (sub_R6 + 0x70)) = 15;
  }
  {
    register s32 lev2_R2 asm("$2");
    register s32 idxoff2_R4 asm("$4");
    register u8 *p2_R3 asm("$3");
    lev2_R2 = D_801E408C;
    idxoff2_R4 = idx_R8 * 16;
    p2_R3 = base_R9 + (idxoff2_R4 + (lev2_R2 * 192));
    w = *((u16 *) (p2_R3 + 0x8FE));
    *((s16 *) (sub_R6 + 0x76)) = w;
    if (((s16) w) < 0x3D)
    {
      *((s16 *) (sub_R6 + 0x76)) = 0x3C;
    }
    lev2_R2 = D_801E408C;
    __asm__("" : "=r"(idxoff2_R4) : "0"(idxoff2_R4));
    p2_R3 = base_R9 + (idxoff2_R4 + (lev2_R2 * 192));
    w = *((u16 *) (p2_R3 + 0x900));
    *((s16 *) (sub_R6 + 0x78)) = w;
    if (((s16) w) <= 0)
    {
      *((s16 *) (sub_R6 + 0x78)) = 0;
    }
  }
  {
    register s32 v_R3 asm("$3");
    v_R3 = *((s16 *) (sub_R6 + 0x68));
    *((s16 *) (sub_R6 + 0x74)) = (v_R3 * 6) / 100;
  }
  if (pos2_R10 >= 4)
  {
    register s32 d_R5 asm("$5");
    register s32 pm4_R3 asm("$3");
    d_R5 = D_801E40D8;
    pm4_R3 = pos2_R10 - 4;
    *((s32 *) (sub_R6 + 0x5C)) = (d_R5 / 12) + ((d_R5 / 40) * pm4_R3);
  }
  else
  {
    *((s32 *) (sub_R6 + 0x5C)) = D_801E40D8 / 12;
  }
}
