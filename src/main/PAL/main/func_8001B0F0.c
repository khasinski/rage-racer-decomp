typedef unsigned char u8;
typedef signed int s32;

extern u8 *D_8019C900;

void *func_8001720C(void *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void *func_80017390(void *arg0, void *arg1, s32 arg2);

void func_8001B0F0(s32 arg0) {
    void *current;
    void **scratch;
    u8 *base;
    void *next;
    s32 color;

    color = arg0 & 0xFF;
    base = D_8019C900;
    base += 0xCC;
    scratch = (void **)0x1F800000;
    current = *scratch;
    next = func_8001720C(base, current, 0, 0x18, 0x140, 0xC0, color, color, color);
    *scratch = func_80017390(base, next, 0x29);
}
