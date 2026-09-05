#ifndef RAGE_PC_INCLUDE_ASM_H
#define RAGE_PC_INCLUDE_ASM_H

#if !defined(M2CTX) && !defined(PERMUTER)

/* A reconstructed assembly block: a kernel entry reached by `syscall`, a BIOS call that jumps
   through a register, a GTE routine that moves coprocessor control registers.
   The assembly lives in a .s beside the source and is pulled in here so that
   it lands at the right offset within the unit.

   Used both for mixed C/ASM units and assembly-only wrappers. In either case
   the report classifies the unit as included assembly, not completed C.
   The historical macro name does not prove the original source language;
   see docs/ASM_AND_GTE_POLICY.md for the reporting convention.

   The block is wrapped in a throwaway function because the compiler will not
   carry a file-scope `.include` through maspsx untouched. maspsx drops the
   body and leaves the name behind as an undefined reference, which is harmless
   in the image. */
#ifndef HANDWRITTEN_ASM
#define HANDWRITTEN_ASM(FOLDER, NAME) \
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

__asm__(".include \"include/macro.inc\"\n");

#else

#ifndef HANDWRITTEN_ASM
#define HANDWRITTEN_ASM(FOLDER, NAME)
#endif

#endif

#endif
