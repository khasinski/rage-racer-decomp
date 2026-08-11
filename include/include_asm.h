#ifndef RAGE_PC_INCLUDE_ASM_H
#define RAGE_PC_INCLUDE_ASM_H

#if !defined(M2CTX) && !defined(PERMUTER)

#ifndef INCLUDE_ASM_USE_MACRO_INC
#define INCLUDE_ASM_USE_MACRO_INC 1
#endif

#ifndef INCLUDE_ASM
#define INCLUDE_ASM(FOLDER, NAME) \
    void __maspsx_include_asm_hack_##NAME(void) { \
        __asm__( \
            ".text # maspsx-keep\n" \
            "\t.align\t2 # maspsx-keep\n" \
            "\t.set\tnoreorder # maspsx-keep\n" \
            "\t.set\tnoat # maspsx-keep\n" \
            "\t.include \"" FOLDER "/" #NAME ".s\" # maspsx-keep\n" \
            "\t.set\treorder # maspsx-keep\n" \
            "\t.set\tat # maspsx-keep\n" \
        ); \
    }
#endif

#ifndef INCLUDE_RODATA
#define INCLUDE_RODATA(FOLDER, NAME) \
    __asm__( \
        ".section .rodata\n" \
        "    .include \"" FOLDER "/" #NAME ".s\"\n" \
        ".section .text" \
    )
#endif

/* A block that is assembly on purpose and always will be: a kernel entry
   reached by `syscall`, or a BIOS call that jumps through a register. It
   expands exactly like INCLUDE_ASM but has to be spelled differently, because
   INCLUDE_ASM means "not decompiled yet" everywhere else in this tree.
   tools/scripts/gen_nonmatching_asm.py reads the second argument of every
   INCLUDE_ASM as a symbol it must disassemble out of the EXE, and here the
   argument names a checked-in file instead. */
#ifndef HANDWRITTEN_ASM
#define HANDWRITTEN_ASM(FOLDER, NAME) INCLUDE_ASM(FOLDER, NAME)
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

#ifndef HANDWRITTEN_ASM
#define HANDWRITTEN_ASM(FOLDER, NAME)
#endif

#endif

#endif
