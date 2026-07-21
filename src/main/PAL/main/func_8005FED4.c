#include "common.h"

extern char D_80012F90[];
extern char D_80012F98[];

extern s16 D_8019CE08;
extern s16 D_8019CB08;
extern u16 D_801E4BF0;
extern u16 D_8019CAD0;
extern u16 D_8019CA08;
extern u16 D_8019CA0A;
extern u16 D_801E418C;
extern u16 D_8019CA0C;
extern s32 D_801E4094;
extern s32 D_801E4098;
extern s32 D_801E409C;
extern s32 D_801E40A0;
extern s32 D_801E40A4;
extern s32 D_801E6E7C;
extern s32 D_801E6E80;
extern s32 D_801E6E84;
extern s32 D_801E6E88;
extern s32 D_801E6E8C;
extern s32 D_801E42CC;
extern s32 D_8019C980;
extern s32 D_8019C984;
extern s32 D_8019C988;
extern s32 D_8019C98C;
extern s32 D_8019C990;
extern u16 D_8019CAC0;
extern s32 D_801E772C;
extern s32 D_801E7730;

extern u8 D_801E4F44[];
extern u8 D_8019C914[];
extern u8 D_801E4388[];
extern u16 D_8019CB40[];
extern u16 D_801E444C[];
extern u16 D_801E6F2C[];
extern s32 D_801E4408[];
extern s32 D_8019C70C[];
extern s32 D_801E7744[];
extern s32 D_8019CB78[];
extern s32 D_801E41E8[];

extern s32 D_8019C704;
extern s32 D_801E8A50;
extern s32 D_801E6C70;
extern u8 D_801E42EC[];
extern u8 D_8009E874[];

extern u8 D_8007BEE4[];
extern u8 D_8007BEDC[];

void func_8001674C(char *fmt, ...);
void func_80013F80(s32 a, s32 b);
void func_80021224(void);
void func_80065B24(void *rect, void *data);

s32 func_8005FED4(u8 *arg0) {
    register u8 *base asm("$17") = arg0;
    __asm__("" : "=r"(base) : "0"(base));
    {
        register u32 sum asm("$16");
        register u32 i asm("$13");
        register u16 *p asm("$3");

        i = 0;
        __asm__("" : "=r"(i) : "0"(i));
        sum = i;
        p = (u16 *)base;
        do {
            sum += *p++;
            i++;
        } while (i < 0x7FE);
        func_8001674C(D_80012F90);
        sum = ~sum;
        func_8001674C(D_80012F98, *(s32 *)(base + 0xFFC), sum);
        if (*(s32 *)(base + 0xFFC) != sum) {
            return 0;
        }
    }

    {
        u16 h0 = *(u16 *)(base + 0x0);
        u16 h1 = *(u16 *)(base + 0x2);
        u16 h2 = *(u16 *)(base + 0x4);
        u16 h3 = *(u16 *)(base + 0x6);
        D_8019CA08 = *(u16 *)(base + 0x8);
        D_8019CA0A = *(u16 *)(base + 0xA);
        D_8019CA0C = *(u16 *)(base + 0xC);
        {
            u16 hE = *(u16 *)(base + 0xE);
            s32 w30;
            D_801E4094 = *(s32 *)(base + 0x10);
            D_801E4098 = *(s32 *)(base + 0x14);
            D_801E409C = *(s32 *)(base + 0x18);
            D_801E40A0 = *(s32 *)(base + 0x1C);
            D_801E40A4 = *(s32 *)(base + 0x20);
            D_801E6E7C = *(s32 *)(base + 0x24);
            D_801E6E80 = *(s32 *)(base + 0x28);
            D_801E6E84 = *(s32 *)(base + 0x2C);
            w30 = *(s32 *)(base + 0x30);
            D_8019CE08 = h0;
            D_8019CB08 = h1;
            D_801E4BF0 = h2;
            D_8019CAD0 = h3;
            D_801E418C = hE;
            D_801E6E88 = w30;
        }
        D_801E6E8C = *(s32 *)(base + 0x34);
        D_8019C980 = *(s32 *)(base + 0x38);
        D_8019C984 = *(s32 *)(base + 0x3C);
        D_8019C988 = *(s32 *)(base + 0x40);
        D_8019C98C = *(s32 *)(base + 0x44);
        D_8019C990 = *(s32 *)(base + 0x48);
        {
            register s32 h4C asm("$3") = *(s16 *)(base + 0x4C);
            register u16 h4E asm("$4") = *(u16 *)(base + 0x4E);
            s32 w54;
            D_801E772C = *(s32 *)(base + 0x50);
            w54 = *(s32 *)(base + 0x54);
            D_801E42CC = h4C;
            D_8019CAC0 = h4E;
            D_801E7730 = w54;
        }
    }

    {
        register u8 *src asm("$6") = base;
        register s32 offset asm("$5") = 0;
        for (; offset < 0x68; offset += 8) {
            D_801E4F44[offset + 0] = src[0x58 + 0];
            D_801E4F44[offset + 1] = src[0x58 + 1];
            D_801E4F44[offset + 2] = src[0x58 + 2];
            D_801E4F44[offset + 3] = src[0x58 + 3];
            D_801E4F44[offset + 4] = src[0x58 + 4];
            D_801E4F44[offset + 5] = src[0x58 + 5];
            D_8019C914[offset + 0] = src[0xC0 + 0];
            D_8019C914[offset + 1] = src[0xC0 + 1];
            D_8019C914[offset + 2] = src[0xC0 + 2];
            D_8019C914[offset + 3] = src[0xC0 + 3];
            D_8019C914[offset + 4] = src[0xC0 + 4];
            D_8019C914[offset + 5] = src[0xC0 + 5];
            D_801E4388[offset + 0] = src[0x128 + 0];
            D_801E4388[offset + 1] = src[0x128 + 1];
            D_801E4388[offset + 2] = src[0x128 + 2];
            D_801E4388[offset + 3] = src[0x128 + 3];
            D_801E4388[offset + 4] = src[0x128 + 4];
            D_801E4388[offset + 5] = src[0x128 + 5];
            src += 8;
        }
    }

    {
        register u8 *src asm("$4") = base;
        register s32 offset asm("$3") = 0;
        for (; offset < 0x2C; offset += 4) {
            *(u16 *)((u8 *)D_8019CB40 + offset) = *(u16 *)(src + 0x190);
            *(u16 *)((u8 *)D_8019CB40 + offset + 2) = *(u16 *)(src + 0x192);
            src += 4;
        }
    }

    {
        register s32 count asm("$13") = 0;
        register u16 *dst asm("$4") = D_801E444C;
        register u8 *src asm("$3") = base;
        for (; count < 0x10; count++) {
            *dst++ = *(u16 *)(src + 0x1BC);
            src += 2;
        }
    }

    {
        register s32 count asm("$13") = 0;
        register u16 *dst asm("$4") = D_801E6F2C;
        register u8 *src asm("$3") = base;
        for (; count < 0x400; count++) {
            *dst++ = *(u16 *)(src + 0x1DC);
            src += 2;
        }
    }

    /* D_801E4408 / D_8019C70C */
    {
        register s32 i asm("$13") = 0;
        register s32 j asm("$12");
        register s32 off asm("$4");
        for (; i < 2; i++) {
            j = 0;
            off = i << 5;
            for (; j < 4; j++) {
                *(s32 *)((u8 *)D_801E4408 + off) = *(s32 *)(base + off + 0x9DC);
                *(s32 *)((u8 *)D_8019C70C + off) = *(s32 *)(base + off + 0xA1C);
                off += 8;
            }
        }
    }

    /* D_801E7744 / D_8019CB78 */
    {
        register s32 i asm("$13") = 0;
        register s32 j asm("$12");
        register s32 k asm("$7");
        register s32 *cb78 asm("$25") = D_8019CB78;
        register s32 *d1base asm("$24") = D_801E7744;
        register s32 ioff asm("$16") = 0;
        for (; i < 2; i++) {
            register s32 iofc asm("$15");
            register s32 *d1 asm("$14");
            register s32 joff asm("$10");
            j = 0;
            iofc = ioff;
            d1 = d1base;
            joff = 0;
            for (; j < 4; j++) {
                register s32 dc asm("$2");
                register s32 *dst2 asm("$11");
                register s32 sb asm("$3");
                register s32 s2 asm("$2");
                register s32 *src2 asm("$9");
                register s32 *dst1 asm("$8");
                register s32 *src1 asm("$6");
                s32 s1;
                k = 0;
                dc = iofc + (s32)cb78;
                __asm__("" : "=r"(dc) : "0"(dc));
                dst2 = (s32 *)(joff + dc);
                sb = iofc + (s32)base;
                __asm__("" : "=r"(sb) : "0"(sb));
                s2 = sb + 0xCDC;
                __asm__("" : "=r"(s2) : "0"(s2));
                src2 = (s32 *)(joff + s2);
                dst1 = d1;
                s1 = sb + 0xA5C;
                __asm__("" : "=r"(s1) : "0"(s1));
                src1 = (s32 *)(joff + s1);
                do {
                    s32 a0 = src1[0], a1 = src1[1], a2 = src1[2], a3 = src1[3];
                    dst1[0] = a0;
                    dst1[1] = a1;
                    dst1[2] = a2;
                    dst1[3] = a3;
                    {
                        s32 b0 = src2[0], b1 = src2[1], b2 = src2[2], b3 = src2[3];
                        dst2[0] = b0;
                        dst2[1] = b1;
                        dst2[2] = b2;
                        dst2[3] = b3;
                    }
                    dst2 += 4;
                    __asm__("" : "=r"(src2) : "0"(src2), "r"(dst2));
                    src2 += 4;
                    __asm__("" : "=r"(dst1) : "0"(dst1), "r"(src2));
                    dst1 += 4;
                    __asm__("" : "=r"(k) : "0"(k), "r"(dst1));
                    k++;
                    __asm__("" : "=r"(src1) : "0"(src1), "r"(k));
                    src1 += 4;
                } while (k < 5);
                d1 = (s32 *)((u8 *)d1 + 0x50);
                joff += 0x50;
            }
            d1base = (s32 *)((u8 *)d1base + 0x140);
            ioff += 0x140;
        }
    }

    /* D_801E41E8 */
    {
        register s32 i asm("$13") = 0;
        register s32 j asm("$12");
        register s32 k asm("$7");
        register s32 *e41e8 asm("$10") = D_801E41E8;
        register s32 ioff asm("$8") = 0;
        for (; i < 2; i++) {
            register s32 iofc asm("$9");
            register s32 *dbase asm("$6");
            register s32 joff asm("$5");
            j = 0;
            iofc = ioff;
            dbase = e41e8;
            joff = 0;
            for (; j < 4; j++) {
                register s32 *dst asm("$4");
                register s32 sb asm("$2");
                register s32 *src asm("$3");
                k = 0;
                dst = dbase;
                sb = iofc + (s32)base + 0xF5C;
                __asm__("" : "=r"(sb) : "0"(sb));
                src = (s32 *)(joff + sb);
                for (; k < 3; k++) {
                    *dst++ = *src++;
                }
                dbase = (s32 *)((u8 *)dbase + 0xC);
                joff += 0xC;
            }
            e41e8 = (s32 *)((u8 *)e41e8 + 0x30);
            ioff += 0x30;
        }
    }

    /* D_8019C704 / D_801E8A50 / D_801E6C70 clamps */
    {
        register s32 v asm("$2") = *(s32 *)(base + 0xFBC);
        register s32 c asm("$3");
        D_8019C704 = v;
        if (v >= 0) {
            c = v;
            if (c >= 0x10) {
                c = 0xF;
            }
        } else {
            c = 0;
        }
        v = *(s32 *)(base + 0xFC0);
        D_8019C704 = c;
        D_801E8A50 = v;
        if (v >= 0) {
            c = v;
            if (c >= 0x10) {
                c = 0xF;
            }
        } else {
            c = 0;
        }
        v = *(s32 *)(base + 0xFC4);
        D_801E8A50 = c;
        D_801E6C70 = v;
        if (v != 0) {
            D_801E6C70 = 1;
        }
    }

    /* D_801E42EC / D_8009E874 unaligned copies */
    memcpy(D_801E42EC, base + 0xFC8, 8);
    memcpy(D_8009E874, base + 0xFD0, 8);

    func_80013F80(D_8019CE08, D_8019CB08);
    func_80021224();
    func_80065B24((void *)D_8007BEE4, D_801E6F2C);
    func_80065B24((void *)D_8007BEDC, D_801E444C);
    return 1;
}
