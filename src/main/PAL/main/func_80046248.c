#include "common.h"
#include "game/render.h"
#include "psyq/gte.h"

void MatrixApplyZRotation(Matrix *arg0, s32 arg1) asm("func_800684B0");
s32 func_800689A8(s32 arg0);
void func_80046188(GameRenderAxisMatrix *out, s32 arg1, s32 arg2, s32 mode);
void func_80069458(Matrix *dst, GameRenderAxisMatrix *src);
void func_800681F0(s16 *mtx, s32 x, s32 y, s32 z, s32 *outX, s32 *outY, s32 *outZ);
void func_80069858(s32 *matrix);
void func_800698E8(s32 *matrix);

s32 func_80046248(GameRenderObject *obj) {
    Matrix m;
    GameRenderAxisMatrix am;
    volatile s32 pad[18];
    s32 outX;
    s32 outY;
    s32 outZ;
    s32 len;
    s32 horiz;
    s32 pitch;

    m.m[0][0] = 0x1000;
    m.m[0][1] = 0;
    m.m[0][2] = 0;
    m.m[1][0] = 0;
    m.m[1][1] = 0x1000;
    m.m[1][2] = 0;
    m.m[2][0] = 0;
    m.m[2][1] = 0;
    m.m[2][2] = 0x1000;
    MatrixApplyZRotation(&m, 0);

    len = func_800689A8((obj->field_0C - obj->x) * (obj->field_0C - obj->x) +
                        (obj->field_10 - obj->y) * (obj->field_10 - obj->y) +
                        (obj->field_14 - obj->z) * (obj->field_14 - obj->z));
    if (len == 0) {
        return 1;
    }

    horiz = obj->y - obj->field_10;
    pitch = (horiz << 12) / len;
    pitch = -pitch;
    horiz = func_800689A8((obj->field_0C - obj->x) * (obj->field_0C - obj->x) +
                          (obj->field_14 - obj->z) * (obj->field_14 - obj->z));
    func_80046188(&am, (s16)pitch, (s16)((horiz << 12) / len), 0x78);
    func_80069458(&m, &am);

    if (horiz != 0) {
        s32 t1;
        s32 t2;

        len = horiz;
        horiz = obj->field_0C - obj->x;
        t1 = (horiz << 12) / len;
        horiz = obj->field_14 - obj->z;
        t2 = (horiz << 12) / len;
        func_80046188(&am, (s16)(-t1), (s16)t2, 0x79);
        func_80069458(&m, &am);
    }

    func_800681F0((s16 *)&m, -obj->x, -obj->y, -obj->z, &outX, &outY, &outZ);
    m.t[0] = outX;
    m.t[1] = outY;
    m.t[2] = outZ;
    m.m[0][0] <<= 1;
    m.m[0][1] <<= 1;
    m.m[0][2] <<= 1;
    m.m[1][0] <<= 2;
    m.m[1][1] <<= 2;
    m.m[1][2] <<= 2;
    m.m[2][0] <<= 1;
    m.m[2][1] <<= 1;
    m.m[2][2] <<= 1;
    m.t[0] <<= 1;
    m.t[1] <<= 1;
    m.t[2] <<= 1;
    func_80069858((s32 *)&m);
    func_800698E8((s32 *)&m);
    return 0;
}
