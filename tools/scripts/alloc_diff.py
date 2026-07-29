#!/usr/bin/env python3
"""Explain what a source edit did to GCC 2.6.3 register allocation.

    tools/scripts/alloc_diff.py func_8003F9C4 before.c after.c

This deliberately compares two *candidate* compilations.  It cannot recover a
retail pseudo, priority, allocation order, or "first inversion" from final
assembly; TASK_FOR_CODEX_2I proved that those compiler-time facts are not
observable there.  Pretending otherwise turns a useful diagnostic into a source
of invented targets.

For each candidate this runs the project compiler with local/global allocation
dumps, reconstructs real allocnos (including the rare reg_may_share grouping),
recomputes GCC's priority, verifies the result against the order printed by
global_alloc, and reports the final coloring and linked retail `exact` metric.
The diff then answers the actionable question: which allocno did this edit move,
what refs/live-length/priority change caused it, and what hard-register or stack
change followed?
"""
from __future__ import annotations

import argparse
import os
import platform
import re
import shutil
import subprocess
import sys
import tempfile
from collections import defaultdict
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
TOOLCHAIN = ROOT / "build" / "toolchain"
BIN_DIR = TOOLCHAIN / "bin"
MASPSX = TOOLCHAIN / "maspsx" / "maspsx.py"
MEASURE = ROOT / "tools" / "scripts" / "measure_candidate.py"

HARD_REG_NAMES = (
    ["zero", "at", "v0", "v1", "a0", "a1", "a2", "a3"]
    + [f"t{i}" for i in range(8)]
    + [f"s{i}" for i in range(8)]
    + ["t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"]
    + [f"f{i}" for i in range(32)]
)

MODE_BYTES = {
    "QI": 1,
    "HI": 2,
    "SI": 4,
    "SF": 4,
    "DI": 8,
    "DF": 8,
    "TI": 16,
    "XF": 12,
}

REG_RTX = r"\(reg(?:/[a-z]+)*:[A-Z0-9_]+\s+(\d+)(?:\s+[^)]*)?\)"
DIRECT_COPY_RE = re.compile(
    rf"\(set\s+{REG_RTX}\s+{REG_RTX}\s*\)",
    re.MULTILINE,
)
STATS_RE = re.compile(
    r"^Register (\d+) used (\d+) times across (-?\d+) insns(?P<tail>[^\n]*);?$",
    re.MULTILINE,
)
MODE_RE = re.compile(r"\(reg(?:/[a-z]+)*:([A-Z0-9_]+)\s+(\d+)")
ORDER_RE = re.compile(r"^;; \d+ regs to allocate:(?P<order>[ 0-9]+)$", re.MULTILINE)
DISPOSITIONS_RE = re.compile(
    r"^;; Register dispositions:\n(?P<body>.*?)(?:\n\n|\Z)", re.MULTILINE | re.DOTALL
)
DEF_RE = re.compile(
    r"\.def\s+([^;]+);.*?\.val\s+(-?\d+);.*?\.scl\s+(\d+);.*?\.endef"
)


class AllocDiffError(RuntimeError):
    """A dump was incomplete or contradicted GCC's printed allocation order."""


@dataclass(frozen=True)
class PseudoStats:
    refs: int
    live: int
    global_p: bool
    mode: str | None
    bytes_hint: int


@dataclass(frozen=True)
class Allocno:
    number: int
    representative: int
    members: tuple[int, ...]
    refs: int
    live: int
    size: int
    priority: int
    color: int | None


@dataclass
class Candidate:
    label: str
    source: Path
    exact: int
    frame: str
    order: list[int]
    allocnos: dict[int, Allocno]
    dispositions: dict[int, int]
    locals: dict[str, str]


def hard_reg(regno: int | None) -> str:
    if regno is None:
        return "stack"
    if 0 <= regno < len(HARD_REG_NAMES):
        return f"${HARD_REG_NAMES[regno]}"
    return f"$r{regno}"


def cc1_command(tmp: Path) -> list[str]:
    version = os.environ.get("RAGE_CC1_VERSION", "2.6.3")
    if version != "2.6.3":
        raise AllocDiffError(f"unsupported RAGE_CC1_VERSION={version}")

    cc1 = BIN_DIR / "cc1-psx-263"
    if (
        platform.system() == "Darwin"
        and os.environ.get("RAGE_CC1_DARWIN", "1") != "0"
        and Path(f"{cc1}-darwin").is_file()
    ):
        cc1 = Path(f"{cc1}-darwin")

    if not cc1.is_file():
        raise AllocDiffError(f"missing compiler: {cc1}")

    try:
        probe = subprocess.run(
            [str(cc1), "-version"],
            stdin=subprocess.DEVNULL,
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL,
            check=False,
        )
    except OSError:
        probe = None
    if probe is not None and probe.returncode == 0:
        return [str(cc1)]
    if shutil.which("qemu-i386"):
        return ["qemu-i386", str(cc1)]
    if shutil.which("qemu-i386-static"):
        return ["qemu-i386-static", str(cc1)]
    if shutil.which("docker"):
        docker_probe = subprocess.run(
            ["docker", "version", "--format", "{{.Server.Version}}"],
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL,
            check=False,
        )
        if docker_probe.returncode == 0:
            cc1_rel = cc1.relative_to(ROOT)
            return [
                "docker",
                "run",
                "--rm",
                "--platform",
                "linux/386",
                "-i",
                "-v",
                f"{ROOT}:/work",
                "-v",
                f"{tmp}:/dump",
                "-w",
                "/dump",
                "i386/alpine:3.20",
                f"/work/{cc1_rel}",
            ]
    raise AllocDiffError(f"{cc1} is not runnable and no qemu-i386 or Docker fallback works")


def compile_with_dumps(source: Path, tmp: Path) -> tuple[Path, Path, Path, Path]:
    raw_asm = tmp / "candidate.s"
    obj = tmp / "candidate.o"
    cpp = os.environ.get("CPP", "mipsel-none-elf-cpp")
    assembler = os.environ.get("AS", "mipsel-none-elf-as")
    python = os.environ.get("PYTHON", sys.executable)

    cpp_cmd = [
        cpp,
        f"-I{ROOT / 'include'}",
        f"-I{ROOT / 'src' / 'main'}",
        "-undef",
        "-Wall",
        "-fno-builtin",
        str(source),
    ]
    cc1_cmd = cc1_command(tmp) + [
        "-quiet",
        "-mcpu=3000",
        "-g",
        "-mgas",
        "-gcoff",
        "-O2",
        "-G0",
        "-funsigned-char",
        "-dl",
        "-dg",
        "-",
        "-o",
        str(raw_asm),
    ]

    cpp_proc = subprocess.Popen(cpp_cmd, cwd=ROOT, stdout=subprocess.PIPE)
    assert cpp_proc.stdout is not None
    cc1_proc = subprocess.run(cc1_cmd, cwd=tmp, stdin=cpp_proc.stdout, check=False)
    cpp_proc.stdout.close()
    cpp_rc = cpp_proc.wait()
    if cpp_rc != 0 or cc1_proc.returncode != 0:
        raise AllocDiffError(
            f"candidate compilation failed (cpp={cpp_rc}, cc1={cc1_proc.returncode}): {source}"
        )

    lreg = tmp / "-.lreg"
    greg = tmp / "-.greg"
    if not raw_asm.is_file() or not lreg.is_file() or not greg.is_file():
        raise AllocDiffError(f"compiler did not produce .s/.lreg/.greg for {source}")

    with raw_asm.open("rb") as raw:
        maspsx_proc = subprocess.Popen(
            [
                python,
                str(MASPSX),
                "--expand-div",
                "--aspsx-version=2.34",
                "--force-stdin",
            ],
            cwd=ROOT,
            stdin=raw,
            stdout=subprocess.PIPE,
        )
        assert maspsx_proc.stdout is not None
        as_proc = subprocess.run(
            [
                assembler,
                "-EL",
                "-G0",
                "-march=r3000",
                "-mtune=r3000",
                "-no-pad-sections",
                "-I",
                str(ROOT / "include"),
                "-o",
                str(obj),
                "-",
            ],
            cwd=ROOT,
            stdin=maspsx_proc.stdout,
            check=False,
        )
        maspsx_proc.stdout.close()
        maspsx_rc = maspsx_proc.wait()
    if maspsx_rc != 0 or as_proc.returncode != 0:
        raise AllocDiffError(
            f"candidate assembly failed (maspsx={maspsx_rc}, as={as_proc.returncode}): {source}"
        )
    return raw_asm, lreg, greg, obj


def parse_dispositions(greg: str) -> dict[int, int]:
    match = DISPOSITIONS_RE.search(greg)
    if not match:
        raise AllocDiffError("missing Register dispositions block in .greg")
    return {int(p): int(h) for p, h in re.findall(r"(\d+)\s+in\s+(\d+)", match["body"])}


def parse_stats(lreg: str) -> dict[int, PseudoStats]:
    modes: dict[int, str] = {}
    for mode, pseudo in MODE_RE.findall(lreg):
        modes.setdefault(int(pseudo), mode)
    stats: dict[int, PseudoStats] = {}
    for match in STATS_RE.finditer(lreg):
        pseudo = int(match.group(1))
        bytes_match = re.search(r";\s*(\d+) bytes;", match["tail"])
        stats[pseudo] = PseudoStats(
            refs=int(match.group(2)),
            live=int(match.group(3)),
            global_p=" in block " not in match["tail"],
            mode=modes.get(pseudo),
            # dump_flow_info omits the byte count for a word-sized pseudo.
            bytes_hint=int(bytes_match.group(1)) if bytes_match else 4,
        )
    if not stats:
        raise AllocDiffError("no register usage rows in .lreg")
    return stats


def shared_groups(
    lreg: str,
    stats: dict[int, PseudoStats],
    order: list[int],
    dispositions: dict[int, int],
) -> dict[int, tuple[int, ...]]:
    representatives = set(order)
    omitted = {p for p, row in stats.items() if row.global_p and p not in representatives}
    groups: dict[int, list[int]] = {p: [p] for p in order}
    if not omitted:
        return {p: (p,) for p in order}

    graph: dict[int, set[int]] = defaultdict(set)
    for match in DIRECT_COPY_RE.finditer(lreg):
        left, right = map(int, match.groups())
        graph[left].add(right)
        graph[right].add(left)

    for member in sorted(omitted):
        same_color = {
            rep
            for rep in representatives
            if member in dispositions
            and rep in dispositions
            and dispositions[member] == dispositions[rep]
        }
        immediate = graph[member] & same_color
        if len(immediate) == 1:
            representative = immediate.pop()
        else:
            seen = {member}
            todo = [member]
            reached: set[int] = set()
            while todo:
                current = todo.pop()
                for neighbor in graph[current] - seen:
                    seen.add(neighbor)
                    if neighbor in same_color:
                        reached.add(neighbor)
                    elif neighbor in omitted:
                        todo.append(neighbor)
            if len(reached) != 1:
                raise AllocDiffError(
                    "cannot reconstruct reg_may_share group for global pseudo "
                    f"{member}; candidate representatives={sorted(reached or same_color)}"
                )
            representative = reached.pop()
        groups[representative].append(member)
    return {rep: tuple(sorted(members)) for rep, members in groups.items()}


def mode_size(mode: str | None, bytes_hint: int, pseudo: int) -> int:
    if mode is not None and mode not in MODE_BYTES:
        raise AllocDiffError(f"cannot determine PSEUDO_REGNO_SIZE for p{pseudo} mode {mode!r}")
    byte_count = MODE_BYTES[mode] if mode is not None else bytes_hint
    return (byte_count + 3) // 4


def source_identifier(source: Path, symbol: str) -> str | None:
    """The C name of the function whose asm alias is `symbol`.

    GCC labels its RTL dumps with the C identifier but emits `.ent` with the asm
    alias, so a function written as `void Foo(void) asm("func_800418D4");` appears
    as `;; Function Foo` in .lreg/.greg but as `.ent func_800418D4` in the .s.
    Looking only for the symbol silently missed every renamed function.

    Resolved by walking back from the alias to the parameter list and taking the
    identifier in front of it. A regex spanning to the alias matches whatever word
    happens to sit in the preceding comment instead.
    """
    try:
        text = source.read_text(errors="ignore")
    except OSError:
        return None
    for alias in re.finditer(r'asm\s*\(\s*"' + re.escape(symbol) + r'"\s*\)', text):
        head = text[: alias.start()].rstrip()
        if not head.endswith(")"):
            continue
        depth = 0
        for i in range(len(head) - 1, -1, -1):
            if head[i] == ")":
                depth += 1
            elif head[i] == "(":
                depth -= 1
                if depth == 0:
                    name = re.search(r"([A-Za-z_]\w*)\s*$", head[:i])
                    return name.group(1) if name else None
                    break
        else:
            continue
    return None


def dump_function(dump: str, func: str, alias: str | None = None) -> str:
    start = re.search(rf"^;; Function {re.escape(func)}\s*$", dump, re.MULTILINE)
    if not start and alias:
        start = re.search(rf"^;; Function {re.escape(alias)}\s*$", dump, re.MULTILINE)
    if not start:
        names = ", ".join(sorted(set(re.findall(r"^;; Function (\S+)$", dump, re.MULTILINE))))
        raise AllocDiffError(
            f"missing Function {func} section in dump; dump contains: {names or 'none'}"
        )
    next_function = re.search(r"^;; Function \S+\s*$", dump[start.end() :], re.MULTILINE)
    end = start.end() + next_function.start() if next_function else len(dump)
    return dump[start.start() : end]


def reconstruct_allocnos(
    lreg: str, greg: str, func: str, alias: str | None = None
) -> tuple[list[int], dict[int, Allocno], dict[int, int]]:
    lreg = dump_function(lreg, func, alias)
    greg = dump_function(greg, func, alias)
    order_match = ORDER_RE.search(greg)
    if not order_match:
        raise AllocDiffError("missing 'regs to allocate' line in .greg")
    printed_order = [int(value) for value in order_match["order"].split()]
    stats = parse_stats(lreg)
    dispositions = parse_dispositions(greg)
    groups = shared_groups(lreg, stats, printed_order, dispositions)

    creation_order = sorted(printed_order, key=lambda rep: min(groups[rep]))
    number = {rep: i for i, rep in enumerate(creation_order)}
    allocnos: dict[int, Allocno] = {}
    for representative in printed_order:
        members = groups[representative]
        try:
            refs = sum(stats[p].refs for p in members)
            # global.c starts the aggregate at zero, so an all--2 group remains
            # zero and is then changed to -1 before allocno_compare.
            live = max([0] + [stats[p].live for p in members])
        except KeyError as exc:
            raise AllocDiffError(f"missing .lreg row for p{exc.args[0]}") from exc
        if live == 0:
            live = -1
        # global.c visits pseudos in regno order and overwrites allocno_size for
        # every reg_may_share member, so the last member supplies the size.
        size_member = members[-1]
        size = mode_size(
            stats[size_member].mode,
            stats[size_member].bytes_hint,
            size_member,
        )
        priority = int(((refs.bit_length() - 1) * refs / live) * 10000 * size)
        allocnos[representative] = Allocno(
            number=number[representative],
            representative=representative,
            members=members,
            refs=refs,
            live=live,
            size=size,
            priority=priority,
            color=dispositions.get(representative),
        )

    computed_order = sorted(
        printed_order,
        key=lambda rep: (-allocnos[rep].priority, allocnos[rep].number),
    )
    if computed_order != printed_order:
        first = next(
            i
            for i, (computed, printed) in enumerate(zip(computed_order, printed_order))
            if computed != printed
        )
        raise AllocDiffError(
            "computed order disagrees with GCC at rank "
            f"{first + 1}: computed p{computed_order[first]}, printed p{printed_order[first]}"
        )
    return printed_order, allocnos, dispositions


def function_asm(asm: str, func: str) -> str:
    start = re.search(rf"^\s*\.ent\s+{re.escape(func)}\s*$", asm, re.MULTILINE)
    if not start:
        return asm
    end = re.search(rf"^\s*\.end\s+{re.escape(func)}\s*$", asm[start.end() :], re.MULTILINE)
    if not end:
        return asm[start.start() :]
    return asm[start.start() : start.end() + end.end()]


def parse_frame_and_locals(asm: str, func: str) -> tuple[str, dict[str, str]]:
    body = function_asm(asm, func)
    frame_match = re.search(r"^\s*\.frame\s+(.+)$", body, re.MULTILINE)
    if not frame_match:
        raise AllocDiffError(f"missing .frame for {func}")
    frame = ".frame " + frame_match.group(1).strip()
    locals_: dict[str, str] = {}
    for name, value_text, scl_text in DEF_RE.findall(body):
        value = int(value_text)
        scl = int(scl_text)
        if scl == 4 and 0 <= value < len(HARD_REG_NAMES):
            locals_[name] = hard_reg(value)
        elif scl == 1:
            locals_[name] = "stack"
    return frame, locals_


def measure_exact(func: str, obj: Path) -> int:
    result = subprocess.run(
        [sys.executable, str(MEASURE), func, str(obj)],
        cwd=ROOT,
        capture_output=True,
        text=True,
        check=False,
    )
    if result.returncode not in (0, 1):
        raise AllocDiffError(result.stderr.strip() or result.stdout.strip() or "measurement failed")
    match = re.search(r"^\s*exact\s+(\d+)", result.stdout, re.MULTILINE)
    if not match:
        raise AllocDiffError(f"could not parse exact metric:\n{result.stdout}")
    return int(match.group(1))


def compile_candidate(
    func: str,
    label: str,
    source: Path,
    tmp: Path,
    keep: Path | None,
) -> Candidate:
    raw_asm_path, lreg_path, greg_path, obj = compile_with_dumps(source, tmp)
    raw_asm = raw_asm_path.read_text(errors="replace")
    lreg = lreg_path.read_text(errors="replace")
    greg = greg_path.read_text(errors="replace")
    alias = source_identifier(source, func)
    order, allocnos, dispositions = reconstruct_allocnos(lreg, greg, func, alias)
    frame, locals_ = parse_frame_and_locals(raw_asm, func)
    exact = measure_exact(func, obj)
    if keep is not None:
        keep.mkdir(parents=True, exist_ok=True)
        shutil.copy(raw_asm_path, keep / f"{label}.s")
        shutil.copy(lreg_path, keep / f"{label}.lreg")
        shutil.copy(greg_path, keep / f"{label}.greg")
    return Candidate(
        label=label,
        source=source,
        exact=exact,
        frame=frame,
        order=order,
        allocnos=allocnos,
        dispositions=dispositions,
        locals=locals_,
    )


def allocno_name(allocno: Allocno) -> str:
    if len(allocno.members) == 1:
        return f"p{allocno.representative}"
    members = ",".join(f"p{member}" for member in allocno.members)
    return f"p{allocno.representative}{{{members}}}"


def print_candidate(candidate: Candidate) -> None:
    print(f"\n== {candidate.label}: {candidate.source} ==")
    print(f"exact: {candidate.exact}")
    print(candidate.frame)
    print("rank  allocno       refs  live size priority  color")
    for rank, representative in enumerate(candidate.order, 1):
        allocno = candidate.allocnos[representative]
        print(
            f"{rank:4}  {allocno_name(allocno):<12} "
            f"{allocno.refs:5} {allocno.live:5} {allocno.size:4} "
            f"{allocno.priority:8}  {hard_reg(allocno.color)}"
        )


def movement(before_rank: int, after_rank: int) -> str:
    delta = after_rank - before_rank
    if delta > 0:
        return f"later {delta}"
    if delta < 0:
        return f"earlier {-delta}"
    return "same"


def matching_local_names(
    before: Candidate,
    after: Candidate,
    before_location: str,
    after_location: str,
) -> list[str]:
    return sorted(
        name
        for name in before.locals.keys() & after.locals.keys()
        if before.locals[name] == before_location and after.locals[name] == after_location
    )


def print_diff(before: Candidate, after: Candidate) -> None:
    print("\n== allocation diff ==")
    print(f"exact: {before.exact} -> {after.exact} ({after.exact - before.exact:+d})")
    if before.frame != after.frame:
        print(f"frame: {before.frame} -> {after.frame}")
    else:
        print(f"frame: unchanged ({before.frame})")

    common = before.allocnos.keys() & after.allocnos.keys()
    before_rank = {pseudo: rank for rank, pseudo in enumerate(before.order, 1)}
    after_rank = {pseudo: rank for rank, pseudo in enumerate(after.order, 1)}
    changed = [
        pseudo
        for pseudo in common
        if (
            before_rank[pseudo] != after_rank[pseudo]
            or before.allocnos[pseudo].refs != after.allocnos[pseudo].refs
            or before.allocnos[pseudo].live != after.allocnos[pseudo].live
            or before.allocnos[pseudo].priority != after.allocnos[pseudo].priority
        )
    ]
    changed.sort(
        key=lambda pseudo: (
            -abs(after_rank[pseudo] - before_rank[pseudo]),
            before_rank[pseudo],
        )
    )
    print("\norder changes:")
    if not changed:
        print("  none")
    else:
        print("  allocno       rank/move       refs       live       priority")
        for pseudo in changed:
            old = before.allocnos[pseudo]
            new = after.allocnos[pseudo]
            print(
                f"  {allocno_name(old):<12} "
                f"{before_rank[pseudo]:4}->{after_rank[pseudo]:<4} "
                f"{movement(before_rank[pseudo], after_rank[pseudo]):<9} "
                f"{old.refs:4}->{new.refs:<4} "
                f"{old.live:4}->{new.live:<4} "
                f"{old.priority:7}->{new.priority:<7}"
            )

    removed = sorted(before.allocnos.keys() - after.allocnos.keys())
    added = sorted(after.allocnos.keys() - before.allocnos.keys())
    if removed:
        print("  removed allocnos: " + " ".join(f"p{pseudo}" for pseudo in removed))
    if added:
        print("  added allocnos:   " + " ".join(f"p{pseudo}" for pseudo in added))

    print("\ncoloring changes:")
    color_changes = [
        pseudo
        for pseudo in common
        if before.allocnos[pseudo].color != after.allocnos[pseudo].color
    ]
    color_changes.sort(key=lambda pseudo: before_rank[pseudo])
    if not color_changes:
        print("  none")
    for pseudo in color_changes:
        old_location = hard_reg(before.allocnos[pseudo].color)
        new_location = hard_reg(after.allocnos[pseudo].color)
        names = matching_local_names(before, after, old_location, new_location)
        annotation = f" [{', '.join(names)}]" if names else ""
        print(f"  p{pseudo}: {old_location} -> {new_location}{annotation}")

    source_changes = [
        (name, before.locals[name], after.locals[name])
        for name in before.locals.keys() & after.locals.keys()
        if before.locals[name] != after.locals[name]
    ]
    source_changes.sort()
    print("\nsource-local coloring changes:")
    if not source_changes:
        print("  none identifiable from COFF definitions")
    else:
        for name, old_location, new_location in source_changes:
            print(f"  {name}: {old_location} -> {new_location}")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("func", help="func_XXXXXXXX")
    parser.add_argument("before", type=Path, help="candidate before the source edit")
    parser.add_argument("after", type=Path, help="candidate after the source edit")
    parser.add_argument(
        "--keep-dumps",
        type=Path,
        metavar="DIR",
        help="copy before/after .s, .lreg and .greg evidence into DIR",
    )
    args = parser.parse_args()

    if not re.fullmatch(r"[A-Za-z_]\w*", args.func):
        raise SystemExit("function must be a C identifier or func_XXXXXXXX")
    before_path = args.before.resolve()
    after_path = args.after.resolve()
    for path in (before_path, after_path):
        if not path.is_file():
            raise SystemExit(f"candidate does not exist: {path}")

    try:
        with tempfile.TemporaryDirectory(prefix="alloc-diff-before-") as before_tmp_name:
            with tempfile.TemporaryDirectory(prefix="alloc-diff-after-") as after_tmp_name:
                before = compile_candidate(
                    args.func,
                    "before",
                    before_path,
                    Path(before_tmp_name),
                    args.keep_dumps,
                )
                after = compile_candidate(
                    args.func,
                    "after",
                    after_path,
                    Path(after_tmp_name),
                    args.keep_dumps,
                )
        print_candidate(before)
        print_candidate(after)
        print_diff(before, after)
    except (AllocDiffError, OSError, subprocess.SubprocessError) as exc:
        print(f"alloc_diff: {exc}", file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    sys.exit(main())
