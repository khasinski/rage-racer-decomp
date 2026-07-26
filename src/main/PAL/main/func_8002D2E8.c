#include "common.h"

s32 func_80069C98(s32 arg0, s32 arg1, s32 arg2);

/*
 * Point-in-quad test: returns 1 if point `pt` is inside the quad with corners
 * p0,p1,p3,p2 (four chained half-plane sign checks via func_80069C98), else 0.
 */
s32 func_8002D2E8(s32 p0, s32 p1, s32 p2, s32 p3, s32 pt) {
    s32 result;
    s32 ret = 0;

    if (func_80069C98(p0, p1, pt) >= 0) {
        if (func_80069C98(p1, p3, pt) >= 0) {
            if (func_80069C98(p3, p2, pt) >= 0) {
                result = func_80069C98(p2, p0, pt) >= 0;
                ret = result;
            }
        }
    }

    return ret;
}
