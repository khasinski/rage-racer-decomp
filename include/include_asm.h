#ifndef RAGE_PC_INCLUDE_ASM_H
#define RAGE_PC_INCLUDE_ASM_H

#if !defined(M2CTX) && !defined(PERMUTER)

#ifndef INCLUDE_ASM_USE_MACRO_INC
#define INCLUDE_ASM_USE_MACRO_INC 1
#endif

#ifndef INCLUDE_ASM
#define INCLUDE_ASM(FOLDER, NAME) \
    __asm__( \
        ".text\n" \
        "\t.align\t2\n" \
        "\t.set\tnoat\n" \
        "\t.set\tnoreorder\n" \
        "\t.include \"" FOLDER "/" #NAME ".s\"\n" \
        "\t.set\treorder\n" \
        "\t.set\tat\n" \
    )
#endif

#ifndef INCLUDE_RODATA
#define INCLUDE_RODATA(FOLDER, NAME) \
    __asm__( \
        ".section .rodata\n" \
        "    .include \"" FOLDER "/" #NAME ".s\"\n" \
        ".section .text" \
    )
#endif

#if INCLUDE_ASM_USE_MACRO_INC
__asm__(".include \"include/macro.inc\"\n");
#else
__asm__(".include \"include/labels.inc\"\n");
#endif

#else

#ifndef INCLUDE_ASM
#define INCLUDE_ASM(FOLDER, NAME)
#endif

#ifndef INCLUDE_RODATA
#define INCLUDE_RODATA(FOLDER, NAME)
#endif

#endif

#endif
