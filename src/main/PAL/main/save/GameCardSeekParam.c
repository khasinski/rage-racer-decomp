#include "common.h"
void func_8005F2AC(s32 arg0);
void _card_info(s32 arg0) asm("func_80063DAC");
void func_8005F4D8(void);
void GameCardSeekParam(s32 arg0) asm("func_8005EB78");
void GameCardSeekParam(s32 arg0) {
    func_8005F2AC(arg0);
    _card_info(arg0 & 0xFF);
    func_8005F4D8();
}
