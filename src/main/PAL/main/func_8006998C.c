#include "common.h"

void SetSZfifo3(s32 arg0, s32 arg1, s32 arg2) asm("func_8006998C");

void SetSZfifo3(s32 arg0, s32 arg1, s32 arg2) {
    asm("mtc2 $4,$17");
    asm("mtc2 $5,$18");
    asm("mtc2 $6,$19");
}

void SetSZfifo4(s32 arg0, s32 arg1, s32 arg2, s32 arg3) asm("func_800699A0");

void SetSZfifo4(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    asm("mtc2 $4,$16");
    asm("mtc2 $5,$17");
    asm("mtc2 $6,$18");
    asm("mtc2 $7,$19");
}

void SetSXSYfifo(s32 arg0, s32 arg1, s32 arg2) asm("func_800699B8");

void SetSXSYfifo(s32 arg0, s32 arg1, s32 arg2) {
    asm("mtc2 $4,$12");
    asm("mtc2 $5,$13");
    asm("mtc2 $6,$14");
}

void SetRii(s32 arg0, s32 arg1, s32 arg2) asm("func_800699CC");

void SetRii(s32 arg0, s32 arg1, s32 arg2) {
    asm("ctc2 $4,$0");
    asm("ctc2 $5,$2");
    asm("ctc2 $6,$4");
}

void SetMAC123(s32 arg0, s32 arg1, s32 arg2) asm("func_800699E0");

void SetMAC123(s32 arg0, s32 arg1, s32 arg2) {
    asm("mtc2 $4,$25");
    asm("mtc2 $5,$26");
    asm("mtc2 $6,$27");
}

void func_800699F4(s32 arg0) asm("func_800699F4");

void func_800699F4(s32 arg0) {
    asm("mtc2 $4,$30");
}

void func_80069A00(s32 arg0) asm("func_80069A00");

void func_80069A00(s32 arg0) {
    asm("ctc2 $4,$27");
}

void func_80069A0C(s32 arg0) asm("func_80069A0C");

void func_80069A0C(s32 arg0) {
    asm("ctc2 $4,$28");
}

void SetBackColor(s32 arg0, s32 arg1, s32 arg2) asm("func_80069A18");

void SetBackColor(s32 arg0, s32 arg1, s32 arg2) {
    asm("sll $4,$4,4");
    asm("sll $5,$5,4");
    asm("sll $6,$6,4");
    asm("ctc2 $4,$13");
    asm("ctc2 $5,$14");
    asm("ctc2 $6,$15");
}

void SetFarColor(s32 arg0, s32 arg1, s32 arg2) asm("func_80069A38");

void SetFarColor(s32 arg0, s32 arg1, s32 arg2) {
    asm("sll $4,$4,4");
    asm("sll $5,$5,4");
    asm("sll $6,$6,4");
    asm("ctc2 $4,$21");
    asm("ctc2 $5,$22");
    asm("ctc2 $6,$23");
}

void SetGeomOffset(s32 arg0, s32 arg1) asm("func_80069A58");

void SetGeomOffset(s32 arg0, s32 arg1) {
    asm("sll $4,$4,16");
    asm("sll $5,$5,16");
    asm("ctc2 $4,$24");
    asm("ctc2 $5,$25");
}
