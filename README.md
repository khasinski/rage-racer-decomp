# Rage Racer Decompilation

![functions](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fkhasinski%2Frage-racer-decomp%2Fmain%2Fdocs%2Fbadges%2Ffunctions.json) ![code](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fkhasinski%2Frage-racer-decomp%2Fmain%2Fdocs%2Fbadges%2Fcode.json)

A byte-exact source reconstruction of the PAL PlayStation release of
Rage Racer, target `SCES-006.50`, plus byte-exact USA and Japanese regional
builds that reuse the PAL C wherever the compiler output translates directly,
and a byte-exact pre-release Demo build decompiled largely on its own terms.

This repository is source-only. It does not contain disc images, extracted game
assets, CDDA/XA data, Sony PSYQ SDK files, compiler binaries, generated split
output, local post-build rewrite passes, or decompilation scratch work.
Contributors must provide their own legally obtained copy of the game and local
toolchains.

## Targets

| Version | Executable | SHA-1 | Entry PC | Loads at |
|---|---|---|---:|---:|
| PAL | `SCES_006.50` | `2913e15648eddef40821c5f666460abc04155ee6` | `0x800630B4` | `0x80010000` |
| USA | `SLUS_004.03` | `2661e8bf18d209c98fd70d33e18ab88b10abd52b` | `0x80062DC4` | `0x80010000` |
| Japan v1.0 | `SLPS_006.00` | `f0ca386e1c7b2c5961b8c2a53cc751a83ae0d406` | `0x80062910` | `0x80010000` |
| Japan v1.1 | `SLPS_006.00` | `bfa7a4cf466480133c10845eae632a0c4e122360` | `0x800629F8` | `0x80010000` |
| Demo One v5 (27 Feb 1997) | `RAGE.EXE` | `6e706fd58000734707b2fe793161e5cc1ada7bed` | `0x80069EB8` | `0x80018000` |

PAL and USA have a `0x8B000`-byte payload; the two Japanese executables have
`0x8A800` bytes; the pre-release Demo has `0x8A000` bytes and loads `0x8000`
bytes higher than every retail build.

## Progress

**Executable matching is complete for every target above**, each build
byte-identical to its retail SHA-1. The table below is PAL, the primary
target; see `make report progress VERSION=<region>` for another build's own
numbers.

| Scope | Functions | Code bytes | Data bytes |
|---|---:|---:|---:|
| Game code | 704 / 704 (100.00%) | 100.00% | 99.88% |
| PsyQ libraries | 496 / 496 (100.00%) | 100.00% | 4.18% |
| **Whole executable** | **1200 / 1200 (100.00%)** | **100.00%** | **99.82%** |

The table comes from objdiff comparisons against objects reconstructed from
the retail executable. Remaining data percentage is unpaired padding or
unnamed table data, not a difference in the linked image; BSS matching
measures layout only.

Matching does not imply complete C decompilation. The PAL report also exposes
this separate partition of the same units:

| Source form | Units | Code bytes in these units |
|---|---:|---:|
| C units (header intrinsics allowed) | 247 | 214012 |
| C units with inline ASM or compiler constraints | 96 | 189684 |
| Units with retained ASM or raw opcodes | 21 | 19724 |
| Extracted data and BSS layout | 25 | 0 |

`complete` marks a C unit without included assembly or raw opcode arrays
(inline instructions, header intrinsics, register pins and barriers are
permitted). A mixed C/ASM unit counts entirely under retained assembly, so
these are unit counts, not an instruction-level C percentage. See
[reporting policy](docs/ASM_AND_GTE_POLICY.md).

The regional targets have complete source coverage: every translation unit is
either unchanged PAL C, a region-specific C file, or reconstructed GTE/COP2,
MDEC, or startup assembly via `HANDWRITTEN_ASM` (not counted as completed C;
handwritten origin is inferred from instruction idioms, not the macro name).
`make port VERSION=<region>` bootstraps a new region from PAL and rejects any
gap or overlap in the text range; selection and coverage totals for an
already-ported region live in `configs/<version>/portable_text.json`.

| Target | Source-covered text | Coverage | Reused PAL C units |
|---|---:|---:|---:|
| USA | 422988 / 422988 bytes | 100% | 328 / 360 |
| Japan v1.0 | 422316 / 422316 bytes | 100% | 314 / 359 |
| Japan v1.1 | 422544 / 422544 bytes | 100% | 319 / 359 |

The pre-release Demo predates most of the above by months and diverges from
PAL far more than any retail region does, so it isn't a PAL port in this same
sense: no `asm`-only segment remains anywhere in `configs/DEMO/main.yaml`, but
most of its source is written directly rather than reused from PAL.

## Layout

- `configs/<version>/` - splat config, symbols, relocs, and checksum per target.
- `src/main/` - decompiled C translation units for the main executable.
- `include/` - project headers and local PSYQ-compatible declarations.
- `tools/scripts/` - project-specific build and analysis helpers.
- `docs/names.md` - the naming evidence: what each function and global is, and
  why. Source files are named after their subject, not after whichever function
  happens to sit first in them.

Generated directories such as `asm/`, `linkers/`, `build/`, `assets/`, and
`disc/` are intentionally ignored, along with local scratch/proposal work.

## Toolchain

Linking and assembling use standard GNU binutils on `PATH`
(`mipsel-none-elf-as`, `-ld`, `-objcopy`). Compiling matching C uses old
PlayStation GCC variants plus [maspsx](https://github.com/mkst/maspsx) to
translate Psy-Q style assembly to GNU assembler input; the wrapper fetches
public helper tooling into `build/toolchain/` when needed, but this repository
publishes neither compiler binaries nor SDK material.

Expects `assets/<version>/main.exe` locally for `PAL`, `USA`, `JAP10`,
`JAP11`, and `DEMO`.

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

Use `VERSION=USA`, `JAP10`, `JAP11`, or `DEMO` for another target; see
`make help` for every target. `tools/scripts/stage_discs.py --version JAP10`
stages one configured local disc dump, or omit `--version` to stage all of
them.

## License

See [LICENSE.md](LICENSE.md). Game code and data remain the property of their
respective owners; this repository contains only clean-room reimplementation
source.
