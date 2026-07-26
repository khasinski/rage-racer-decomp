#include "common.h"
#include "game/car.h"
#include "game/track.h"
#include "game/race.h"

s32 func_80068568(s32 arg0);

/*
 * Car body oscillation / bounce. While motionMode is non-zero, decays
 * motionModeTimer and drives a decaying sine wave (amplitude from
 * motionValue, phase from the timer) into the car's render offsets
 * (field_20 / field_28). motionMode selects the axis/shape of the bounce:
 * 1 & 5 both axes, 2 & 4 vertical only, 3 lateral only.
 */




/*
 * Detects whether the car crossed a route marker this frame: scans the 8-entry
 * row (0x40 stride) of the marker table g_TrackEventData keyed by the car's routeRow,
 * comparing trackProgress against previousTrackProgress (the lap-direction flag
 * g_RaceSeries flips the comparison). Returns the crossed-marker code, or 0.
 * Register-pinned, goto-structured.
 */

s32 func_80039184(GameCarRuntime *arg0);

/*
 * Jump / launch setup: when func_80039184 reports a marker crossing, seeds the
 * launch trajectory (field_90/94/98/9A/9C/9E) and snapshots the car's render
 * offsets (field_20/28 and y). field_98 holds the launch state (1 = jump). The
 * inline mult/mfhi block is the compiler's divide idiom; keep it verbatim.
 */

/*
 * Fields of g_Cars[0] that retail reaches through their own split symbols
 * rather than by indexing the array, so the 0x19C-stride walks below keep them
 * raw:  D_801F18C4 = .trackProgress  D_801F198C = .field_138
 *       D_801F198E = .field_13A
 */
extern u8 D_801F18C4[];

extern u8 D_801F198C[];

extern u8 D_801F198E[];

void func_800393AC(GameCarRuntime *arg0, s32 arg1);

void func_80038FF0(GameCarRuntime *car) {
    s32 value;
    s32 wave;
    s32 amplitude;
    s32 product;

    if (car->motionMode == 0) {
        return;
    }

    car->motionModeTimer--;
    if ((s16)car->motionModeTimer == 0) {
        car->motionMode = 0;
        car->field_92 = 0;
    }

    {
        s32 timer;

        timer = car->motionModeTimer;
        product = timer * car->motionValue;
        if (product < 0) {
            product += 0x7F;
        }
        amplitude = product >> 7;

        wave = func_80068568(((timer * 3) << 12) / 30) * amplitude;
    }
    if (wave < 0) {
        wave += 0x7FF;
    }
    value = wave >> 11;

    switch (car->motionMode) {
    case 1:
    case 5:
        car->field_92 = value + amplitude;
        car->field_20 += car->field_92;
        car->field_92 = value + ((s32)(amplitude + ((u32)amplitude >> 31)) >> 1);
        car->field_28 += car->field_92 / 2;
        break;

    case 2:
        if (car->field_98 != 0) {
            break;
        }
        car->field_28 += value;
        break;

    case 3:
        car->field_92 = value + amplitude;
        car->field_20 += car->field_92;
        break;

    case 4:
        car->field_28 += value;
        break;
    }
}

s32 func_80039184(GameCarRuntime *arg0) {
    register u8 *base asm("t2");
    register s32 pos0 asm("a1");
    register s32 pos1 asm("t0");
    register s32 row asm("v1");
    register s32 temp asm("t1");
    register s32 crossed asm("t3");
    register s32 i asm("a2");
    register s32 offset asm("a3");
    register s32 sentinel asm("t4");
    register s32 cursor asm("a1");
    register s32 diff asm("v0");
    register s32 cmp asm("v0");
    register s32 threshold asm("a1");
    register s32 resultOffset asm("v0");

    base = g_TrackEventData;
    if (arg0->field_A4 < 0x320) {
        return 0;
    }

    pos0 = arg0->trackProgress;
    pos1 = arg0->previousTrackProgress;
    row = arg0->routeRow;

    if (g_RaceSeries != 0) {
        pos0 = g_TrackLength - pos0;
        pos1 = g_TrackLength - pos1;
    }

    if (pos1 < pos0) {
        temp = pos0;
        diff = temp - pos1;
        goto ordered;
    }
    goto not_crossed;

crossed_label:
    crossed = 1;
    goto finish;

not_crossed:
    temp = pos1;
    pos1 = pos0;
    diff = temp - pos1;

ordered:
    if (diff >= 0x1000) {
        temp = 0;
        pos1 = 0;
    }

    crossed = 0;
    i = 0;
    sentinel = -1;
    offset = row << 6;
    cursor = (s32)(base + offset);

loop:
    if (*(s32 *)(cursor + 8) == sentinel) {
        goto finish;
    }
    threshold = *(s32 *)(cursor + 4);
    cmp = temp < threshold;
    offset += 8;
    if (cmp != 0) {
        goto next;
    }
    cmp = pos1 < threshold;
    if (cmp != 0) {
        goto crossed_label;
    }

next:
    i++;
    if (i < 8) {
        cursor = (s32)(base + offset);
        goto loop;
    }

finish:
    if (crossed != 0) {
        resultOffset = i << 3;
        resultOffset += row << 6;
        resultOffset = (s32)(base + resultOffset);
        return *(s32 *)(resultOffset + 8);
    }
    return 0;
}

void func_80039280(GameCarRuntime *arg0) {
    register GameCarRuntime *obj asm("$16");
    register s32 value asm("$4");
    register s32 temp asm("$3");
    register s32 result asm("$2");
    register s32 one asm("$5");
    volatile s32 stack[2];

    obj = arg0;
    (void)stack;

    if (obj->field_98 != 0) {
        result = obj->field_9A;
        value = result * result;
        asm volatile("" : "=r"(value) : "0"(value));
        result = 0x2AAAAAAB;
        asm volatile("mult $4,$2");
        temp = obj->field_90;
        asm volatile("sra $4,$4,31\nmfhi $2\nsubu $4,$2,$4" : "=r"(value), "=r"(result) : "0"(value), "1"(result));
        asm volatile("" : "=r"(value) : "0"(value));
        result = temp;
        asm volatile("" : "=r"(temp) : "0"(temp));
        if (temp >= 0x12C) {
            value >>= 8;
        }
        result += value;
        obj->field_90 = result;

        temp = value;
        if (value < 0) {
            temp = value + 3;
        }
        result = (u16)obj->field_94;
        temp >>= 2;
        result += temp;
        obj->field_94 = result;

        result = obj->field_90;
        temp = obj->field_94;
        obj->field_20 = result;
        obj->field_28 = temp;
        return;
    }

    asm("" : "=r"(obj) : "0"(obj));
    value = func_80039184((GameCarRuntime *)obj);
    if (value == 0) {
        return;
    }

    one = 1;
    obj->field_98 = one;
    if (value > 0) {
        temp = value * obj->field_A4;
        temp = temp / -4800;
        obj->field_98 = one;
        obj->field_9C = temp;
        goto finish_setup;
    } else {
        result = 2;
        obj->field_98 = result;
        result = -value;
        obj->field_9C = result;
    }

finish_setup:
    result = (u16)obj->field_20;
    temp = (u16)obj->field_28;
    value = (u16)obj->y;
    obj->field_9A = 0;
    obj->field_90 = result;
    obj->field_94 = temp;
    obj->field_9E = value;
}

void func_800393AC(GameCarRuntime *arg0, s32 arg1) {
    GameCarRuntime *obj = arg0;
    s32 temp;
    s32 value;
    u8 *base;
    s32 scene;

    base = (u8 *)&obj->field_BC;
    if (*(s32 *)&obj->field_F0 == 0) {
        if (obj->field_F4 != 0) {
            goto check_field_F0;
        }
        if (arg1 != 0) {
            value = obj->field_A4;
            if (value < 0x3C1) {
                return;
            }
            scene = g_RaceSeries;
            arg1 *= value;
            value = arg1;
            temp = value / 0x320;
            *(s32 *)&obj->field_F0 = temp;
            if (scene != 0) {
                temp = -temp;
                *(s32 *)&obj->field_F0 = temp;
            }
            obj->field_F4 = 0;
            return;
        }
check_field_F0:
        temp = *(s32 *)(base + 0x34);
        if (temp == 0) {
            goto decay_field_F4;
        }
    }

decay_field_F0:
    value = *(s32 *)(base + 0x34);
    value = (value << 5) - value;
    if (value < 0) {
        value += 0x1F;
    }
    temp = value >> 5;
    value = (u32)value >> 31;
    *(s32 *)(base + 0x34) = temp;
    temp += value;
    value = *(s32 *)(base + 0x38);
    temp >>= 1;
    value -= temp;
    *(s32 *)(base + 0x38) = value;
    if (value >= 0x2BC) {
        *(s32 *)(base + 0x38) = 0x2BC;
        return;
    }
    temp = value < -0x2BB;
    if (temp != 0) {
        temp = -0x2BC;
        *(s32 *)(base + 0x38) = temp;
        return;
    }
    return;

decay_field_F4:
    value = *(s32 *)(base + 0x38);
    if (value != 0) {
        temp = (value << 4) - value;
        temp <<= 1;
        if (temp < 0) {
            temp += 0x1F;
        }
        temp >>= 5;
        *(s32 *)(base + 0x38) = temp;
        if (temp == 0) {
            *(s16 *)(base + 0x7E) = 0;
        }
    }
}

void func_800394DC(GameCarRuntime *obj, s32 arg1) {
    register GameCarRuntime *objReg asm("$6") = obj;
    register s32 target asm("$7");
    register u8 *state asm("$8");
    s32 index;
    register s32 advanceOffset asm("$4");
    register s32 scene asm("$3");
    register s32 offset asm("$3");
    register u8 *base asm("$2");
    register s16 *entry asm("$3");
    register s32 value asm("$5");
    register s32 valueRaw asm("$4");
    register s32 raw asm("$2");
    s32 stack[2];

    raw = objReg->trackProgress;
    asm volatile("" : : "r"(stack));
    scene = g_RaceSeries;
    target = raw >> 4;
    index = objReg->routeIndex;
    raw = (scene << 1) + scene;
    offset = (raw << 4) - raw;
    offset <<= 3;
    base = g_TrackEventData;
    offset += 0x84;
    entry = (s16 *)(offset + (s32)base);
    raw = (index << 1) + index;
    raw <<= 2;
    entry = (s16 *)((s32)entry + raw);

    if (target < 0x20) {
        state = (u8 *)&objReg->field_BC;
        objReg->routeIndex = 0;
        target = 0;
    } else {
        state = (u8 *)&objReg->field_BC;
    }

    if (target < entry[0]) {
        goto below;
    }
    if (entry[1] < target) {
        goto advance;
    }
    if (arg1 < 4 && objReg->field_10C == 0) {
        valueRaw = objReg->field_11C;
        if (entry[2] < valueRaw) {
            value = valueRaw;
            asm volatile("" : : "r"(value));
            asm volatile("" : "=r"(valueRaw) : "0"(valueRaw));
            raw = entry[3];
            raw = valueRaw < raw;
            if (raw != 0) {
                raw = value + *(u16 *)(entry + 4);
                objReg->field_11C = raw;
            }
        }
    }
    return;

below:
    if (entry[1] < target) {
        goto advance;
    }
    goto clear;

advance:
    {
        register s32 next asm("$2");

        next = *(s32 *)(state + 0x44);
        scene = g_RaceSeries;
        next++;
        advanceOffset = (next << 1) + next;
        advanceOffset <<= 2;
        *(s32 *)(state + 0x44) = next;
    }
    raw = (scene << 1) + scene;
    offset = (raw << 4) - raw;
    offset <<= 3;
    asm volatile("" : "=r"(offset) : "0"(offset));
    base = g_TrackEventData;
    advanceOffset += offset;
    raw = (s32)(base + advanceOffset);
    if (*(s16 *)(raw + 0x84) == -1) {
        *(s32 *)(state + 0x44) = 0;
    }
    *(s16 *)(state + 0x52) = 0;
    return;

clear:
    objReg->field_10E = 0;
}

void func_80039644(void) {
    register s32 one asm("t2") = 1;
    register s32 offset asm("a2") = 0;
    register u8 *base asm("t0");
    register s32 scene asm("v1");
    register s32 product asm("v0");
    register s32 baseOffset asm("t1");
    register s32 index asm("a0");
    register s32 tableOffset asm("a1");
    register s32 raw asm("v0");
    register s32 target asm("a3");
    register s32 value asm("v1");

    scene = g_RaceSeries;
    base = g_TrackEventData;
    product = (scene << 3) + scene;
    baseOffset = product << 6;

outer:
    __asm__ volatile("" ::: "memory");
    index = 0;
    raw = *(s32 *)(D_801F18C4 + offset);
    tableOffset = baseOffset;
    *(s16 *)(D_801F198E + offset) = one;
    target = raw >> 4;

inner:
    value = *(s16 *)(base + tableOffset + 0x474);
    if (target >= value) {
        *(s16 *)(D_801F198C + offset) = index;
        offset += 0x19C;
        goto next;
    }
    if (value == -1) {
        *(s16 *)(D_801F198C + offset) = 0;
        offset += 0x19C;
        goto next;
    }
    __asm__ volatile("" ::: "memory");
    index++;
    if (index < 0x30) {
        tableOffset += 0xC;
        goto inner;
    }
    offset += 0x19C;

next:
    if (offset < 0x11B4) {
        goto outer;
    }
}

void func_800396FC(u8 *car, s32 gear)
{
  u8 *p[2];
  u16 lim[4];
  u16 val[2];
  register u8 *sub_R9 asm("$9");
  s32 rpm;
  s32 g0;
  s32 raw;
  u8 *tbl;
  s32 f;
  register s32 lo_R7 asm("$7");
  s32 hi;
  s32 range;
  register s32 d_R3 asm("$3");
  s32 pitch;
  s32 q;
  register s32 v20_R4 asm("$4");
  s32 cnt;
  s32 one;
  int new_var;
  raw = *((s32 *) (car + 0x70));
  rpm = raw >> 4;
  g0 = *((s16 *) (car + 0x138));
  sub_R9 = car + 0xBC;
  if (rpm < 0x20)
  {
    *((s16 *) (car + 0x138)) = 0;
  }
  if (g0 < 0)
  {
    *((s16 *) (car + 0x138)) = 0;
  }
  tbl = g_TrackEventData + ((g_RaceSeries * 576) + 0x474);
  p[0] = tbl + (g0 * 12);
  p[1] = tbl + ((g0 * 12) + 12);
  lim[0] = *((u16 *) p[0]);
  lim[1] = *((u16 *) p[1]);
  if (gear < 4)
  {
    val[0] = *((u16 *) ((p[0] + (gear * 2)) + 4));
    val[1] = *((u16 *) ((p[1] + (gear * 2)) + 4));
  }
  else
  {
    f = 0x55 - gear;
    val[0] = ((*((s16 *) (p[0] + 0xA))) * f) / 100;
    val[1] = ((*((s16 *) (p[1] + 0xA))) * f) / 100;
  }
  pitch = 0;
  lo_R7 = *((s16 *) (&lim[0]));
  if (rpm < lo_R7)
  {
    goto L2;
  }
  hi = *((s16 *) (&lim[1]));
  one = hi;
  if (one < rpm)
  {
    goto L2_inc;
  }
  range = hi - lo_R7;
  pitch = *((u16 *) (p[0] + 2));
  if (!(range > 0))
  {
    range = 1;
  }
  d_R3 = rpm - lo_R7;
  v20_R4 = (new_var = *((s16 *) (&val[0])));
  q = (((*((s16 *) (&val[1]))) - new_var) * d_R3) / range;
  *((s16 *) (sub_R9 + 0x74)) = ((((new_var + q) * 1168) / 160) * 6) / 100;
  goto OUT;
  L2:
  if ((*((s16 *) (&lim[1]))) < rpm)
  {
    L2_inc:
    __asm__ volatile("" : : : "memory");
    cnt = *((u16 *) (sub_R9 + 0x7C));
    d_R3 = 1;
    q = d_R3;
    g0 = q;
    *((s16 *) (sub_R9 + 0x7E)) = g0;
    cnt = cnt + 1;
  }
  else
  {
    cnt = *((u16 *) (sub_R9 + 0x7C));
    one = 1;
    __asm__("" : "=r"(one) : "0"(one));
    *((s16 *) (sub_R9 + 0x7E)) = one;
    cnt = cnt - 1;
  }

  *((u16 *) (sub_R9 + 0x7C)) = cnt;
  __asm__ volatile("");
  if (rpm < 0x20)
  {
    *((u16 *) (sub_R9 + 0x7C)) = 0;
  }
  OUT:
  if ((*((s16 *) (sub_R9 + 0x7E))) != 0)
  {
    func_800393AC((GameCarRuntime *)car, (s16) pitch);
  }

}
