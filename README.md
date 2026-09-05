# Rage Racer Decompilation

![functions](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fkhasinski%2Frage-racer-decomp%2Fmain%2Fdocs%2Fbadges%2Ffunctions.json) ![code](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fkhasinski%2Frage-racer-decomp%2Fmain%2Fdocs%2Fbadges%2Fcode.json)

A byte-exact source reconstruction of the PAL PlayStation release of
Rage Racer, target `SCES-006.50`, plus byte-exact USA and Japanese regional
builds which reuse the PAL C wherever the compiler output translates directly.

This repository is source-only. It does not contain disc images, extracted game
assets, CDDA/XA data, Sony PSYQ SDK files, compiler binaries, generated split
output, local post-build rewrite passes, or decompilation scratch work.
Contributors must provide their own legally obtained copy of the game and local
toolchains.

## Targets

| Version | Executable | SHA-1 | Entry PC |
|---|---|---|---:|
| PAL | `SCES_006.50` | `2913e15648eddef40821c5f666460abc04155ee6` | `0x800630B4` |
| USA | `SLUS_004.03` | `2661e8bf18d209c98fd70d33e18ab88b10abd52b` | `0x80062DC4` |
| Japan v1.0 | `SLPS_006.00` | `f0ca386e1c7b2c5961b8c2a53cc751a83ae0d406` | `0x80062910` |
| Japan v1.1 | `SLPS_006.00` | `bfa7a4cf466480133c10845eae632a0c4e122360` | `0x800629F8` |

All load at `0x80010000`. PAL and USA have a `0x8B000`-byte payload;
the two Japanese executables have a `0x8A800`-byte payload.

## Progress

**Executable matching is complete.** The PAL build matches all 1200 reported
functions and all 423420 code bytes, and the linked executable is byte-identical
to retail.

| Scope | Functions | Code bytes | Data bytes |
|---|---:|---:|---:|
| Game code | 704 / 704 (100.00%) | 100.00% | 99.88% |
| PsyQ libraries | 496 / 496 (100.00%) | 100.00% | 4.18% |
| **Whole executable** | **1200 / 1200 (100.00%)** | **100.00%** | **99.82%** |

The table comes from objdiff comparisons between objects built from this tree
and objects reconstructed directly from the retail executable. The remaining
data percentage is unpaired padding or unnamed table data, not a difference in
the linked image. Initialized reference data is reconstructed from retail bytes;
BSS has no file contents and its matching score measures layout only.
The report reclassifies 56 audited data bytes embedded in `.text`; the original
objdiff output is retained as `build/<version>/report.raw.json`, alongside the
correction log `report.embedded-data.json`.

Matching does not imply complete C decompilation. The PAL report also exposes
this separate partition of the same units:

| Source form | Units | Code bytes in these units |
|---|---:|---:|
| C units (header intrinsics allowed) | 247 | 214012 |
| C units with inline ASM or compiler constraints | 96 | 189684 |
| Units with retained ASM or raw opcodes | 21 | 19724 |
| Extracted data and BSS layout | 25 | 0 |

`complete` marks C units without included assembly or raw opcode arrays. Inline instructions, header
intrinsics, register pins and barriers are permitted and disclosed above.
Mixed C/ASM units are counted entirely under retained assembly, so these are
unit-level counts, not percentages of instructions decompiled to C. The
assembly units remain in the matching denominator even when `complete` is false.
Regenerate with `make report progress`; see [reporting policy](docs/ASM_AND_GTE_POLICY.md).

The regional targets also have complete source coverage. Unchanged translation
units reuse PAL C; actual regional differences live in separate USA/Japanese C
files. Reconstructed GTE/COP2, MDEC and startup assembly is included through
`HANDWRITTEN_ASM`. These wrappers do not count as completed C. Handwritten
origin is inferred from instruction idioms and calling conventions; the macro
name itself is not evidence of the original source language.

| Target | Source-covered text | Coverage | Reused PAL C units | Reused PAL text |
|---|---:|---:|---:|---:|
| USA | 422988 / 422988 bytes | 100% | 328 / 360 | 358228 bytes |
| Japan v1.0 | 422316 / 422316 bytes | 100% | 314 / 359 | 340900 bytes |
| Japan v1.1 | 422544 / 422544 bytes | 100% | 319 / 359 | 344820 bytes |

`make port VERSION=<region>` now rejects a gap or overlap anywhere in the
executable text range. Every build is additionally verified against its retail
SHA-1. The exact portable/regional selection and coverage totals are recorded
in `configs/<version>/portable_text.json`.

## Layout

- `configs/PAL/` - splat config, symbols, relocs, checksum for `SCES_006.50`.
- `configs/USA/`, `configs/JAP10/`, `configs/JAP11/` - regional splits,
  mapped symbols, and direct-PAL-port reports.
- `src/main/` - decompiled C translation units for the main executable.
- `include/` - project headers and local PSYQ-compatible declarations.
- `tools/scripts/` - project-specific build and analysis helpers.
- `docs/names.md` - the naming evidence: what each function and global is, and
  why. Source files are named after their subject, not after whichever function
  happens to sit first in them.
Generated directories such as `asm/`, `linkers/`, `build/`, `assets/`, and
`disc/` are intentionally ignored, along with local scratch/proposal work.

## Toolchain

Linking and assembling use standard GNU binutils on `PATH`:

```text
mipsel-none-elf-as
mipsel-none-elf-ld
mipsel-none-elf-objcopy
```

Compiling matching C uses old PlayStation GCC variants plus
[maspsx](https://github.com/mkst/maspsx) to translate Psy-Q style assembly to
GNU assembler input. The wrapper fetches public helper tooling into
`build/toolchain/` when needed, but does not publish compiler binaries or SDK
material in this repository.

Expected local inputs:

```text
assets/PAL/main.exe
assets/USA/main.exe
assets/JAP10/main.exe
assets/JAP11/main.exe
```

## Quick Start

```sh
python3 -m venv .venv
.venv/bin/pip install -r requirements.txt

mkdir -p assets/PAL
cp /path/to/your/SCES_006.50 assets/PAL/main.exe

make split VERSION=PAL
make build VERSION=PAL
make check VERSION=PAL
make progress
```

Use `VERSION=USA`, `VERSION=JAP10`, or `VERSION=JAP11` for another target.
`python3 tools/scripts/stage_discs.py --version JAP10` can stage one configured
local dump; repeat `--version` or omit it to stage several or all versions.

`make split` regenerates `asm/` and `linkers/` from the user-supplied
executable, `make build` compiles and links `build/PAL/main.exe`, and
`make check` verifies its SHA-1 against retail. `make progress` refreshes the
badge JSON and prints the table shown above.

## License

See [LICENSE.md](LICENSE.md). Game code and data remain the property of their
respective owners; this repository contains only clean-room reimplementation
source.
