#include "common.h"

/*
 * func_80028874: GTE primitive-builder. Hand-written asm: frameless, custom register args (t6/t7/t8/t9/v0), ra saved into context struct (a0->0x9C) not stack. Cluster with func_80028120/28C54. Unmatchable as compiled C.
 */
asm(
    ".set noreorder\n"
    ".set noat\n"
    ".globl func_80028874\n"
    "func_80028874:\n"
    ".word 0xAC8E00C8\n"
    ".word 0xAC8F00CC\n"
    ".word 0xA49800D0\n"
    ".word 0xA49900D2\n"
    ".word 0x1580001E\n"
    ".word 0xAC8200A4\n"
    ".word 0x8C8E0100\n"
    ".word 0x8C8F0104\n"
    ".word 0x8C980108\n"
    ".word 0x8C99010C\n"
    ".word 0xACAE0008\n"
    ".word 0xACAE0020\n"
    ".word 0xACAF000C\n"
    ".word 0xACB80024\n"
    ".word 0xACB90010\n"
    ".word 0x8D0C0018\n"
    ".word 0xACB90028\n"
    ".word 0xACAC0004\n"
    ".word 0xACAC001C\n"
    ".word 0x3C0C5555\n"
    ".word 0x358C5555\n"
    ".word 0xACAC0014\n"
    ".word 0xACAC002C\n"
    ".word 0x24430100\n"
    ".word 0x00056A00\n"
    ".word 0x8C6C0000\n"
    ".word 0x000D6A02\n"
    ".word 0x25AE0018\n"
    ".word 0x3C0F0500\n"
    ".word 0x018F6025\n"
    ".word 0x01AF6825\n"
    ".word 0xAC6E0000\n"
    ".word 0xACAC0000\n"
    ".word 0xACAD0018\n"
    ".word 0x20A50030\n"
    ".word 0xAC9F009C\n"
    ".word 0x4859A800\n"  /* cfc2 */
    ".word 0x4858B000\n"  /* cfc2 */
    ".word 0x484FB800\n"  /* cfc2 */
    ".word 0xAC990110\n"
    ".word 0xAC980114\n"
    ".word 0xAC8F0118\n"
    ".word 0x24191000\n"
    ".word 0x00D93006\n"
    ".word 0x00F93806\n"
    ".word 0x849900A8\n"
    ".word 0x849800AA\n"
    ".word 0x848F00AC\n"
    ".word 0xAC9900D4\n"
    ".word 0xAC9800D8\n"
    ".word 0xAC8F00DC\n"
    ".word 0x849900B8\n"
    ".word 0x849800BA\n"
    ".word 0x848F00BC\n"
    ".word 0xAC9900E4\n"
    ".word 0xAC9800E8\n"
    ".word 0xAC8F00EC\n"
    ".word 0x909900C8\n"
    ".word 0x909800C9\n"
    ".word 0xAC9900FC\n"
    ".word 0xAC980100\n"
    ".word 0x909900D0\n"
    ".word 0x909800D1\n"
    ".word 0xAC9900F4\n"
    ".word 0xAC9800F8\n"
    ".word 0x0C00A315\n"
    ".word 0x00806021\n"
    ".word 0x24021000\n"
    ".word 0x00461023\n"
    ".word 0x48824000\n"  /* mtc2 */
    ".word 0x909900C8\n"
    ".word 0x909800C9\n"
    ".word 0x48D9A800\n"  /* ctc2 */
    ".word 0x48D8B000\n"  /* ctc2 */
    ".word 0x909900CC\n"
    ".word 0x909800CD\n"
    ".word 0x48994800\n"  /* mtc2 */
    ".word 0x48985000\n"  /* mtc2 */
    ".word 0x909900D0\n"
    ".word 0x4A980011\n"  /* cop2 (GTE cmd) */
    ".word 0x909800D1\n"
    ".word 0x908F00D2\n"
    ".word 0x908E00D3\n"
    ".word 0xE88900FC\n"  /* swc2 */
    ".word 0xE88A0100\n"  /* swc2 */
    ".word 0x48D9A800\n"  /* ctc2 */
    ".word 0x48D8B000\n"  /* ctc2 */
    ".word 0x488F4800\n"  /* mtc2 */
    ".word 0x488E5000\n"  /* mtc2 */
    ".word 0x849900A8\n"
    ".word 0x4A980011\n"  /* cop2 (GTE cmd) */
    ".word 0x849800AA\n"
    ".word 0x848F00AC\n"
    ".word 0x848E00B0\n"
    ".word 0x848D00B2\n"
    ".word 0x848C00B4\n"
    ".word 0xE88900F4\n"  /* swc2 */
    ".word 0xE88A00F8\n"  /* swc2 */
    ".word 0x48D9A800\n"  /* ctc2 */
    ".word 0x48D8B000\n"  /* ctc2 */
    ".word 0x48CFB800\n"  /* ctc2 */
    ".word 0x488E4800\n"  /* mtc2 */
    ".word 0x488D5000\n"  /* mtc2 */
    ".word 0x488C5800\n"  /* mtc2 */
    ".word 0x849900B8\n"
    ".word 0x4A980011\n"  /* cop2 (GTE cmd) */
    ".word 0x849800BA\n"
    ".word 0x848F00BC\n"
    ".word 0x848E00C0\n"
    ".word 0x848D00C2\n"
    ".word 0x848C00C4\n"
    ".word 0xE88900D4\n"  /* swc2 */
    ".word 0xE88A00D8\n"  /* swc2 */
    ".word 0xE88B00DC\n"  /* swc2 */
    ".word 0x48D9A800\n"  /* ctc2 */
    ".word 0x48D8B000\n"  /* ctc2 */
    ".word 0x48CFB800\n"  /* ctc2 */
    ".word 0x488E4800\n"  /* mtc2 */
    ".word 0x488C5800\n"  /* mtc2 */
    ".word 0x488D5000\n"  /* mtc2 */
    ".word 0x248C0110\n"
    ".word 0x4A980011\n"  /* cop2 (GTE cmd) */
    ".word 0xE88900E4\n"  /* swc2 */
    ".word 0xE88A00E8\n"  /* swc2 */
    ".word 0x0C00A315\n"
    ".word 0xE88B00EC\n"  /* swc2 */
    ".word 0x00806021\n"
    ".word 0x24031000\n"
    ".word 0xC98C016C\n"  /* lwc2 */
    ".word 0xC98D027C\n"  /* lwc2 */
    ".word 0xC98E0174\n"  /* lwc2 */
    ".word 0x8C990068\n"
    ".word 0x948E00CA\n"
    ".word 0x4B400006\n"  /* gte_nclip (NCLIP) */
    ".word 0x958F0172\n"
    ".word 0x000E7400\n"
    ".word 0x01EE7825\n"
    ".word 0xACAF000C\n"
    ".word 0x4818C000\n"  /* mfc2 */
    ".word 0x13200002\n"
    ".word 0x00000000\n"
    ".word 0x0018C022\n"
    ".word 0x1F00000A\n"
    ".word 0x00000000\n"
    ".word 0xC98C0284\n"  /* lwc2 */
    ".word 0x00000000\n"
    ".word 0x00000000\n"
    ".word 0x4B400006\n"  /* gte_nclip (NCLIP) */
    ".word 0x4818C000\n"  /* mfc2 */
    ".word 0x17200002\n"
    ".word 0x00000000\n"
    ".word 0x0018C022\n"
    ".word 0x1B00004A\n"
    ".word 0x8499007C\n"
    ".word 0x8598016C\n"
    ".word 0x858F027C\n"
    ".word 0x858E0174\n"
    ".word 0x858D0284\n"
    ".word 0x0338082A\n"
    ".word 0x10200006\n"
    ".word 0x032F082A\n"
    ".word 0x10200004\n"
    ".word 0x032E082A\n"
    ".word 0x10200002\n"
    ".word 0x032D082A\n"
    ".word 0x1420003D\n"
    ".word 0x84990078\n"
    ".word 0x00000000\n"
    ".word 0x0319082A\n"
    ".word 0x10200006\n"
    ".word 0x01F9082A\n"
    ".word 0x10200004\n"
    ".word 0x01D9082A\n"
    ".word 0x10200002\n"
    ".word 0x01B9082A\n"
    ".word 0x14200033\n"
    ".word 0x8499007E\n"
    ".word 0x8598016E\n"
    ".word 0x858F027E\n"
    ".word 0x858E0176\n"
    ".word 0x858D0286\n"
    ".word 0x0338082A\n"
    ".word 0x10200006\n"
    ".word 0x032F082A\n"
    ".word 0x10200004\n"
    ".word 0x032E082A\n"
    ".word 0x10200002\n"
    ".word 0x032D082A\n"
    ".word 0x14200026\n"
    ".word 0x8499007A\n"
    ".word 0x00000000\n"
    ".word 0x0319082A\n"
    ".word 0x10200006\n"
    ".word 0x01F9082A\n"
    ".word 0x10200004\n"
    ".word 0x01D9082A\n"
    ".word 0x10200002\n"
    ".word 0x01B9082A\n"
    ".word 0x1420001C\n"
    ".word 0x8D99016C\n"
    ".word 0x8D98027C\n"
    ".word 0x8C8F00A0\n"
    ".word 0xACB90008\n"
    ".word 0xACB80010\n"
    ".word 0xACAF0004\n"
    ".word 0x8D990174\n"
    ".word 0x8D8D0284\n"
    ".word 0xACB90018\n"
    ".word 0xACAD0020\n"
    ".word 0x949800CE\n"
    ".word 0x95990282\n"
    ".word 0x0018C400\n"
    ".word 0x0338C825\n"
    ".word 0xACB90014\n"
    ".word 0x9598017A\n"
    ".word 0x9599028A\n"
    ".word 0xA4B8001C\n"
    ".word 0xA4B90024\n"
    ".word 0x8C9900A4\n"
    ".word 0x0005C200\n"
    ".word 0x8F2F0000\n"
    ".word 0x0018C202\n"
    ".word 0x3C010900\n"
    ".word 0x01E17825\n"
    ".word 0xAF380000\n"
    ".word 0xACAF0000\n"
    ".word 0x20A50028\n"
    ".word 0x8D98027C\n"
    ".word 0x8D990280\n"
    ".word 0xAD98016C\n"
    ".word 0xAD990170\n"
    ".word 0x00671823\n"
    ".word 0x1460FF97\n"
    ".word 0x258C0008\n"
    ".word 0x8D98027C\n"
    ".word 0x8D990280\n"
    ".word 0xAD98016C\n"
    ".word 0x1440FF56\n"
    ".word 0xAD990170\n"
    ".word 0x8C990110\n"
    ".word 0x8C980114\n"
    ".word 0x8C8F0118\n"
    ".word 0x8C9F009C\n"
    ".word 0x48D9A800\n"  /* ctc2 */
    ".word 0x48D8B000\n"  /* ctc2 */
    ".word 0x48CFB800\n"  /* ctc2 */
    ".word 0x03E00008\n"
    ".word 0x00000000\n"
);
