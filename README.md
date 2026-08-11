# Rage Racer Decompilation

![functions](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fkhasinski%2Frage-racer-decomp%2Fmain%2Fdocs%2Fbadges%2Ffunctions.json) ![code](https://img.shields.io/endpoint?url=https%3A%2F%2Fraw.githubusercontent.com%2Fkhasinski%2Frage-racer-decomp%2Fmain%2Fdocs%2Fbadges%2Fcode.json)

A complete, byte-exact decompilation of the PAL PlayStation release of
Rage Racer, target `SCES-006.50`.

This repository is source-only. It does not contain disc images, extracted game
assets, CDDA/XA data, Sony PSYQ SDK files, compiler binaries, generated split
output, local post-build rewrite passes, or decompilation scratch work.
Contributors must provide their own legally obtained copy of the game and local
toolchains.

## Target

| Field | Value |
|---|---|
| Game | Rage Racer, PAL / Europe |
| Executable | `SCES_006.50` |
| Target path | `assets/PAL/main.exe` |
| SHA-1 | `2913e15648eddef40821c5f666460abc04155ee6` |
| Entry PC | `0x800630B4` |
| Text VRAM | `0x80010000..0x8009B000` |

The USA executable `SLUS_004.03` is kept as a comparison target with SHA-1
`2661e8bf18d209c98fd70d33e18ab88b10abd52b`.

## Progress

**The decompilation is complete.** Every function in the PAL executable is
plain C, and the built executable is byte-identical to retail.

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

The table counts the game. Sony's PsyQ libraries - `libgpu`, `libgte`, `libspu`
and the rest, another 472 functions and 100048 code bytes - had to be matched to
relink the executable, but they are not this game, and counting them would
credit the project with someone else's work. A further 26 functions (10280 code
bytes) are handwritten assembly in the original, marked `HANDWRITTEN_ASM`: a
kernel entry reached by `syscall`, a BIOS call that jumps through a register,
and the boot stub that runs before `$gp` and `$sp` exist. Both groups are
excluded rather than counted as failures.

Regenerate the table and the badge JSON with `make progress`.

Per-object progress is also published to
[decomp.dev](https://decomp.dev/khasinski/rage-racer-decomp), where every unit
is compared against an object disassembled from the retail executable rather
than against this tree - see `tools/scripts/gen_expected.py`. Code and functions
read 100%; data reads 99.88%, the missing 1872 bytes being jump tables that gcc
emits into `.rodata` without a symbol, leaving objdiff nothing to pair them by.

## Layout

- `configs/PAL/` - splat config, symbols, relocs, checksum for `SCES_006.50`.
- `configs/USA/` - comparison target metadata for `SLUS_004.03`.
- `src/main/` - decompiled C translation units for the main executable.
- `include/` - project headers and local PSYQ-compatible declarations.
- `tools/scripts/` - project-specific build and analysis helpers.

Source files are named after their subject, not after whichever function happens
to sit first in them. Generated directories such as `asm/`, `linkers/`, `build/`,
`assets/`, and `disc/` are intentionally ignored, along with local
scratch/proposal work.

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
assets/USA/main.exe        # optional comparison target
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

`make split` regenerates `asm/` and `linkers/` from the user-supplied
executable, `make build` compiles and links `build/PAL/main.exe`, and
`make check` verifies its SHA-1 against retail. `make progress` refreshes the
badge JSON and prints the table shown above.

## License

See [LICENSE.md](LICENSE.md). Game code and data remain the property of their
respective owners; this repository contains only clean-room reimplementation
source.
