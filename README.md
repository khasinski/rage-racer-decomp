# Rage Racer Decompilation

![functions](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fkhasinski%2Frage-racer-decomp%2Fmain%2Fdocs%2Fbadges%2Ffunctions.json) ![code](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fkhasinski%2Frage-racer-decomp%2Fmain%2Fdocs%2Fbadges%2Fcode.json)

A complete, byte-exact decompilation of the PAL PlayStation release of
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

**The decompilation is complete.** Every compiler-generated game function in
the PAL executable is plain C, and the built executable is byte-identical to
retail.

| Binary | Functions | Code bytes |
|---|---:|---:|
| `SCES_006.50 (main)` | 648 / 648 (100%) | 313380 / 313380 (100%) |

A function counts as decompiled when it carries no `INCLUDE_ASM` or
`INCLUDE_RODATA` and no non-empty inline assembly. Three things are sanctioned
and do not lower the count: GTE/COP2 operations expressed through
`psyq/gte_macros.h`, which are the hardware interface and cannot be written in
C; register and symbol `asm` labels; and empty barriers used to hold statement
order. Functions are counted individually rather than per file, so one function
needing a crutch cannot reclassify the plain C beside it.

A further 26 functions (10280 code bytes) are documented handwritten assembly
in the original game, marked `HANDWRITTEN_ASM`, and are excluded from the totals
above rather than counted as failures.

Sony's 473 PsyQ library functions (100048 code bytes) are also excluded from
the game-code denominator; they are nevertheless matched because the complete
executable must relink byte-for-byte.

Regenerate the table and the badge JSON with `make progress`.

The regional targets also have complete source coverage. Unchanged translation
units reuse PAL C; actual regional differences live in separate USA/Japanese C
files. Original hand-written GTE/COP2, MDEC and startup routines remain clearly
marked `HANDWRITTEN_ASM` sources rather than being misrepresented as generated
C.

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
