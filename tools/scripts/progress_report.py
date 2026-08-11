#!/usr/bin/env python3
from __future__ import annotations

import json
import re
from datetime import date
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
VERSION = "PAL"
CONFIG = ROOT / "configs" / VERSION / "main.yaml"
SRC_ROOT = ROOT / "src" / "main"
DOCS = ROOT / "docs"
BADGES = DOCS / "badges"


INCLUDE_MARKERS = re.compile(r"INCLUDE_ASM|INCLUDE_RODATA")
# Assembler directives count only where they can actually occur in C: inside a
# string literal handed to asm(). Matched against bare source they also hit
# ordinary struct field accesses -- `foo.word` on a union with a `word` member
# reads as a `.word` directive and silently reclassifies plain C as assembly.
ASM_DIRECTIVE = re.compile(r"\.(?:word|globl|ent|include)\b")
STRING_LITERAL = re.compile(r'"(?:[^"\\\n]|\\.)*"')
ASM_RE = re.compile(r"(^|[^_a-zA-Z0-9])(__asm__|asm)\b")


def has_asm_markers(text: str) -> bool:
    if INCLUDE_MARKERS.search(text):
        return True
    return any(ASM_DIRECTIVE.search(lit.group(0)) for lit in STRING_LITERAL.finditer(text))
SUBSEGMENT = re.compile(
    r"\[0x([0-9A-Fa-f]+),\s*([^,\]\s]+),\s*([^,\]\s]+)"
)


# Sony shipped libgpu, libgte, libspu and the rest with the SDK. Matching them
# was necessary to relink the executable, but they are not this game and
# counting them would inflate every number here with someone else's work. The
# same split feeds decomp.dev; see tools/scripts/gen_objdiff_config.py.
SDK_UNIT = re.compile(r"^[^/]+/lib/")


def is_sdk(name: str) -> bool:
    """True for a subsegment that belongs to the PsyQ libraries, not the game."""
    return bool(SDK_UNIT.match(name))


def color(percent: float) -> str:
    if percent >= 90:
        return "brightgreen"
    if percent >= 60:
        return "green"
    if percent >= 30:
        return "yellow"
    if percent >= 10:
        return "orange"
    return "red"


def badge(path: Path, label: str, message: str, percent: float) -> None:
    path.write_text(
        json.dumps(
            {
                "schemaVersion": 1,
                "label": label,
                "message": message,
                "color": color(percent),
            },
            indent=2,
        )
        + "\n"
    )


def _blank(match: re.Match) -> str:
    """Replace a comment with spaces so offsets stay aligned with the raw text."""
    return re.sub(r"[^\n]", " ", match.group(0))


def strip_comments(text: str) -> str:
    # Length-preserving: a HANDWRITTEN_ASM note is located by its offset in the
    # raw text, and a unit that holds several functions only classifies them
    # correctly when the stripped offsets still line up.
    return re.sub(r"//[^\n]*", _blank, re.sub(r"/\*.*?\*/", _blank, text, flags=re.S))


# Sanctioned GTE/COP2 operations (see README.md). Expressed
# through psyq/gte_macros.h or as a single documented cop2 op, these represent
# the hardware interface and do not lower progress. Ordinary MIPS in inline asm
# still counts against plain C.
GTE_ASM_TEMPLATE = re.compile(
    r"(?:\.word\s+0x4[89ABab]|"
    r"\b(?:swc2|lwc2|mfc2|mtc2|cfc2|ctc2|cop2|rtps|rtpt|rtir|nclip|avsz3|avsz4|"
    r"mvmva|ncds|ncdt|ncs|nct|nccs|ncct|cdp|cc|dpcs|dpct|intpl|dcpl|gpf|gpl|"
    r"sqr|op|nclip)\b)",
    re.IGNORECASE,
)

ASM_STMT = re.compile(
    r"(?:__asm__|asm)\s*(?:__volatile__|volatile)?\s*"
    r"\(\s*\"(?P<tpl>(?:[^\"\\]|\\.)*)\"(?:\s*:[^)]*)?\)\s*;?",
    re.S,
)


def strip_nonblocking_asm(text: str) -> str:
    # Strip C comments first: a directive-like token (asm, .word, .globl, ...)
    # appearing in prose is documentation, not code, and must not lower progress.
    text = re.sub(r"/\*.*?\*/", "", text, flags=re.S)
    text = re.sub(r"//[^\n]*", "", text)

    decl_label = r"(?m)(^.*\S\s+)\b(?:__asm__|asm)\s*\(\s*\"(?:[^\"\\]|\\.)*\"\s*\)"
    text = re.sub(decl_label, r"\1", text)

    empty = r"\b(?:__asm__|asm)\s*(?:__volatile__|volatile)?\s*\(\s*\"\"\s*(?::.*?)?\)\s*;"
    text = re.sub(empty, "", text, flags=re.S)

    # Sanctioned GTE/COP2 ops do not lower progress.
    def _drop_gte(m: "re.Match") -> str:
        return "" if GTE_ASM_TEMPLATE.search(m.group("tpl")) else m.group(0)

    return ASM_STMT.sub(_drop_gte, text)


def is_plain_c(path: Path) -> bool:
    if not path.exists():
        return False
    text = strip_nonblocking_asm(strip_comments(path.read_text(errors="ignore")))
    return not has_asm_markers(text) and ASM_RE.search(text) is None


# Proven handwritten assembly (see README.md) is excluded from
# the progress denominator entirely: it is neither pending C work nor a C match.
HANDWRITTEN_RE = re.compile(r"\bHANDWRITTEN_ASM\b")
HANDWRITTEN_SYMBOL_RE = re.compile(r"^\s*\*?\s*Symbol:\s*(\w+)", re.M)
ASM_ALIAS_RE = re.compile(r"\b(\w+)\s*\([^;{]*?\)\s*asm\(\"(\w+)\"\)")


def is_handwritten(path: Path) -> bool:
    return path.exists() and HANDWRITTEN_RE.search(path.read_text(errors="ignore")) is not None


# A subsegment can cover several functions (see "Translation units" in
# docs/names.md), so counting subsegments would under-report the binary as
# functions are grouped. Count what a unit actually accounts for instead: one
# per INCLUDE_ASM stub plus one per function definition. `static inline`
# helpers are excluded because they are inlined rather than emitted separately.
# Note the symbol table cannot be used for this: the assembly marks some
# mid-function labels `.globl`, which inflates the count.
FUNC_DEF = re.compile(
    r"^[A-Za-z_][\w \t\*]*?\b([A-Za-z_]\w*)\s*\([^;{]*\)\s*\{",
    re.M,
)
INCLUDE_ASM_RE = re.compile(r"\bINCLUDE_ASM\s*\(")
INCLUDE_ASM_NAME_RE = re.compile(r"\bINCLUDE_ASM\s*\([^,]*,\s*(\w+)\s*\)")


def count_functions(path: Path) -> int:
    if not path.exists():
        return 1
    return max(1, sum(classify_functions(path).values()))


def _function_bodies(text: str):
    """Yield (name, body, offset) for each definition, by brace matching."""
    for match in FUNC_DEF.finditer(text):
        if "static" in match.group(0) and "inline" in match.group(0):
            continue
        depth, index = 0, text.index("{", match.start())
        start = index
        while index < len(text):
            if text[index] == "{":
                depth += 1
            elif text[index] == "}":
                depth -= 1
                if depth == 0:
                    break
            index += 1
        yield match.group(1), text[start : index + 1], match.start()


SYMBOL_ADDRESS_RE = re.compile(r"^(?:func|D)_(8[0-9A-Fa-f]{7})$")


def _header_aliases(cache={}) -> dict:
    """C function name -> emitted symbol, from the `asm("...")` aliases in
    include/. A unit that defines a named function gets its address this way."""
    if not cache:
        for header in (ROOT / "include").rglob("*.h"):
            text = strip_comments(header.read_text(errors="ignore"))
            for name, symbol in ASM_ALIAS_RE.findall(text):
                cache.setdefault(name, symbol)
    return cache


def _address_of(name: str, aliases: dict) -> int | None:
    for candidate in (name, aliases.get(name), _header_aliases().get(name)):
        match = SYMBOL_ADDRESS_RE.match(candidate or "")
        if match:
            return int(match.group(1), 16)
    return None


def classify_functions(path: Path) -> dict:
    """Count a unit's functions by kind. See classify_with_addresses."""
    counts = {"plain": 0, "asm": 0, "handwritten": 0}
    for kind, _address in classify_with_addresses(path):
        counts[kind] += 1
    return counts


def classify_with_addresses(path: Path) -> list:
    """Classify a unit's functions, each with its vram address when known.

    Attribution is per function, not per file: a unit may hold both plain C and
    a function that needs an assembly crutch, and grouping the two must not
    reclassify the clean ones. Kinds are "plain", "asm" and "handwritten".
    """
    raw = path.read_text(errors="ignore")
    text = strip_comments(raw)
    found = []

    # A HANDWRITTEN_ASM note either names its subject on a `Symbol:` line - in
    # which case it documents that symbol wherever in the unit it sits - or it
    # sits in the comment block right above what it documents.
    handwritten_before = [m.start() for m in HANDWRITTEN_RE.finditer(raw)]
    handwritten_symbols = set()
    for start in handwritten_before:
        symbol = HANDWRITTEN_SYMBOL_RE.search(raw, start, start + 1200)
        if symbol:
            handwritten_symbols.add(symbol.group(1))

    # A unit-local `asm("func_XXXXXXXX")` alias lets a note name the emitted
    # symbol while the definition carries the readable name.
    aliases = dict(ASM_ALIAS_RE.findall(text))

    def documented(position: int, name: str | None = None) -> bool:
        if name and (
            name in handwritten_symbols or aliases.get(name) in handwritten_symbols
        ):
            return True
        return position >= 0 and any(
            0 <= position - start < 4000 for start in handwritten_before
        )

    # Each INCLUDE_ASM stands in for one function that is still assembly, unless
    # it is documented handwritten assembly, which is excluded entirely.
    for match in INCLUDE_ASM_RE.finditer(text):
        stub = INCLUDE_ASM_NAME_RE.match(text, match.start())
        stub_name = stub.group(1) if stub else None
        kind = "handwritten" if documented(match.start(), stub_name) else "asm"
        found.append((kind, _address_of(stub_name or "", aliases)))

    for name, body, offset in _function_bodies(text):
        marked = documented(offset, name)
        stripped = strip_nonblocking_asm(body)
        blocking = has_asm_markers(stripped) or ASM_RE.search(stripped)
        if marked and blocking:
            kind = "handwritten"
        elif blocking:
            kind = "asm"
        else:
            kind = "plain"
        found.append((kind, _address_of(name, aliases)))
    return found


# The PS-X EXE maps file offset 0x800 to vram 0x80010000.
BASE_VRAM = 0x80010000
BASE_OFF = 0x800


def _function_extents(classified: list, start: int, end: int):
    """Split a subsegment's bytes across its functions, or None if it cannot be
    done exactly (an unresolved address, or one outside the subsegment)."""
    low = BASE_VRAM + (start - BASE_OFF)
    high = BASE_VRAM + (end - BASE_OFF)
    entries = []
    for kind, address in classified:
        if address is None or not low <= address < high:
            return None
        entries.append((address, kind))
    entries.sort()
    if len(dict(entries)) != len(entries):
        return None
    # Anything ahead of the first recognised function (alignment, or a leading
    # routine the source scan could not place) goes to that first function.
    bounds = [low] + [address for address, _kind in entries[1:]] + [high]
    return [
        (kind, bounds[index + 1] - bounds[index])
        for index, (_address, kind) in enumerate(entries)
    ]


def main() -> int:
    subsegments = []
    for line in CONFIG.read_text().splitlines():
        match = SUBSEGMENT.search(line)
        if match:
            subsegments.append((int(match.group(1), 16), match.group(2), match.group(3)))

    rows = []
    total_funcs = 0
    plain_funcs = 0
    total_bytes = 0
    plain_bytes = 0
    handwritten_funcs = 0
    handwritten_bytes = 0
    sdk_funcs = 0
    sdk_bytes = 0

    for index, (start, kind, name) in enumerate(subsegments):
        if kind != "c":
            continue
        end = None
        for next_start, _next_kind, _next_name in subsegments[index + 1 :]:
            end = next_start
            break
        if end is None:
            continue

        size = max(0, end - start)
        src = SRC_ROOT / f"{name}.c"

        if is_sdk(name):
            sdk_bytes += size
            sdk_funcs += count_functions(src) if src.exists() else 1
            continue

        if not src.exists():
            total_funcs += 1
            total_bytes += size
            continue

        classified = classify_with_addresses(src)
        kinds = {"plain": 0, "asm": 0, "handwritten": 0}
        for unit_kind, _address in classified:
            kinds[unit_kind] += 1
        counted = kinds["plain"] + kinds["asm"]

        handwritten_funcs += kinds["handwritten"]
        if counted == 0:
            # The whole unit is documented handwritten assembly.
            handwritten_bytes += size
            continue

        total_funcs += counted
        plain_funcs += kinds["plain"]

        # Bytes are attributed per function so that grouping functions into a
        # translation unit cannot change the total: each function owns the
        # bytes from its address up to the next one in the unit. A unit whose
        # addresses cannot all be resolved falls back to whole-subsegment
        # attribution, where a mixed unit counts as plain only when every
        # function in it is plain.
        extents = _function_extents(classified, start, end)
        if extents is None:
            total_bytes += size
            if kinds["asm"] == 0:
                plain_bytes += size
            continue

        for unit_kind, length in extents:
            if unit_kind == "handwritten":
                handwritten_bytes += length
                continue
            total_bytes += length
            if unit_kind == "plain":
                plain_bytes += length

    func_pct = plain_funcs / total_funcs * 100 if total_funcs else 0.0
    byte_pct = plain_bytes / total_bytes * 100 if total_bytes else 0.0
    rows.append(
        (
            "`SCES_006.50 (main)`",
            f"{plain_funcs}/{total_funcs}",
            f"{func_pct:.1f}%",
            f"{plain_bytes}/{total_bytes}",
            f"{byte_pct:.1f}%",
        )
    )

    DOCS.mkdir(exist_ok=True)
    BADGES.mkdir(exist_ok=True)

    badge(
        BADGES / "functions.json",
        "functions",
        f"{plain_funcs}/{total_funcs} {func_pct:.1f}%",
        func_pct,
    )
    badge(
        BADGES / "code.json",
        "code",
        f"{plain_bytes}/{total_bytes} {byte_pct:.1f}%",
        byte_pct,
    )

    lines = [
        "# Decompilation Progress",
        "",
        f"_Generated by `tools/scripts/progress_report.py` on {date.today().isoformat()}. Regenerate with `make progress`._",
        "",
        "A function counts as decompiled when it has no INCLUDE_ASM/INCLUDE_RODATA and no non-empty inline assembly, except sanctioned GTE/COP2 operations expressed through `psyq/gte_macros.h` (or an equivalent single documented cop2 op), which represent the hardware interface and do not lower progress (see `README.md`). Register/symbol asm labels and empty barriers used for matching do not lower progress. Functions are counted and classified INDIVIDUALLY rather than per file, because a translation unit may hold several functions and one function needing an assembly crutch must not reclassify the plain C beside it. Code bytes are attributed the same way: each function owns the bytes from its address to the next function in its unit, so grouping functions into a translation unit cannot move the totals. A unit whose addresses cannot all be resolved falls back to whole-subsegment attribution, where a mixed unit counts as plain only when every function in it is plain. The built executable is byte-identical to retail (`make check VERSION=PAL`).",
        "",
        f"{handwritten_funcs} functions ({handwritten_bytes} code bytes) are documented handwritten assembly (`HANDWRITTEN_ASM`, see `README.md`) and are excluded from the totals below.",
        "",
        f"A further {sdk_funcs} functions ({sdk_bytes} code bytes) are Sony's PsyQ libraries. They had to be matched to relink the executable, but they are not this game, so they are excluded as well.",
        "",
        "| Binary | Functions | % | Code bytes | % |",
        "|---|---:|---:|---:|---:|",
    ]
    lines.extend(f"| {binary} | {funcs} | {fpct} | {code} | {bpct} |" for binary, funcs, fpct, code, bpct in rows)
    # The decompilation is complete, so the table lives in README.md and does
    # not move any more. Only the badge JSON is regenerated here.
    print("\n".join(lines))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
