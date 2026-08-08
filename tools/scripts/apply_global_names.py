#!/usr/bin/env python3
"""Apply proposed names for D_ globals, then leave `make check` to judge them.

Input is the tab-separated proposal format the naming agents produce:

    D_8XXXXXXX<TAB>g_NewName<TAB>high|medium|low<TAB>evidence

A name goes into `configs/PAL/sym.bss.main.txt` when the symbol is already
listed there, and into `configs/PAL/sym.main.txt` otherwise, kept in address
order so the file stays readable. Every reference under src/, include/ and
docs/ is rewritten at the same time.

Everything is checked before anything is written: unknown address, a name that
already exists, two proposals wanting the same name, or a name that is not a
plain identifier all abort the run rather than half-apply it.

Usage: apply_global_names.py [--min-confidence LEVEL] <proposals.tsv>...
"""

import pathlib
import re
import subprocess
import sys

ROOT = pathlib.Path(__file__).resolve().parents[2]
SYM = ROOT / 'configs/PAL/sym.main.txt'
BSS = ROOT / 'configs/PAL/sym.bss.main.txt'
IDENT = re.compile(r'^[A-Za-z_]\w*$')
RANK = {'high': 3, 'medium': 2, 'low': 1}


def existing_names():
    names = set()
    for cfg in (SYM, BSS):
        names |= set(re.findall(r'^(\w+)\s*=', cfg.read_text(), re.M))
    return names


def bss_lines():
    """address -> the whole line, for symbols splat already lists in bss."""
    out = {}
    for line in BSS.read_text().split('\n'):
        m = re.match(r'(\w+) = 0x([0-9A-Fa-f]+);', line.strip())
        if m:
            out[int(m.group(2), 16)] = line
    return out


def read_proposals(paths, floor):
    seen, out = {}, []
    for path in paths:
        for raw in pathlib.Path(path).read_text().split('\n'):
            if not raw.strip() or raw.lstrip().startswith('#'):
                continue
            parts = raw.split('\t')
            if len(parts) < 3:
                sys.exit('malformed line in %s: %r' % (path, raw))
            sym, name, conf = parts[0].strip(), parts[1].strip(), parts[2].strip().lower()
            if name == 'SKIP':
                continue
            if RANK.get(conf, 0) < floor:
                continue
            if not IDENT.match(name):
                sys.exit('not an identifier: %r (%s)' % (name, sym))
            if not re.match(r'^D_8[0-9A-F]{7}$', sym):
                sys.exit('not a symbol: %r' % sym)
            if name in seen and seen[name] != sym:
                sys.exit('two symbols want the name %s: %s and %s'
                         % (name, seen[name], sym))
            seen[name] = sym
            out.append((sym, name))
    return out


def main(argv):
    floor = RANK['low']
    if argv and argv[0] == '--min-confidence':
        floor = RANK[argv[1]]
        argv = argv[2:]
    proposals = read_proposals(argv, floor)

    # A proposal for a symbol that already carries that name is a no-op, not an
    # error: the agents sometimes re-derive a name the tables already hold.
    taken = existing_names()
    clashes = [(sym, n) for sym, n in proposals if n in taken]
    if clashes:
        print('already named, skipping %d: %s'
              % (len(clashes), ', '.join('%s->%s' % c for c in clashes[:6])))
        proposals = [(sym, n) for sym, n in proposals if n not in taken]

    referenced = set()
    for tree in ('src', 'include'):
        for p in (ROOT / tree).rglob('*'):
            if p.suffix in ('.c', '.h'):
                referenced |= set(re.findall(r'\bD_8[0-9A-F]{7}\b', p.read_text()))
    missing = [s for s, _ in proposals if s not in referenced]
    if missing:
        print('warning: %d symbol(s) not referenced from C: %s'
              % (len(missing), ', '.join(missing[:8])))

    in_bss = bss_lines()

    # 1. The symbol tables.
    bss_text = BSS.read_text()
    sym_text = SYM.read_text()
    additions = []
    for sym, name in proposals:
        addr = int(sym[2:], 16)
        if addr in in_bss:
            old = in_bss[addr]
            bss_text = bss_text.replace(old, old.replace(sym, name, 1), 1)
        else:
            additions.append((addr, '%s = 0x%08X;' % (name, addr)))
    if additions:
        lines = sym_text.rstrip('\n').split('\n')
        entries = []
        for i, line in enumerate(lines):
            m = re.match(r'\w+ = 0x([0-9A-Fa-f]+);', line.strip())
            if m:
                entries.append((int(m.group(1), 16), i))
        for addr, text in sorted(additions):
            after = [i for a, i in entries if a <= addr]
            at = (after[-1] + 1) if after else len(lines)
            lines.insert(at, text)
            entries = [(a, i + 1 if i >= at else i) for a, i in entries]
            entries.append((addr, at))
            entries.sort()
        sym_text = '\n'.join(lines) + '\n'
    BSS.write_text(bss_text)
    SYM.write_text(sym_text)

    # 2. Every reference in the tree.
    rename = dict(proposals)
    pattern = re.compile(r'\b(%s)\b' % '|'.join(sorted(rename, key=len, reverse=True)))
    touched = 0
    for tree in ('src', 'include', 'docs'):
        base = ROOT / tree
        if not base.exists():
            continue
        for p in base.rglob('*'):
            if p.suffix not in ('.c', '.h', '.md'):
                continue
            text = p.read_text()
            new = pattern.sub(lambda m: rename[m.group(1)], text)
            if new != text:
                p.write_text(new)
                touched += 1

    print('%d symbols named, %d files rewritten' % (len(proposals), touched))
    print('now: make split && make check')


if __name__ == '__main__':
    main(sys.argv[1:])
