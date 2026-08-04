#include "common.h"
#include "game/render.h"
#include "psyq/gte.h"

/*
 * Builds a single-axis rotation matrix into `out` (0x1000 == 1.0 fixed-point).
 * axisMode selects the axis from (axisMode & 0xFF) - 0x58:
 *   0/0x20 -> rotation about X, 1/0x21 -> about Y, 2/0x22 -> about Z.
 * sinTerm/cosTerm are the precomputed sin/cos of the rotation angle.
 */
void BuildAxisRotMatrix(GameRenderAxisMatrix *out, s32 sinTerm, s32 cosTerm, s32 axis) asm("func_80046188");
void BuildAxisRotMatrix(GameRenderAxisMatrix *out, s32 sinTerm, s32 cosTerm, s32 axisMode) {
    s32 one;

    switch ((axisMode & 0xFF) - 0x58) {
    case 0:   /* X-axis rotation */
    case 32:
        one = 0x1000;
        out->m[0][0] = one;
        one = -sinTerm;
        out->m[0][1] = 0;
        out->m[0][2] = 0;
        out->m[1][0] = 0;
        out->m[1][1] = cosTerm;
        out->m[1][2] = one;
        out->m[2][0] = 0;
        out->m[2][1] = sinTerm;
        out->m[2][2] = cosTerm;
        break;

    case 1:   /* Y-axis rotation */
    case 33:
        one = 0x1000;
        out->m[1][1] = one;
        one = -sinTerm;
        out->m[0][0] = cosTerm;
        out->m[0][1] = 0;
        out->m[0][2] = sinTerm;
        out->m[1][0] = 0;
        out->m[1][2] = 0;
        out->m[2][0] = one;
        out->m[2][1] = 0;
        out->m[2][2] = cosTerm;
        break;

    case 2:   /* Z-axis rotation */
    case 34:
        out->m[0][1] = -sinTerm;
        out->m[0][0] = cosTerm;
        out->m[0][2] = 0;
        out->m[1][0] = sinTerm;
        out->m[1][1] = cosTerm;
        out->m[1][2] = 0;
        out->m[2][0] = 0;
        out->m[2][1] = 0;
        out->m[2][2] = 0x1000;
        break;
    }
}

void MatrixApplyZRotation(Matrix *arg0, s32 arg1) asm("func_800684B0");
s32 func_800689A8(s32 arg0);
void MatrixApplyVectorComponents(s16 *mtx, s32 x, s32 y, s32 z, s32 *outX, s32 *outY, s32 *outZ) asm("func_800681F0");
void func_80069858(s32 *matrix);
void func_800698E8(s32 *matrix);

/*
 * Builds a billboard / look-at view Matrix for a GameRenderObject: the eye is
 * obj->{x,y,z} and the look-at target is obj->{field_0C,field_10,field_14}.
 * `len` is the distance (func_800689A8 sqrt); computes a pitch and a yaw axis
 * rotation (func_80046188), the translation (MatrixApplyVectorComponents), then per-row
 * fixed-point projection scaling (<<1 / <<2). Returns 1 if eye==target, else 0.
 */
s32 SetLookAtMatrix(GameRenderObject *obj) asm("func_80046248");
s32 SetLookAtMatrix(GameRenderObject *obj) {
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
                        (obj->motionX - obj->y) * (obj->motionX - obj->y) +
                        (obj->motionY - obj->z) * (obj->motionY - obj->z));
    if (len == 0) {
        return 1;
    }

    horiz = obj->y - obj->motionX;
    pitch = (horiz << 12) / len;
    pitch = -pitch;
    horiz = func_800689A8((obj->field_0C - obj->x) * (obj->field_0C - obj->x) +
                          (obj->motionY - obj->z) * (obj->motionY - obj->z));
    BuildAxisRotMatrix(&am, (s16)pitch, (s16)((horiz << 12) / len), 0x78);
    MulMatrix(&m, &am);

    if (horiz != 0) {
        s32 t1;
        s32 t2;

        len = horiz;
        horiz = obj->field_0C - obj->x;
        t1 = (horiz << 12) / len;
        horiz = obj->motionY - obj->z;
        t2 = (horiz << 12) / len;
        BuildAxisRotMatrix(&am, (s16)(-t1), (s16)t2, 0x79);
        MulMatrix(&m, &am);
    }

    MatrixApplyVectorComponents((s16 *)&m, -obj->x, -obj->y, -obj->z, &outX, &outY, &outZ);
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

// Fixed-point blend in 0..10000 scale.
/* Quadratic Bezier ease over t in 0..10000 with control point `control`:
 * 2t(1-t)*control + t^2, all in the same 1/10000 scale. */
s32 BezierEase(s32 t, s32 control) asm("func_80046598");
s32 BezierEase(s32 arg0, s32 arg1) {
    s32 initial;
    s32 value;
    s32 doubled;

    initial = 0x2710 - arg0;
    value = initial;
    doubled = arg0 * 2;
    value = (value * doubled) / 10000;
    return ((value * arg1) + (arg0 * arg0)) / 10000;
}
