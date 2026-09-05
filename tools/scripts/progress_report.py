#!/usr/bin/env python3
"""Print the progress table and refresh the badge JSON from the objdiff report.

Progress is whether the code this tree produces is the code the game shipped,
so the numbers come from the same classified report decomp.dev ingests: every
object is compared, function by function, against an object disassembled from
the retail executable. Audited embedded data is moved out of the function
ledger, with raw objdiff output retained separately. `make report` writes it.

Matching and completion are different measures. Source-form categories show
which units retain assembly, and completion allows inline instructions and
compiler constraints but excludes included ASM and raw opcode arrays.
"""

from __future__ import annotations

import argparse
import json
import re
import sys
from datetime import date
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
DOCS = ROOT / "docs"
BADGES = DOCS / "badges"

# Sony shipped libgpu, libgte, libspu and the rest with the SDK. Both halves are
# reported - decomp.dev requires every function in the binary to be accounted
# for - but they are counted separately, because matching someone else's
# library is not the same work as decompiling the game.
CATEGORY_NAMES = {"game": "Game code", "psyq": "PsyQ libraries"}

ASM_INCLUDE = re.compile(r"\bHANDWRITTEN_ASM\s*\(")
# Assembler directives count only where they can actually occur in C: inside a
# string literal handed to asm(). Matched against bare source they also hit
# ordinary struct field accesses -- `foo.word` on a union with a `word` member
# reads as a `.word` directive and silently reclassifies plain C as assembly.
ASM_DIRECTIVE = re.compile(r"\.(?:word|globl|ent|include)\b")
STRING_LITERAL = re.compile(r'"(?:[^"\\\n]|\\.)*"')
ASM_STATEMENT = re.compile(r"(^|[^_a-zA-Z0-9])(__asm__|asm)\s*(volatile\s*)?\(")


def color(percent: float) -> str:
    if percent >= 100:
        return "brightgreen"
    if percent >= 90:
        return "green"
    if percent >= 60:
        return "yellow"
    if percent >= 30:
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


def percent(measures: dict, key: str) -> float:
    return float(measures.get(key, 0.0) or 0.0)


def count(measures: dict, key: str) -> int:
    return int(measures.get(key, 0) or 0)


def carries_assembly(text: str) -> bool:
    """True when a translation unit still holds assembly of any kind."""
    if ASM_INCLUDE.search(text):
        return True
    if any(ASM_DIRECTIVE.search(lit.group(0)) for lit in STRING_LITERAL.finditer(text)):
        return True
    return bool(ASM_STATEMENT.search(text))


def source_mix(src_root: Path) -> tuple[int, int]:
    """How many translation units are plain C, out of how many altogether.

    Deliberately per file rather than per function: a unit that mixes C with a
    hand-written block is not plain C, and pretending otherwise is how the old
    count drifted away from what it claimed to measure.
    """
    total = plain = 0
    for path in sorted(src_root.rglob("*.c")):
        total += 1
        if not carries_assembly(path.read_text(errors="ignore")):
            plain += 1
    return plain, total


def table(report: dict) -> list[str]:
    overall = report["measures"]
    rows = [
        "| Scope | Functions | Code bytes | Data bytes |",
        "|---|---:|---:|---:|",
    ]
    for category in report.get("categories", []):
        if category["id"] not in CATEGORY_NAMES:
            continue
        measures = category["measures"]
        rows.append(
            "| %s | %d / %d (%.2f%%) | %.2f%% | %.2f%% |"
            % (
                CATEGORY_NAMES.get(category["id"], category["name"]),
                count(measures, "matched_functions"),
                count(measures, "total_functions"),
                percent(measures, "matched_functions_percent"),
                percent(measures, "matched_code_percent"),
                percent(measures, "matched_data_percent"),
            )
        )
    rows.append(
        "| **Whole executable** | **%d / %d (%.2f%%)** | **%.2f%%** | **%.2f%%** |"
        % (
            count(overall, "matched_functions"),
            count(overall, "total_functions"),
            percent(overall, "matched_functions_percent"),
            percent(overall, "matched_code_percent"),
            percent(overall, "matched_data_percent"),
        )
    )
    return rows


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--version", default="PAL")
    parser.add_argument("--report", default=None)
    args = parser.parse_args(argv)

    report_path = Path(args.report).resolve() if args.report else (
        ROOT / "build" / args.version / "report.json")
    if not report_path.exists():
        raise SystemExit("%s missing - run `make report` first" % report_path)
    report = json.loads(report_path.read_text())

    overall = report["measures"]
    functions_pct = percent(overall, "matched_functions_percent")
    code_pct = percent(overall, "matched_code_percent")

    BADGES.mkdir(parents=True, exist_ok=True)
    badge(
        BADGES / "functions.json",
        "functions matched",
        "%d/%d %.2f%%" % (count(overall, "matched_functions"),
                          count(overall, "total_functions"), functions_pct),
        functions_pct,
    )
    badge(
        BADGES / "code.json",
        "code matched",
        "%.2f%%" % code_pct,
        code_pct,
    )

    lines = [
        "# Decompilation Progress",
        "",
        "_Generated by `tools/scripts/progress_report.py` from "
        "`%s` on %s. Regenerate with `make report progress`._"
        % (report_path.relative_to(ROOT), date.today().isoformat()),
        "",
        "A function counts as matched when the object this tree compiles and an "
        "object disassembled from the retail executable contain the same code, "
        "as judged by objdiff. The linked executable is byte-identical to "
        "retail (`make check`).",
        "",
    ]
    lines.extend(table(report))
    lines += ["", "Source form (a separate partition of the same units):", "",
              "| Source form | Units | Code bytes in these units |",
              "|---|---:|---:|"]
    for category in report.get("categories", []):
        if category["id"].startswith("source_"):
            measures = category["measures"]
            lines.append("| %s | %d | %d |" % (category["name"],
                         count(measures, "total_units"), count(measures, "total_code")))
    lines += ["", "Completion marks C units without included assembly or raw opcode arrays; inline "
              "instructions, header intrinsics and compiler constraints are allowed. "
              "Mixed C/ASM units stay in the retained-assembly category in full. "
              "These unit-level totals do not measure how many instructions are C. "
              "Extracted data and BSS are accounted for but do not count as C "
              "decompilation. BSS matching measures layout only."]
    print("\n".join(lines))
    # Completion is a maintained invariant now, not merely a badge claim.
    # Data can contain unnamed padding/jump-table words that objdiff does not
    # pair, so gate on the executable code and its discovered functions.
    return 0 if (
        count(overall, "matched_functions") == count(overall, "total_functions")
        and count(overall, "matched_code") == count(overall, "total_code")
    ) else 1


if __name__ == "__main__":
    sys.exit(main())
