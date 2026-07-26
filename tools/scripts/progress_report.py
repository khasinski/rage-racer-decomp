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


ASM_MARKERS = re.compile(
    r"INCLUDE_ASM|INCLUDE_RODATA|\.word\b|\.globl\b|\.ent\b|\.include\b"
)
ASM_RE = re.compile(r"(^|[^_a-zA-Z0-9])(__asm__|asm)\b")
SUBSEGMENT = re.compile(
    r"\[0x([0-9A-Fa-f]+),\s*([^,\]\s]+),\s*([^,\]\s]+)"
)


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


def strip_comments(text: str) -> str:
    return re.sub(r"//.*", "", re.sub(r"/\*.*?\*/", "", text, flags=re.S))


# Sanctioned GTE/COP2 operations (see docs/ASM_AND_GTE_POLICY.md). Expressed
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
    return ASM_MARKERS.search(text) is None and ASM_RE.search(text) is None


# Proven handwritten assembly (see docs/ASM_AND_GTE_POLICY.md) is excluded from
# the progress denominator entirely: it is neither pending C work nor a C match.
HANDWRITTEN_RE = re.compile(r"\bHANDWRITTEN_ASM\b")


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


def count_functions(path: Path) -> int:
    if not path.exists():
        return 1
    return max(1, sum(classify_functions(path).values()))


def _function_bodies(text: str):
    """Yield (name, body) for each function definition, by brace matching."""
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
        yield match.group(1), text[start : index + 1]


def classify_functions(path: Path) -> dict:
    """Count a unit's functions by kind.

    Attribution is per function, not per file: a unit may hold both plain C and
    a function that needs an assembly crutch, and grouping the two must not
    reclassify the clean ones. Keys are "plain", "asm" and "handwritten".
    """
    raw = path.read_text(errors="ignore")
    text = strip_comments(raw)
    counts = {"plain": 0, "asm": 0, "handwritten": 0}

    # A HANDWRITTEN_ASM note sits in the comment block above whatever it
    # documents, so attribute each marker to the following stub or definition.
    handwritten_before = [m.start() for m in HANDWRITTEN_RE.finditer(raw)]

    def documented(position: int) -> bool:
        return position >= 0 and any(
            0 <= position - start < 4000 for start in handwritten_before
        )

    # Each INCLUDE_ASM stands in for one function that is still assembly, unless
    # it is documented handwritten assembly, which is excluded entirely.
    for match in INCLUDE_ASM_RE.finditer(text):
        marker = raw.find(text[match.start() : match.start() + 40])
        if documented(marker if marker >= 0 else match.start()):
            counts["handwritten"] += 1
        else:
            counts["asm"] += 1

    for name, body in _function_bodies(text):
        marked = documented(raw.find(name))
        stripped = strip_nonblocking_asm(body)
        blocking = ASM_MARKERS.search(stripped) or ASM_RE.search(stripped)
        if marked and blocking:
            counts["handwritten"] += 1
        elif blocking:
            counts["asm"] += 1
        else:
            counts["plain"] += 1
    return counts


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

        if not src.exists():
            total_funcs += 1
            total_bytes += size
            continue

        kinds = classify_functions(src)
        counted = kinds["plain"] + kinds["asm"]

        handwritten_funcs += kinds["handwritten"]
        if counted == 0:
            # The whole unit is documented handwritten assembly.
            handwritten_bytes += size
            continue

        # Bytes are still attributed per subsegment, so a unit that mixes plain
        # C with a crutch counts its bytes as plain only when every function in
        # it is plain.
        total_funcs += counted
        total_bytes += size
        plain_funcs += kinds["plain"]
        if kinds["asm"] == 0:
            plain_bytes += size

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
        "A function counts as decompiled when it has no INCLUDE_ASM/INCLUDE_RODATA and no non-empty inline assembly, except sanctioned GTE/COP2 operations expressed through `psyq/gte_macros.h` (or an equivalent single documented cop2 op), which represent the hardware interface and do not lower progress (see `docs/ASM_AND_GTE_POLICY.md`). Register/symbol asm labels and empty barriers used for matching do not lower progress. Functions are counted and classified INDIVIDUALLY rather than per file, because a translation unit may hold several functions and one function needing an assembly crutch must not reclassify the plain C beside it. Code bytes are still attributed per subsegment, so a mixed unit counts its bytes as plain only when every function in it is plain. The built executable is byte-identical to retail (`make check VERSION=PAL`).",
        "",
        f"{handwritten_funcs} functions ({handwritten_bytes} code bytes) are documented handwritten assembly (`HANDWRITTEN_ASM`, see `docs/ASM_AND_GTE_POLICY.md`) and are excluded from the totals below.",
        "",
        "| Binary | Functions | % | Code bytes | % |",
        "|---|---:|---:|---:|---:|",
    ]
    lines.extend(f"| {binary} | {funcs} | {fpct} | {code} | {bpct} |" for binary, funcs, fpct, code, bpct in rows)
    (DOCS / "PROGRESS.md").write_text("\n".join(lines) + "\n")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
