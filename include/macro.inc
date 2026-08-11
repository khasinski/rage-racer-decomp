# This file is used by modern gas.
# Defines the expected assembly macros

# Evaluate this file only once in case it's included more than once
.ifndef _MACRO_INC_GUARD
.internal _MACRO_INC_GUARD
.set _MACRO_INC_GUARD, 1

# A function symbol.
.macro glabel label, visibility=global
    .\visibility \label
    .type \label, @function
    \label:
        .ent \label
.endm

# The end of a function symbol.
.macro endlabel label
    .size \label, . - \label
    .end \label
.endm

# An alternative entry to a function.
.macro alabel label, visibility=global
    .\visibility \label
    .type \label, @function
    \label:
        .aent \label
.endm

# A label referenced by an error handler table.
.macro ehlabel label, visibility=global
    .\visibility \label
    \label:
.endm


# A label referenced by a jumptable.
.macro jlabel label, visibility=global
    .\visibility \label
    \label:
.endm


# A data symbol.
.macro dlabel label, visibility=global
    .\visibility \label
    .type \label, @object
    \label:
.endm

# End of a data symbol.
.macro enddlabel label
    .size \label, . - \label
.endm


# Label to signal the symbol haven't been matched yet.
#
# Emits nothing. It used to declare \label.NON_MATCHING as a second global
# object at the symbol's own address, for asm-differ to spot. Nothing is
# unmatched any more, and the alias was not free: two symbols sharing one
# address stop objdiff pairing that section at all, which reported every
# constant blob in the game as 129 KB of unmatched data.
#
# The instruction and data words are untouched either way - this only removes
# symbol table entries - so restoring the body is a one-line change if some
# future tool wants the marker back.
.macro nonmatching label, size=1
.endm

.include "gte_macros.inc"

.endif
