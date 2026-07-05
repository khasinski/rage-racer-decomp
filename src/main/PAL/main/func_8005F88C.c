#include "common.h"

extern u16 D_8019CE08;
extern u16 D_8019CB08;
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
extern u16 D_801E42CC;
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

void func_8005F88C(u8 *arg0) {
    {
        u16 h0 = D_8019CE08;
        u16 h1 = D_8019CB08;
        u16 h2 = D_801E4BF0;
        u16 h3 = D_8019CAD0;
        *(u16 *)(arg0 + 0x0) = h0;
        *(u16 *)(arg0 + 0x2) = h1;
        *(u16 *)(arg0 + 0x4) = h2;
        *(u16 *)(arg0 + 0x6) = h3;
    }
    *(u16 *)(arg0 + 0x8) = D_8019CA08;
    *(u16 *)(arg0 + 0xA) = D_8019CA0A;
    {
        u16 h0 = D_801E418C;
        u16 h1 = D_8019CA0C;
        *(u16 *)(arg0 + 0xE) = h0;
        *(u16 *)(arg0 + 0xC) = h1;
    }

    *(s32 *)(arg0 + 0x10) = D_801E4094;
    *(s32 *)(arg0 + 0x14) = D_801E4098;
    *(s32 *)(arg0 + 0x18) = D_801E409C;
    *(s32 *)(arg0 + 0x1C) = D_801E40A0;
    *(s32 *)(arg0 + 0x20) = D_801E40A4;
    *(s32 *)(arg0 + 0x24) = D_801E6E7C;
    *(s32 *)(arg0 + 0x28) = D_801E6E80;
    *(s32 *)(arg0 + 0x2C) = D_801E6E84;
    *(s32 *)(arg0 + 0x30) = D_801E6E88;
    {
        s32 w34 = D_801E6E8C;
        register u16 h4C asm("$3") = D_801E42CC;
        *(s32 *)(arg0 + 0x34) = w34;
        *(s32 *)(arg0 + 0x38) = D_8019C980;
        *(s32 *)(arg0 + 0x3C) = D_8019C984;
        *(s32 *)(arg0 + 0x40) = D_8019C988;
        *(s32 *)(arg0 + 0x44) = D_8019C98C;
        {
            register s32 w48 asm("$4") = D_8019C990;
            u16 h4E = D_8019CAC0;
            *(u16 *)(arg0 + 0x4C) = h4C;
            *(u16 *)(arg0 + 0x4E) = h4E;
            *(s32 *)(arg0 + 0x48) = w48;
        }
    }
    *(s32 *)(arg0 + 0x50) = D_801E772C;
    *(s32 *)(arg0 + 0x54) = D_801E7730;

    {
        register s32 offset asm("$3") = 0;
        register u8 *dst asm("$4") = arg0;

        for (; offset < 0x68; offset += 8) {
            dst[0x58 + 0] = D_801E4F44[offset + 0];
            dst[0x58 + 1] = D_801E4F44[offset + 1];
            dst[0x58 + 2] = D_801E4F44[offset + 2];
            dst[0x58 + 3] = D_801E4F44[offset + 3];
            dst[0x58 + 4] = D_801E4F44[offset + 4];
            dst[0x58 + 5] = D_801E4F44[offset + 5];

            dst[0xC0 + 0] = D_8019C914[offset + 0];
            dst[0xC0 + 1] = D_8019C914[offset + 1];
            dst[0xC0 + 2] = D_8019C914[offset + 2];
            dst[0xC0 + 3] = D_8019C914[offset + 3];
            dst[0xC0 + 4] = D_8019C914[offset + 4];
            dst[0xC0 + 5] = D_8019C914[offset + 5];

            dst[0x128 + 0] = D_801E4388[offset + 0];
            dst[0x128 + 1] = D_801E4388[offset + 1];
            dst[0x128 + 2] = D_801E4388[offset + 2];
            dst[0x128 + 3] = D_801E4388[offset + 3];
            dst[0x128 + 4] = D_801E4388[offset + 4];
            dst[0x128 + 5] = D_801E4388[offset + 5];
            dst += 8;
        }
    }

    {
        register u8 *dst asm("$4") = arg0;
        register s32 offset asm("$3") = 0;

        for (; offset < 0x2C; offset += 4) {
            *(u16 *)(dst + 0x190) = *(u16 *)((u8 *)D_8019CB40 + offset);
            *(u16 *)(dst + 0x192) = *(u16 *)((u8 *)D_8019CB40 + offset + 2);
            dst += 4;
        }
    }

    {
        register s32 count asm("$13") = 0;
        register u16 *src asm("$4") = D_801E444C;
        register u8 *dst asm("$3") = arg0;

        for (; count < 0x10; count++) {
            *(u16 *)(dst + 0x1BC) = *src++;
            dst += 2;
        }
    }

    {
        register s32 count asm("$13") = 0;
        register u16 *src asm("$4") = D_801E6F2C;
        register u8 *dst asm("$3") = arg0;

        for (; count < 0x400; count++) {
            *(u16 *)(dst + 0x1DC) = *src++;
            dst += 2;
        }
    }

    asm volatile(
        "addu $13,$0,$0\n\t"
        "lui $15,%%hi(D_801E4408)\n\t"
        "addiu $15,$15,%%lo(D_801E4408)\n\t"
        "lui $14,%%hi(D_8019C70C)\n\t"
        "addiu $14,$14,%%lo(D_8019C70C)\n\t"
        "addu $10,$24,$0\n"
        "1:\n\t"
        "addu $12,$0,$0\n\t"
        "sll $9,$13,5\n\t"
        "addu $11,$10,$0\n\t"
        "addiu $8,$10,0x9DC\n"
        "2:\n\t"
        "addu $6,$0,$0\n\t"
        "sll $3,$12,3\n\t"
        "addiu $2,$11,0xA1C\n\t"
        "addu $7,$3,$2\n\t"
        "addu $2,$9,$14\n\t"
        "addu $5,$3,$2\n\t"
        "addu $4,$8,$0\n\t"
        "addu $2,$9,$15\n\t"
        "addu $3,$3,$2\n"
        "3:\n\t"
        "lw $2,0($3)\n\t"
        "addiu $3,$3,4\n\t"
        "addiu $6,$6,1\n\t"
        "sw $2,0($4)\n\t"
        "lw $2,0($5)\n\t"
        "addiu $5,$5,4\n\t"
        "addiu $4,$4,4\n\t"
        "sw $2,0($7)\n\t"
        "slti $2,$6,2\n\t"
        "bnez $2,3b\n\t"
        "addiu $7,$7,4\n\t"
        "addiu $12,$12,1\n\t"
        "slti $2,$12,4\n\t"
        "bnez $2,2b\n\t"
        "addiu $8,$8,8\n\t"
        "addiu $13,$13,1\n\t"
        "slti $2,$13,2\n\t"
        "bnez $2,1b\n\t"
        "addiu $10,$10,0x20"
        :
        :
        : "$2", "$3", "$4", "$5", "$6", "$7", "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "memory");

    asm volatile(
        "addu $13,$0,$0\n\t"
        "lui $19,%%hi(D_801E7744)\n\t"
        "addiu $19,$19,%%lo(D_801E7744)\n\t"
        "lui $18,%%hi(D_8019CB78)\n\t"
        "addiu $18,$18,%%lo(D_8019CB78)\n\t"
        "addu $25,$24,$0\n\t"
        "addu $16,$0,$0\n"
        "4:\n\t"
        "addu $12,$0,$0\n\t"
        "addu $15,$16,$0\n\t"
        "addu $17,$25,$0\n\t"
        "addiu $14,$25,0xA5C\n\t"
        "addu $11,$0,$0\n"
        "5:\n\t"
        "addu $6,$0,$0\n\t"
        "addiu $2,$17,0xCDC\n\t"
        "addu $10,$11,$2\n\t"
        "addu $2,$15,$18\n\t"
        "addu $9,$11,$2\n\t"
        "addu $8,$14,$0\n\t"
        "addu $2,$15,$19\n\t"
        "addu $7,$11,$2\n"
        "6:\n\t"
        "lw $2,0($7)\n\t"
        "lw $3,4($7)\n\t"
        "lw $4,8($7)\n\t"
        "lw $5,12($7)\n\t"
        "sw $2,0($8)\n\t"
        "sw $3,4($8)\n\t"
        "sw $4,8($8)\n\t"
        "sw $5,12($8)\n\t"
        "lw $2,0($9)\n\t"
        "lw $3,4($9)\n\t"
        "lw $4,8($9)\n\t"
        "lw $5,12($9)\n\t"
        "sw $2,0($10)\n\t"
        "sw $3,4($10)\n\t"
        "sw $4,8($10)\n\t"
        "sw $5,12($10)\n\t"
        "addiu $10,$10,0x10\n\t"
        "addiu $9,$9,0x10\n\t"
        "addiu $8,$8,0x10\n\t"
        "addiu $6,$6,1\n\t"
        "slti $2,$6,5\n\t"
        "bnez $2,6b\n\t"
        "addiu $7,$7,0x10\n\t"
        "addiu $14,$14,0x50\n\t"
        "addiu $12,$12,1\n\t"
        "slti $2,$12,4\n\t"
        "bnez $2,5b\n\t"
        "addiu $11,$11,0x50\n\t"
        "addiu $25,$25,0x140\n\t"
        "addiu $13,$13,1\n\t"
        "slti $2,$13,2\n\t"
        "bnez $2,4b\n\t"
        "addiu $16,$16,0x140"
        :
        :
        : "$2", "$3", "$4", "$5", "$6", "$7", "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$16", "$17", "$18", "$19", "$25", "memory");

    asm volatile(
        "addu $13,$0,$0\n\t"
        "lui $11,%%hi(D_801E41E8)\n\t"
        "addiu $11,$11,%%lo(D_801E41E8)\n\t"
        "addu $10,$24,$0\n\t"
        "addu $8,$0,$0\n"
        "7:\n\t"
        "addu $12,$0,$0\n\t"
        "addu $9,$8,$0\n\t"
        "addiu $7,$10,0xF5C\n\t"
        "addu $5,$0,$0\n"
        "8:\n\t"
        "addu $6,$0,$0\n\t"
        "addu $4,$7,$0\n\t"
        "addu $2,$9,$11\n\t"
        "addu $3,$5,$2\n"
        "9:\n\t"
        "lw $2,0($3)\n\t"
        "addiu $3,$3,4\n\t"
        "addiu $6,$6,1\n\t"
        "sw $2,0($4)\n\t"
        "slti $2,$6,3\n\t"
        "bnez $2,9b\n\t"
        "addiu $4,$4,4\n\t"
        "addiu $7,$7,0xC\n\t"
        "addiu $12,$12,1\n\t"
        "slti $2,$12,4\n\t"
        "bnez $2,8b\n\t"
        "addiu $5,$5,0xC\n\t"
        "addiu $10,$10,0x30\n\t"
        "addiu $13,$13,1\n\t"
        "slti $2,$13,2\n\t"
        "bnez $2,7b\n\t"
        "addiu $8,$8,0x30"
        :
        :
        : "$2", "$3", "$4", "$5", "$6", "$7", "$8", "$9", "$10", "$11", "$12", "$13", "memory");

    asm volatile(
        "addu $13,$0,$0\n\t"
        "addu $6,$0,$0\n\t"
        "lui $2,%%hi(D_8019C704)\n\t"
        "lw $2,%%lo(D_8019C704)($2)\n\t"
        "lui $3,%%hi(D_801E8A50)\n\t"
        "lw $3,%%lo(D_801E8A50)($3)\n\t"
        "lui $4,%%hi(D_801E6C70)\n\t"
        "lw $4,%%lo(D_801E6C70)($4)\n\t"
        "addu $7,$24,$0\n\t"
        "sw $2,0xFBC($24)\n\t"
        "sw $3,0xFC0($24)\n\t"
        "sw $4,0xFC4($24)\n\t"
        "lui $5,%%hi(D_801E42EC)\n\t"
        "addiu $5,$5,%%lo(D_801E42EC)\n\t"
        "lwl $2,3($5)\n\t"
        "lwr $2,0($5)\n\t"
        "lwl $3,7($5)\n\t"
        "lwr $3,4($5)\n\t"
        "swl $2,0xFCB(%0)\n\t"
        "swr $2,0xFC8(%0)\n\t"
        "swl $3,0xFCF(%0)\n\t"
        "swr $3,0xFCC(%0)\n\t"
        "lui $5,%%hi(D_8009E874)\n\t"
        "addiu $5,$5,%%lo(D_8009E874)\n\t"
        "lwl $2,3($5)\n\t"
        "lwr $2,0($5)\n\t"
        "lwl $3,7($5)\n\t"
        "lwr $3,4($5)\n\t"
        "swl $2,0xFD3(%0)\n\t"
        "swr $2,0xFD0(%0)\n\t"
        "swl $3,0xFD7(%0)\n\t"
        "swr $3,0xFD4(%0)\n"
        "10:\n\t"
        "lhu $2,0($7)\n\t"
        "addiu $13,$13,1\n\t"
        "addu $6,$6,$2\n\t"
        "sltiu $2,$13,0x7FE\n\t"
        "bnez $2,10b\n\t"
        "addiu $7,$7,2\n\t"
        "nor $6,$0,$6\n\t"
        "sw $6,0xFFC($24)"
        :
        : "r"(arg0)
        : "$2", "$3", "$4", "$5", "$6", "$7", "$13", "memory");
}
