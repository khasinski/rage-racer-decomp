#!/usr/bin/env python3
"""Build an m2c context file from the project's own headers and aliases.

m2c matches symbols by identifier, but the assembly spells globals by their raw
`D_XXXXXXXX` name while the headers declare them under a readable name bound
with `asm("D_XXXXXXXX")`. m2c knows nothing about that alias, so without this
file it guesses every type. Here each alias is re-emitted under its raw name
with the type the project actually settled on, on top of the real struct
definitions, so m2c produces typed field accesses instead of raw offsets.

Regenerate whenever naming or types move:
    tools/scripts/gen_m2c_context.py > ctx.c
    m2c.py --target mipsel-gcc-c --context ctx.c <function>.s
"""
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]

HEADERS = [
    "common.h",
    "game/car.h", "game/race.h", "game/render.h", "game/state.h", "game/menu.h",
    "game/audio.h", "game/asset.h", "game/track.h", "game/cd.h", "game/memcard.h",
    "psyq/gpu.h", "psyq/gte.h", "psyq/spu.h", "psyq/snd.h", "psyq/cd.h",
    "psyq/kernel.h",
]

BUILTIN = {
    "char", "short", "int", "long", "float", "double", "void",
    "u_char", "u_short", "u_long", "u_int",
}

ALIAS_RE = re.compile(
    r'^\s*(?:extern\s+)?([A-Za-z_][\w \t\*]*?)\s*([A-Za-z_]\w*)\s*'
    r'((?:\[[^\]]*\])*)\s*asm\("(D_[0-9A-F]{8})"\)\s*;',
    re.M,
)


def preprocess(source: str) -> str:
    return subprocess.run(
        ["mipsel-none-elf-cpp", "-I", str(ROOT / "include"), "-I", str(ROOT / "src/main"),
         "-undef", "-Wall", "-fno-builtin", "-P", "-"],
        input=source, capture_output=True, text=True, check=True,
    ).stdout


def known_types(preprocessed: str) -> set:
    """Type names the preprocessed headers actually define."""
    typedef_lines = "\n".join(
        l for l in preprocessed.splitlines() if l.strip().startswith("typedef")
    )
    names = set(re.findall(r"\b(\w+)\s*;", typedef_lines))
    names |= set(re.findall(r"\}\s*(\w+)\s*;", preprocessed))
    return names | BUILTIN


def main() -> int:
    header_src = "".join(f'#include "{h}"\n' for h in HEADERS)
    preprocessed = preprocess(header_src)
    known = known_types(preprocessed)

    aliases = {}
    for path in list((ROOT / "src").rglob("*.c")) + list((ROOT / "include").rglob("*.h")):
        for m in ALIAS_RE.finditer(path.read_text(errors="ignore")):
            aliases.setdefault(m.group(4), (m.group(1).strip(), m.group(3)))

    kept, dropped = [], []
    for raw, (ctype, dims) in sorted(aliases.items()):
        # a type whose definition is not in the headers would be a parse error for
        # m2c, and one bad line costs the whole context
        base = [w for w in ctype.replace("*", " ").split()
                if w not in ("extern", "volatile", "const", "unsigned", "signed")]
        (kept if all(b in known for b in base) else dropped).append(
            (f"extern {ctype} {raw}{dims};", ctype)
        )

    print(preprocessed)
    print("/* globals under their raw names, so m2c can type what the assembly spells */")
    for line, _ in kept:
        print(line)
    print(f"/* {len(kept)} typed, {len(dropped)} skipped for locally-defined types */",
          file=sys.stderr)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
