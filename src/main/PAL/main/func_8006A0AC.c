#include "common.h"

extern s32 D_80099048;
extern u8 D_80013678[];
extern u8 D_80013688[];
extern u8 D_800136A4[];
extern u8 D_800136B8[];

s32 func_8006A428(s32 arg0);
s32 func_8006A574(s32 arg0);
s32 func_8006A58C(s32 arg0);
s32 func_8006A808(s32 arg0, u8 *arg1, u8 *arg2);
void func_8006A360(void);
void func_8006A388(void);
void func_8006A3B0(void);
void func_8006A3D8(u32 arg0, s32 arg1);
void func_8002785C(void *arg0);
void func_8001674C(u8 *arg0, ...);

s32 func_8006A0AC(s32 arg0, u8 *arg1) {
    u8 command[8];
    u8 response[8];
    register u8 *toc asm("$20") = arg1;
    register s32 oldHandler asm("$21");
    register s32 firstTrack asm("$16");
    register u8 *ptr asm("$17");
    register s32 count asm("$18");
    register s32 lastTrack asm("$19");
    u32 value;

    asm("" : "=r"(toc) : "0"(toc));
    command[0] = 1;
    oldHandler = func_8006A574(0);

    if (func_8006A808(0x13, 0, response) == 0) {
        goto fail;
    }

    {
        register u32 firstBcd asm("$4");
        register u32 lastBcd asm("$5");
        register u32 high asm("$3");

        firstBcd = response[1];
        lastBcd = response[2];
        high = firstBcd >> 4;
        value = high << 2;
        value += high;
        value <<= 1;
        firstBcd &= 0xF;
        firstTrack = value + firstBcd;
        value = lastBcd >> 4;
        high = value << 2;
        high += value;
        high <<= 1;
        lastBcd &= 0xF;
        lastTrack = high + lastBcd;
    }

    if (D_80099048 >= 2) {
        func_8001674C(D_80013678, firstTrack, lastTrack);
    }

    command[0] = 0;
    if (func_8006A808(0x14, command, response) == 0) {
        goto fail;
    }

    value = response[1];
    asm volatile("nop");
    toc[0] = value;
    value = response[2];
    toc[2] = 0;
    toc[1] = value;
    count = 1;

    if (firstTrack <= lastTrack) {
        ptr = toc + 4;
        do {
            command[0] = ((firstTrack / 10) << 4) + (firstTrack % 10);
            if (func_8006A808(0x14, command, response) == 0) {
                goto fail;
            }
            count++;
            value = response[1];
            asm volatile("nop");
            ptr[0] = value;
            value = response[2];
            firstTrack++;
            ptr[2] = 0;
            ptr[1] = value;
            ptr += 4;
        } while (firstTrack <= lastTrack);
    }

    ptr = (u8 *)(count - 1);
    if (D_80099048 >= 2) {
        if ((s32)ptr >= 0) {
            register u8 *entry asm("$16");
            register u8 *fmt asm("$4");
            u32 first;
            u32 second;

            count = 0;
            asm("" : "=r"(count) : "0"(count));
            entry = toc;
            do {
                fmt = D_80013688;
                asm("" : "=r"(fmt) : "0"(fmt));
                first = entry[0];
                second = entry[1];
                entry += 4;
                count++;
                func_8001674C(fmt, first, second);
            } while (count <= (s32)ptr);
        }
    }

    func_8006A574(oldHandler);
    return (s32)ptr;

fail:
    if (D_80099048 != 0) {
        func_8001674C(D_800136A4);
    }
    func_8006A574(oldHandler);
    return 0;
}

s32 func_8006A2D0(void) {
    s32 retries;

    retries = 4;
loop:
    if (func_8006A428(1) != 1) {
        retries--;
        if (retries != -1) {
            goto loop;
        }

        func_8001674C(D_800136B8);
        return 0;
    }

    func_8006A574((s32)func_8006A360);
    func_8006A58C((s32)func_8006A388);
    func_8002785C(func_8006A3B0);
    return 1;
}

void func_8006A360(void) {
    func_8006A3D8(0xF0000003, 0x20);
}

void func_8006A388(void) {
    func_8006A3D8(0xF0000003, 0x40);
}

void func_8006A3B0(void) {
    func_8006A3D8(0xF0000003, 0x40);
}
