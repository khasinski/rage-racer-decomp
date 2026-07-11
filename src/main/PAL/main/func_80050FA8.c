#include "common.h"

s32 func_8001785C(void);

s32 func_80050FA8(s32 arg0) {
    s32 state;
    s32 value;

    state = func_8001785C() + 1;

    switch (arg0) {
    case 0:
        switch (state) {
        case 2:
            return 0;
        case 3:
            return 1;
        case 4:
            return 2;
        case 5:
            return 3;
        }
        goto fallback4;

    case 1:
fallback4:
        switch (state) {
        case 3:
            return 4;
        case 4:
            return 5;
        case 5:
            return 6;
        }
        goto fallback8;

    case 2:
fallback8:
        switch (state) {
        case 4:
            return 7;
        case 5:
            return 8;
        }
        goto fallback9;

    case 3:
fallback9:
        value = state - 1;
        if ((u32)value < 5) {
            switch (value) {
            case 0:
                return 9;
            case 1:
                return 10;
            case 2:
                return 11;
            case 3:
                return 12;
            case 4:
                return 13;
            }
        }

    case 4:
        switch (state) {
        case 2:
            return 14;
        case 3:
            return 15;
        case 4:
            return 16;
        case 5:
            return 17;
        }
        goto fallback18;

    case 5:
fallback18:
        switch (state) {
        case 3:
            return 18;
        case 4:
            return 19;
        case 5:
            return 20;
        }
        goto fallback21;

    case 6:
fallback21:
        switch (state) {
        case 4:
            return 21;
        case 5:
            return 22;
        }
        goto fallback23;

    case 7:
fallback23:
        switch (state) {
        case 3:
            return 23;
        case 4:
            return 24;
        case 5:
            return 25;
        }
        goto fallback26;

    case 8:
fallback26:
        switch (state) {
        case 4:
            return 26;
        case 5:
            return 27;
        }
        return 28;

    case 9:
        return 28;
    case 10:
        return 29;
    case 11:
        return 30;
    case 12:
        return 31;
    }

    return -1;
}
