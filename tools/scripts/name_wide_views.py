#!/usr/bin/env python3
"""Name the parameters of `... asm("RealName")` wide-view declarations.

A wide view re-declares an existing function with wider argument types so the
call site passes a full word. The types must stay as they are - that is the
whole point - but the names can come from the real function.
"""
import pathlib, re, sys
sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
from objverify import ROOT, snapshot, try_edit
from adopt_header_param_names import GENERIC, parse_param, split_params

DEFN = re.compile(r'^[A-Za-z_][\w \t\*]*?\b(\w+)\s*\(([^;{)]*)\)\s*\{', re.M)
names = {}
for src in sorted((ROOT / 'src').rglob('*.c')):
    for m in DEFN.finditer(src.read_text()):
        params = [parse_param(p) for p in split_params(m.group(2))]
        if not params or any(p is None for p in params):
            continue
        if any(GENERIC.match(n) for _, n in params):
            continue
        names.setdefault(m.group(1), [n for _, n in params])
for h in sorted((ROOT / 'include').rglob('*.h')):
    for m in re.finditer(r'\b(\w+)\s*\(([^;{)]*)\)\s*;', h.read_text()):
        params = [parse_param(p) for p in split_params(m.group(2))]
        if not params or any(p is None for p in params):
            continue
        if any(GENERIC.match(n) for _, n in params):
            continue
        names.setdefault(m.group(1), [n for _, n in params])

ALIAS = re.compile(r'^(?P<head>[A-Za-z_][\w \t\*]*?\b\w+\s*)\((?P<args>[^;{)]*)\)(?P<mid>\s*asm\("(?P<real>\w+)"\)\s*);',
                   re.M | re.S)
renamed = skipped = 0
for src in sorted((ROOT / 'src').rglob('*.c')):
    text = src.read_text()
    plan = []
    for m in ALIAS.finditer(text):
        want = names.get(m.group('real'))
        if not want:
            continue
        params = [parse_param(p) for p in split_params(m.group('args'))]
        if not params or any(p is None for p in params) or len(params) != len(want):
            continue
        if not any(GENERIC.match(n) for _, n in params):
            continue
        args = ', '.join('%s %s' % (t, n) for (t, _), n in zip(params, want))
        plan.append((m.group(0), '%s(%s)%s;' % (m.group('head'), args, m.group('mid'))))
    if not plan:
        continue
    base = snapshot(src)
    if base is None:
        print('%s: baseline failed' % src); continue
    for old, new in plan:
        original = src.read_text()
        if old not in original:
            continue
        if try_edit(src, original, original.replace(old, new, 1), base):
            renamed += 1; print('  %s: %s' % (src.name, ' '.join(new.split())[:90]))
        else:
            skipped += 1
    base.unlink()
print('%d wide views named, %d skipped' % (renamed, skipped))
