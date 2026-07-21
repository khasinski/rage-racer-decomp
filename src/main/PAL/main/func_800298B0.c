#include "common.h"

/*
 * func_800298B0: GTE depth-cue color + perspective transform (DPCS/INTPL/RTPS/RTPT/NCLIP), nested loops. Hand-written asm: frameless, custom register-argument convention. Unmatchable as compiled C.
 */
asm(
    ".set noreorder\n"
    ".set noat\n"
    ".globl func_800298B0\n"
    "func_800298B0:\n"
    ".word 0xCB200000\n"  /* lwc2 */
    ".word 0xCB210004\n"  /* lwc2 */
    ".word 0x8DCC0000\n"
    ".word 0x4A180001\n"  /* gte_rtps (RTPS) */
    ".word 0x8DCD0004\n"
    ".word 0xAC8C00A8\n"
    ".word 0xAC8D00AC\n"
    ".word 0x8DEC0000\n"
    ".word 0x8DED0004\n"
    ".word 0xAC8C00B0\n"
    ".word 0xAC8D00B4\n"
    ".word 0x8F0C0000\n"
    ".word 0x8F0D0004\n"
    ".word 0xAC8C00B8\n"
    ".word 0xAC8D00BC\n"
    ".word 0x8F2C0000\n"
    ".word 0x8F2D0004\n"
    ".word 0xAC8C00C0\n"
    ".word 0xAC8D00C4\n"
    ".word 0xC9060008\n"  /* lwc2 */
    ".word 0x8D0C000C\n"
    ".word 0x8C990084\n"
    ".word 0x4A780010\n"  /* cop2 (GTE cmd) */
    ".word 0x8D0D0010\n"
    ".word 0x01996021\n"
    ".word 0xAC8C00C8\n"
    ".word 0xAC8D00CC\n"
    ".word 0x950C0014\n"
    ".word 0x950D0016\n"
    ".word 0xA48C00D0\n"
    ".word 0xA48D00D2\n"
    ".word 0xE89600A0\n"  /* swc2 */
    ".word 0x00C7C825\n"
    ".word 0x1320012A\n"
    ".word 0x4859A800\n"  /* cfc2 */
    ".word 0x4858B000\n"  /* cfc2 */
    ".word 0x484FB800\n"  /* cfc2 */
    ".word 0xAC990110\n"
    ".word 0xAC980114\n"
    ".word 0xAC8F0118\n"
    ".word 0x24191000\n"
    ".word 0x00D93006\n"
    ".word 0x00F93806\n"
    ".word 0x8C9900A8\n"
    ".word 0x8C9800AC\n"
    ".word 0xAC9900DC\n"
    ".word 0xAC9800E0\n"
    ".word 0x8C9900B8\n"
    ".word 0x8C9800BC\n"
    ".word 0xAC9900EC\n"
    ".word 0xAC9800F0\n"
    ".word 0x949900C8\n"
    ".word 0x949800D0\n"
    ".word 0xA49900F6\n"
    ".word 0xA49800FA\n"
    ".word 0x24021000\n"
    ".word 0x00461023\n"
    ".word 0x48824000\n"  /* mtc2 */
    ".word 0x8C9900DC\n"
    ".word 0x8C9800E0\n"
    ".word 0xAC9900D4\n"
    ".word 0xAC9800D8\n"
    ".word 0x8C9900EC\n"
    ".word 0x8C9800F0\n"
    ".word 0xAC9900E4\n"
    ".word 0xAC9800E8\n"
    ".word 0x909900C8\n"
    ".word 0x909800C9\n"
    ".word 0x48D9A800\n"  /* ctc2 */
    ".word 0x48D8B000\n"  /* ctc2 */
    ".word 0x909900CC\n"
    ".word 0x909800CD\n"
    ".word 0x48994800\n"  /* mtc2 */
    ".word 0x48985000\n"  /* mtc2 */
    ".word 0x949900F6\n"
    ".word 0x949800FA\n"
    ".word 0xA49900F4\n"
    ".word 0xA49800F8\n"
    ".word 0x4A980011\n"  /* cop2 (GTE cmd) */
    ".word 0x909900D0\n"
    ".word 0x909800D1\n"
    ".word 0x908F00D2\n"
    ".word 0x908E00D3\n"
    ".word 0x48D9A800\n"  /* ctc2 */
    ".word 0x48D8B000\n"  /* ctc2 */
    ".word 0x48194800\n"  /* mfc2 */
    ".word 0x48185000\n"  /* mfc2 */
    ".word 0x488F4800\n"  /* mtc2 */
    ".word 0x488E5000\n"  /* mtc2 */
    ".word 0xA09900F6\n"
    ".word 0xA09800F7\n"
    ".word 0x4A980011\n"  /* cop2 (GTE cmd) */
    ".word 0x849900A8\n"
    ".word 0x849800AA\n"
    ".word 0x848F00AC\n"
    ".word 0x848E00B0\n"
    ".word 0x848D00B2\n"
    ".word 0x848C00B4\n"
    ".word 0x48D9A800\n"  /* ctc2 */
    ".word 0x48D8B000\n"  /* ctc2 */
    ".word 0x48CFB800\n"  /* ctc2 */
    ".word 0x48194800\n"  /* mfc2 */
    ".word 0x48185000\n"  /* mfc2 */
    ".word 0x488E4800\n"  /* mtc2 */
    ".word 0x488D5000\n"  /* mtc2 */
    ".word 0x488C5800\n"  /* mtc2 */
    ".word 0xA09900FA\n"
    ".word 0xA09800FB\n"
    ".word 0x4A980011\n"  /* cop2 (GTE cmd) */
    ".word 0x849900B8\n"
    ".word 0x849800BA\n"
    ".word 0x848F00BC\n"
    ".word 0x848E00C0\n"
    ".word 0x848D00C2\n"
    ".word 0x848C00C4\n"
    ".word 0x48D9A800\n"  /* ctc2 */
    ".word 0x48D8B000\n"  /* ctc2 */
    ".word 0x48CFB800\n"  /* ctc2 */
    ".word 0x48194800\n"  /* mfc2 */
    ".word 0x48185000\n"  /* mfc2 */
    ".word 0x480F5800\n"  /* mfc2 */
    ".word 0x3339FFFF\n"
    ".word 0x0018C400\n"
    ".word 0x0338C825\n"
    ".word 0x488E4800\n"  /* mtc2 */
    ".word 0x488D5000\n"  /* mtc2 */
    ".word 0x488C5800\n"  /* mtc2 */
    ".word 0xAC9900DC\n"
    ".word 0xAC8F00E0\n"
    ".word 0x4A980011\n"  /* cop2 (GTE cmd) */
    ".word 0xC88000D4\n"  /* lwc2 */
    ".word 0xC88100D8\n"  /* lwc2 */
    ".word 0xC88200DC\n"  /* lwc2 */
    ".word 0xC88300E0\n"  /* lwc2 */
    ".word 0x48194800\n"  /* mfc2 */
    ".word 0x48185000\n"  /* mfc2 */
    ".word 0x480F5800\n"  /* mfc2 */
    ".word 0x3339FFFF\n"
    ".word 0x4A280030\n"  /* gte_rtpt (RTPT) */
    ".word 0x0018C400\n"
    ".word 0x0338C825\n"
    ".word 0xAC9900EC\n"
    ".word 0xAC8F00F0\n"
    ".word 0x949900F4\n"
    ".word 0x949800F6\n"
    ".word 0xA49900FC\n"
    ".word 0xA49800FE\n"
    ".word 0x24031000\n"
    ".word 0xE88C0100\n"  /* swc2 */
    ".word 0xE88D0104\n"  /* swc2 */
    ".word 0x00671823\n"
    ".word 0x48834000\n"  /* mtc2 */
    ".word 0x849900D4\n"
    ".word 0x849800D6\n"
    ".word 0x848F00D8\n"
    ".word 0x848E00E4\n"
    ".word 0x848D00E6\n"
    ".word 0x848C00E8\n"
    ".word 0x48D9A800\n"  /* ctc2 */
    ".word 0x48D8B000\n"  /* ctc2 */
    ".word 0x48CFB800\n"  /* ctc2 */
    ".word 0x488E4800\n"  /* mtc2 */
    ".word 0x488D5000\n"  /* mtc2 */
    ".word 0x488C5800\n"  /* mtc2 */
    ".word 0x849900DC\n"
    ".word 0x4A980011\n"  /* cop2 (GTE cmd) */
    ".word 0x849800DE\n"
    ".word 0x848F00E0\n"
    ".word 0x848E00EC\n"
    ".word 0x848D00EE\n"
    ".word 0x848C00F0\n"
    ".word 0x48D9A800\n"  /* ctc2 */
    ".word 0x48D8B000\n"  /* ctc2 */
    ".word 0x48CFB800\n"  /* ctc2 */
    ".word 0x48194800\n"  /* mfc2 */
    ".word 0x48185000\n"  /* mfc2 */
    ".word 0x480F5800\n"  /* mfc2 */
    ".word 0x488E4800\n"  /* mtc2 */
    ".word 0x488D5000\n"  /* mtc2 */
    ".word 0x488C5800\n"  /* mtc2 */
    ".word 0x3339FFFF\n"
    ".word 0x4A980011\n"  /* cop2 (GTE cmd) */
    ".word 0x0018C400\n"
    ".word 0x0338C825\n"
    ".word 0x48990000\n"  /* mtc2 */
    ".word 0x488F0800\n"  /* mtc2 */
    ".word 0x909900F4\n"
    ".word 0x909800F5\n"
    ".word 0x908E00F8\n"
    ".word 0x908D00F9\n"
    ".word 0x48D9A800\n"  /* ctc2 */
    ".word 0x48D8B000\n"  /* ctc2 */
    ".word 0x48194800\n"  /* mfc2 */
    ".word 0x48185000\n"  /* mfc2 */
    ".word 0x480F5800\n"  /* mfc2 */
    ".word 0x488E4800\n"  /* mtc2 */
    ".word 0x488D5000\n"  /* mtc2 */
    ".word 0x3339FFFF\n"
    ".word 0x0018C400\n"
    ".word 0x0338C825\n"
    ".word 0x48991000\n"  /* mtc2 */
    ".word 0x488F1800\n"  /* mtc2 */
    ".word 0x948F00CA\n"
    ".word 0x4A980011\n"  /* cop2 (GTE cmd) */
    ".word 0x949900FC\n"
    ".word 0x949800FE\n"
    ".word 0x000F7C00\n"
    ".word 0x032FC825\n"
    ".word 0xACB9000C\n"
    ".word 0xA4B8001C\n"
    ".word 0x909900F6\n"
    ".word 0x909800F7\n"
    ".word 0x908E00FA\n"
    ".word 0x908D00FB\n"
    ".word 0x948F00CE\n"
    ".word 0x48D9A800\n"  /* ctc2 */
    ".word 0x48D8B000\n"  /* ctc2 */
    ".word 0x000F7C00\n"
    ".word 0x48194800\n"  /* mfc2 */
    ".word 0x48185000\n"  /* mfc2 */
    ".word 0x488E4800\n"  /* mtc2 */
    ".word 0x488D5000\n"  /* mtc2 */
    ".word 0x0018C200\n"
    ".word 0x4A980011\n"  /* cop2 (GTE cmd) */
    ".word 0x333900FF\n"
    ".word 0x0338C825\n"
    ".word 0x032FC825\n"
    ".word 0xA49900FC\n"
    ".word 0xACB90014\n"
    ".word 0x48194800\n"  /* mfc2 */
    ".word 0x48185000\n"  /* mfc2 */
    ".word 0x4A280030\n"  /* gte_rtpt (RTPT) */
    ".word 0x333900FF\n"
    ".word 0x0018C200\n"
    ".word 0x0338C825\n"
    ".word 0xA49900FE\n"
    ".word 0xA4B90024\n"
    ".word 0xE88C0108\n"  /* swc2 */
    ".word 0xE88D010C\n"  /* swc2 */
    ".word 0xC88E0104\n"  /* lwc2 */
    ".word 0x8C990068\n"
    ".word 0x00000000\n"
    ".word 0x4B400006\n"  /* gte_nclip (NCLIP) */
    ".word 0x4818C000\n"  /* mfc2 */
    ".word 0x17200002\n"
    ".word 0x00000000\n"
    ".word 0x0018C022\n"
    ".word 0x1F00000A\n"
    ".word 0x00000000\n"
    ".word 0xC88D0100\n"  /* lwc2 */
    ".word 0x00000000\n"
    ".word 0x00000000\n"
    ".word 0x4B400006\n"  /* gte_nclip (NCLIP) */
    ".word 0x4818C000\n"  /* mfc2 */
    ".word 0x13200002\n"
    ".word 0xC88D010C\n"  /* lwc2 */
    ".word 0x0018C022\n"
    ".word 0x1B00003D\n"
    ".word 0x8499007C\n"
    ".word 0x84980078\n"
    ".word 0x848F0100\n"
    ".word 0x848E0104\n"
    ".word 0x848D0108\n"
    ".word 0x848C010C\n"
    ".word 0x032F082A\n"
    ".word 0x10200006\n"
    ".word 0x032E082A\n"
    ".word 0x10200004\n"
    ".word 0x032D082A\n"
    ".word 0x10200002\n"
    ".word 0x032C082A\n"
    ".word 0x1420002F\n"
    ".word 0x01F8082A\n"
    ".word 0x10200006\n"
    ".word 0x01D8082A\n"
    ".word 0x10200004\n"
    ".word 0x01B8082A\n"
    ".word 0x10200002\n"
    ".word 0x0198082A\n"
    ".word 0x14200027\n"
    ".word 0x8499007E\n"
    ".word 0x8498007A\n"
    ".word 0x848F0102\n"
    ".word 0x848E0106\n"
    ".word 0x848D010A\n"
    ".word 0x848C010E\n"
    ".word 0x032F082A\n"
    ".word 0x10200006\n"
    ".word 0x032E082A\n"
    ".word 0x10200004\n"
    ".word 0x032D082A\n"
    ".word 0x10200002\n"
    ".word 0x032C082A\n"
    ".word 0x14200019\n"
    ".word 0x01F8082A\n"
    ".word 0x10200006\n"
    ".word 0x01D8082A\n"
    ".word 0x10200004\n"
    ".word 0x01B8082A\n"
    ".word 0x10200002\n"
    ".word 0x0198082A\n"
    ".word 0x14200011\n"
    ".word 0x8C990100\n"
    ".word 0x8C980104\n"
    ".word 0x8C8F00A0\n"
    ".word 0xACB90008\n"
    ".word 0xACB80018\n"
    ".word 0xACAF0004\n"
    ".word 0x8C9900A4\n"
    ".word 0x0005C200\n"
    ".word 0x8F2F0000\n"
    ".word 0x0018C202\n"
    ".word 0x3C010900\n"
    ".word 0x01E17825\n"
    ".word 0xAF380000\n"
    ".word 0xACAF0000\n"
    ".word 0xE8AC0010\n"  /* swc2 */
    ".word 0xE8AD0020\n"  /* swc2 */
    ".word 0x20A50028\n"
    ".word 0xE88C0100\n"  /* swc2 */
    ".word 0x1460FF55\n"
    ".word 0xE88D0104\n"  /* swc2 */
    ".word 0x1440FEF5\n"
    ".word 0x00000000\n"
    ".word 0x8C990110\n"
    ".word 0x8C980114\n"
    ".word 0x8C8F0118\n"
    ".word 0x48D9A800\n"  /* ctc2 */
    ".word 0x48D8B000\n"  /* ctc2 */
    ".word 0x48CFB800\n"  /* ctc2 */
    ".word 0x03E00008\n"
    ".word 0x00A01021\n"
    ".word 0x8C990100\n"
    ".word 0x8C980104\n"
    ".word 0xACB90008\n"
    ".word 0xACB80010\n"
    ".word 0x8C990108\n"
    ".word 0x8C98010C\n"
    ".word 0xACB90018\n"
    ".word 0xACB80020\n"
    ".word 0x8C9900C8\n"
    ".word 0x8C9800CC\n"
    ".word 0xACB9000C\n"
    ".word 0xACB80014\n"
    ".word 0x949900D0\n"
    ".word 0x949800D2\n"
    ".word 0x8C8F00A0\n"
    ".word 0xA4B9001C\n"
    ".word 0xA4B80024\n"
    ".word 0xACAF0004\n"
    ".word 0x8C9900A4\n"
    ".word 0x0005C200\n"
    ".word 0x8F2F0000\n"
    ".word 0x0018C202\n"
    ".word 0x3C010900\n"
    ".word 0x01E17825\n"
    ".word 0xAF380000\n"
    ".word 0xACAF0000\n"
    ".word 0x03E00008\n"
    ".word 0x20A20028\n"
);
