#!/usr/bin/env python3
"""Give definitions the parameter names their own header already spells out.

Plenty of function bodies still read `arg0`, `arg1`, ... while the declaration
in include/ names the same parameters properly. The names are already agreed;
this just moves them to where the code is read.

A rename cannot change what the compiler emits, so anything that stops matching
means the rename was wrong (a body-local of the same name changed meaning). The
per-object check catches exactly that, and the edit is reverted.

Usage: adopt_header_param_names.py [path...]   (default: src)
"""

import pathlib
import re
import sys

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
from objverify import snapshot, try_edit

ROOT = pathlib.Path(__file__).resolve().parents[2]
GENERIC = re.compile(r'^(arg\d+|a\d+|param\d+)$')


def split_params(text):
    """Split a parameter list on top-level commas."""
    out, depth, current = [], 0, ''
    for ch in text:
        if ch in '([':
            depth += 1
        elif ch in ')]':
            depth -= 1
        if ch == ',' and depth == 0:
            out.append(current)
            current = ''
        else:
            current += ch
    if current.strip():
        out.append(current)
    return [p.strip() for p in out]


def parse_param(param):
    """Return (type, name) for one declarator, or None if it has no name."""
    m = re.match(r'^(.*?)([A-Za-z_]\w*)\s*(\[\s*\])?$', param.strip())
    if not m or not m.group(1).strip():
        return None
    typ = re.sub(r'\s+', ' ', m.group(1).strip()).replace(' *', '*')
    return typ, m.group(2)


def header_names():
    """name -> [parameter names] for every prototype under include/."""
    found = {}
    for header in sorted((ROOT / 'include').rglob('*.h')):
        for m in re.finditer(r'\b(\w+)\s*\(([^;{)]*)\)\s*;', header.read_text()):
            params = [parse_param(p) for p in split_params(m.group(2))]
            if not params or any(p is None for p in params):
                continue
            if any(GENERIC.match(name) for _, name in params):
                continue
            found.setdefault(m.group(1), params)
    return found


def body_end(text, open_brace):
    depth = 0
    for i in range(open_brace, len(text)):
        if text[i] == '{':
            depth += 1
        elif text[i] == '}':
            depth -= 1
            if depth == 0:
                return i + 1
    return None


def main(argv):
    headers = header_names()
    roots = [ROOT / a for a in argv] if argv else [ROOT / 'src']
    renamed = reverted = 0

    sources = []
    for root in roots:
        sources.extend(sorted(root.rglob('*.c')) if root.is_dir() else [root])

    for src in sources:
        text = src.read_text()
        plan = []
        for m in re.finditer(r'^[A-Za-z_][\w \t\*]*?\b(\w+)\s*\(([^;{)]*)\)\s*\{', text, re.M):
            name = m.group(1)
            if name not in headers:
                continue
            params = [parse_param(p) for p in split_params(m.group(2))]
            if not params or any(p is None for p in params):
                continue
            wanted = headers[name]
            if len(wanted) != len(params):
                continue
            pairs = []
            for (have_type, have_name), (want_type, want_name) in zip(params, wanted):
                if have_type != want_type:      # positions must agree on type
                    pairs = []
                    break
                if have_name != want_name and GENERIC.match(have_name):
                    pairs.append((have_name, want_name))
            if pairs:
                plan.append((m.start(), pairs))
        if not plan:
            continue

        baseline = snapshot(src)
        if baseline is None:
            print('%s: baseline build failed, skipping' % src)
            continue

        for start, pairs in plan:
            original = src.read_text()
            m = re.compile(r'^[A-Za-z_][\w \t\*]*?\b\w+\s*\([^;{)]*\)\s*\{', re.M).search(original, start)
            if not m:
                continue
            end = body_end(original, m.end() - 1)
            if end is None:
                continue
            chunk = original[m.start():end]
            for old, new in pairs:
                if re.search(r'\b%s\b' % new, chunk):
                    break            # the name is already taken in this body
                chunk = re.sub(r'\b%s\b' % old, new, chunk)
            else:
                candidate = original[:m.start()] + chunk + original[end:]
                if try_edit(src, original, candidate, baseline):
                    renamed += 1
                    print('  %s: %s -> %s' % (src.name,
                                              ', '.join(o for o, _ in pairs),
                                              ', '.join(n for _, n in pairs)))
                else:
                    reverted += 1
        baseline.unlink()
    print('%d definitions renamed, %d reverted' % (renamed, reverted))


if __name__ == '__main__':
    main(sys.argv[1:])
