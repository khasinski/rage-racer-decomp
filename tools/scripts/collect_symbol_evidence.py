#!/usr/bin/env python3
"""Collect, for every still-unnamed D_ global, everything that could name it.

Writes one Markdown dossier per subsystem into the directory given as the first
argument. Each entry carries the symbol's address, its declarations, every line
of C that touches it, and - for rodata/data symbols - the bytes behind it, since
a table of pointers or a run of ASCII usually settles what the thing is.
"""

import collections
import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parents[2]
SYMBOL = re.compile(r'\bD_8[0-9A-F]{7}\b')

# Which subsystem a source path belongs to; the dossiers are split this way so
# one agent sees one coherent area.
AREAS = {'menu', 'car', 'track', 'race', 'render', 'sdk', 'libsnd', 'libspu',
         'kernel', 'asset', 'save', 'fmv', 'audio', 'cd', 'boot', 'gte', 'pad'}


def subsystem(path):
    """The leaf directory under src/main/PAL/{main,lib}/ - `main` appears twice
    in these paths, so match the known area names instead of indexing."""
    for part in reversed(pathlib.Path(path).parts):
        if part in AREAS:
            return part
    return 'other'


def main(argv):
    outdir = pathlib.Path(argv[0])
    outdir.mkdir(parents=True, exist_ok=True)

    uses = collections.defaultdict(list)      # symbol -> [(file, lineno, text)]
    for src in sorted((ROOT / 'src').rglob('*.c')):
        rel = str(src.relative_to(ROOT))
        for n, line in enumerate(src.read_text().split('\n'), 1):
            for sym in set(SYMBOL.findall(line)):
                uses[sym].append((rel, n, line.strip()))
    for hdr in sorted((ROOT / 'include').rglob('*.h')):
        rel = str(hdr.relative_to(ROOT))
        for n, line in enumerate(hdr.read_text().split('\n'), 1):
            for sym in set(SYMBOL.findall(line)):
                uses[sym].append((rel, n, line.strip()))

    # The bytes behind each data/rodata label, straight out of the split output.
    data = {}
    for asm in sorted((ROOT / 'asm').rglob('*.s')):
        text = asm.read_text()
        for m in re.finditer(r'^dlabel (D_8[0-9A-F]{7})\n(.*?)^enddlabel \1$',
                             text, re.M | re.S):
            data[m.group(1)] = m.group(2).rstrip('\n')

    # A symbol belongs to the subsystem that references it most.
    owner = {}
    for sym, sites in uses.items():
        counts = collections.Counter(subsystem(f) for f, _, _ in sites
                                     if f.startswith('src/'))
        owner[sym] = counts.most_common(1)[0][0] if counts else 'headers-only'

    grouped = collections.defaultdict(list)
    for sym in sorted(uses):
        if sym in ('D_8007FB4C',):        # keep any known-special cases visible
            pass
        grouped[owner[sym]].append(sym)

    for area, syms in sorted(grouped.items()):
        out = ['# Unnamed globals used mainly by %s/ (%d symbols)\n' % (area, len(syms))]
        for sym in syms:
            out.append('\n## %s   (address 0x%s)\n' % (sym, sym[2:]))
            sites = uses[sym]
            files = sorted({f for f, _, _ in sites})
            out.append('Referenced in %d place(s) across: %s\n'
                       % (len(sites), ', '.join(files)))
            out.append('\n```c')
            for f, n, line in sites[:60]:
                out.append('%s:%d: %s' % (f, n, line))
            if len(sites) > 60:
                out.append('... %d more' % (len(sites) - 60))
            out.append('```\n')
            if sym in data:
                body = data[sym]
                lines = body.split('\n')
                out.append('Bytes at this label:\n')
                out.append('```')
                out.extend(lines[:40])
                if len(lines) > 40:
                    out.append('... %d more lines' % (len(lines) - 40))
                out.append('```\n')
        (outdir / ('%s.md' % area)).write_text('\n'.join(out))
        print('%-14s %3d symbols -> %s.md' % (area, len(syms), area))


if __name__ == '__main__':
    main(sys.argv[1:])
