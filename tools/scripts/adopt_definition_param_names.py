#!/usr/bin/env python3
"""Give forward declarations the parameter names the definition already uses.

The same function is often declared locally in five files as `f(s32 arg0, s32
arg1)` while its definition names the parameters properly. A declaration's
parameter names mean nothing to the compiler, so this is text-only - but it
still runs the per-object check, because a typo that changes a *type* would
not be.

Usage: adopt_definition_param_names.py [path...]   (default: src)
"""

import pathlib
import re
import sys

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
from objverify import ROOT, snapshot, try_edit
from adopt_header_param_names import GENERIC, parse_param, split_params

DEFN = re.compile(r'^[A-Za-z_][\w \t\*]*?\b(\w+)\s*\(([^;{)]*)\)\s*\{', re.M)
DECL = re.compile(r'^(?P<lead>[ \t]*(?:extern\s+)?[A-Za-z_][\w \t\*]*?\b)'
                  r'(?P<name>\w+)\s*\((?P<args>[^;{)]*)\)(?P<tail>[^;\n]*);', re.M)


def definition_names(sources):
    """name -> [(type, name)] taken from the function's own definition."""
    found = {}
    for src in sources:
        for m in DEFN.finditer(src.read_text()):
            params = [parse_param(p) for p in split_params(m.group(2))]
            if not params or any(p is None for p in params):
                continue
            if any(GENERIC.match(n) for _, n in params):
                continue
            found.setdefault(m.group(1), params)
    return found


def main(argv):
    roots = [ROOT / a for a in argv] if argv else [ROOT / 'src']
    sources = []
    for root in roots:
        sources.extend(sorted(root.rglob('*.c')) if root.is_dir() else [root])
    all_sources = sorted((ROOT / 'src').rglob('*.c'))
    names = definition_names(all_sources)

    renamed = reverted = 0
    for src in sources:
        text = src.read_text()
        plan = []
        for m in DECL.finditer(text):
            wanted = names.get(m.group('name'))
            if not wanted:
                continue
            params = [parse_param(p) for p in split_params(m.group('args'))]
            if not params or any(p is None for p in params):
                continue
            if len(params) != len(wanted):
                continue
            if not any(GENERIC.match(n) for _, n in params):
                continue
            if any(have != want for (have, _), (want, _) in zip(params, wanted)):
                continue        # types must agree exactly
            args = ', '.join('%s %s' % (t, n) for t, n in wanted)
            plan.append((m.group(0), '%s%s(%s)%s;' % (m.group('lead'), m.group('name'),
                                                     args, m.group('tail'))))
        if not plan:
            continue
        baseline = snapshot(src)
        if baseline is None:
            print('%s: baseline build failed, skipping' % src)
            continue
        for old, new in plan:
            original = src.read_text()
            if old not in original:
                continue
            candidate = original.replace(old, new, 1)
            if try_edit(src, original, candidate, baseline):
                renamed += 1
                print('  %s: %s' % (src.name, new.strip()))
            else:
                reverted += 1
        baseline.unlink()
    print('%d declarations renamed, %d reverted' % (renamed, reverted))


if __name__ == '__main__':
    main(sys.argv[1:])
