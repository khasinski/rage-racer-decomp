# Assembly and progress reporting

The project reproduces the retail executable byte for byte. This is a matching
claim, not a claim that every routine has been rewritten in C or that the
original source language is known.

The report accounts for every object placed by the linker. There is no option
to omit an unscorable object. The game/PsyQ categories partition ownership;
the source-form categories independently partition implementation form. Do not
add the two sets of categories together.

`complete` means a C translation unit with no included assembly block or raw
opcode array. Nonzero initializers placed directly in `.text` count as retained
assembly; zero padding does not. C may
use inline instructions, GTE/COP2 header intrinsics, register constraints and
empty compiler barriers. Direct inline ASM is exposed in its own source-form
category, including register bindings and barriers that emit no instructions.
These categories describe source syntax, not the original author's language.

`HANDWRITTEN_ASM` wraps a reconstructed `.s` file. Both assembly-only wrappers
and mixed units are classified as included assembly and have `complete=false`.
They still participate in all matching totals. Their entire code size belongs
to that category; it is not a count of how many bytes within the unit are ASM.
The wrapper exists to preserve compilation and instruction ordering. Changing
the extension to `.c` does not constitute decompilation.

Extracted initialized data and BSS have `complete=false`: preserving data or
describing reserved memory is not C decompilation. Reference data is read from
the retail EXE. Symbol boundaries and relocation locations come from the base
objects, while reference values and relocation addends are reconstructed from
retail bytes. BSS contains no bytes in the EXE; its score checks layout only.

Reference code is disassembled from retail. Symbol names, types and missing
boundaries are supplied from configured symbols and the verified build to pair
objects in objdiff. An additional report audit checks every linked unit and
rejects overlapping ranges, ranges outside their sections, and unreported
nonzero text bytes. Unreported zero padding is counted explicitly. Thus
function counts mean *reported symbol ranges*, not an
independent discovery of original functions. SHA-1 checks the entire linked
image independently of that partition. Section-size checks include data and
BSS and ensure that every linked object has a corresponding target.

objdiff v3.8 groups report symbols by their containing section (see its
[section classification](https://github.com/encounter/objdiff/blob/v3.8.0/objdiff-core/src/obj/read.rs)).
Our `.text` includes a 16-byte stack-top table, a 4-byte MDEC buffer-size word,
and 36 bytes of decoder state storage. `configs/embedded_data.json` records
their source symbols, offsets and extents. Startup loads the table; MDEC
loads/stores the other two objects. The source assembly declares each as an
ELF object with an explicit size; BIOS code entries have separate boundaries.

`classify_embedded_data.py` validates those declarations against both object
files and moves their counts from code to data. It rejects any partial overlap
with a function and compares data bytes directly with the retail EXE. It also
accounts for the zero-filled state storage even if objdiff treated it as
padding. No bytes or units are dropped, and no match percentage is forced.
`report.raw.json` preserves unmodified objdiff output and
`report.embedded-data.json` records every reclassified range. `report.json`
contains the logical code/data totals used by decomp.dev. Its section extents
remain the physical ELF layout, including embedded data; they must not be
summed as though `.text` contained instructions only. Fuzzy section scores are
retained from objdiff; exact code/data and completion percentages are computed
from the corrected counts.

Handwritten origin is an inference requiring per-routine evidence, such as a
private register ABI, shared internal entry points or instruction patterns
that the known compiler does not emit. GTE/COP2 operations also need assembly
intrinsics regardless of the original source language. Neither the presence
of inline ASM nor failure to find matching C proves handwritten origin.

Historical notes saying that assembly is "excluded from progress" refer to
old metrics. Under the current convention it is excluded from completed C
units, but never from the executable matching denominator.

## Matching scaffolding cleanup

The first cleanup pass removed 16 of the 17 PAL game `RAW` accesses across
eight shared C files. Named, correctly typed pointers to existing fields
preserve the instruction ordering without the pointer-to-integer round trip.
These locals still help GCC 2.6.3 produce the matching schedule; they are not
evidence of the original source spelling or newly recovered structures.
No register pins, compiler barriers or volatile qualifiers were added.

One `RAW` remains in `UpdateRouteScenery`: a normal pointer to the indexed
duration field reverses the source operands of an `addu`, changing one byte.
The field-macro debt ceiling is now one. Pins (208) and empty barriers (132)
remain unchanged; individual barrier-removal trials in the modified text,
steering and environment code still changed instructions. Full linked EXEs
for PAL, USA, JAP10 and JAP11 passed their retail SHA-1 checks after cleanup.

A second pass removed both register pins from `DrawTimeValue` (`$10` and
`$4`) in the PAL and shared NTSC source files. Using the function parameters
directly instead of the redundant `savedX`, `savedY`, `savedColor` and
`localDivisor` copies lets GCC choose the retail registers and instruction
order without constraints. The function now contains no inline ASM. This
is a source-shape simplification, not a newly recovered structure. Removing
the pins alone had not matched; the surrounding copies mattered too.
The PAL register-pin ceiling is now 206, with 132 empty barriers unchanged.
