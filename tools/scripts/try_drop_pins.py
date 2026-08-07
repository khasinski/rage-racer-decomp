#!/usr/bin/env python3
"""Drop `register T x asm("$N")` hints that the compiler no longer needs.

A pin forces one local into one hardware register. Most were added while a
function was being matched and stopped carrying their weight once the
surrounding code settled, but nothing tells you which - so this tries each one
in turn and keeps only the removals that leave the object file bit-identical.

Verification is per object, not per link: it rebuilds the single .o and
compares it with the baseline copy taken before the edit, which is both faster
and stricter than `make check` (a stale .o cannot fake a pass).

Usage: try_drop_pins.py <file.c>...
"""

import pathlib
import re
import shutil
import subprocess
import sys

ROOT = pathlib.Path(__file__).resolve().parents[2]
BUILD = ROOT / 'build' / 'PAL'
CC = ROOT / 'tools' / 'scripts' / 'cc.sh'

PIN = re.compile(r'^(?P<indent>\s*)register\s+(?P<decl>.+?)\s+asm\("\$\d+"\)'
                 r'(?P<tail>\s*(?:=[^;]*)?;)\s*$', re.MULTILINE)


def object_for(src):
    return BUILD / (str(src.relative_to(ROOT)) + '.o')


def compile_one(src):
    obj = object_for(src)
    obj.parent.mkdir(parents=True, exist_ok=True)
    if obj.exists():
        obj.unlink()  # never let a stale object answer the question
    result = subprocess.run([str(CC), str(src.relative_to(ROOT)), str(obj.relative_to(ROOT))],
                            cwd=ROOT, capture_output=True, text=True,
                            errors='replace')
    return result.returncode == 0 and obj.exists()


def main(argv):
    dropped = kept = 0
    for name in argv:
        src = (ROOT / name).resolve()
        obj = object_for(src)
        if not compile_one(src):
            print('%s: baseline build failed, skipping' % name)
            continue
        baseline = obj.with_suffix('.o.baseline')
        shutil.copy(obj, baseline)
        original = src.read_text()

        pins = list(PIN.finditer(original))
        print('%s: %d pin(s)' % (name, len(pins)))
        text = original
        for index in range(len(pins)):
            attempt = list(PIN.finditer(text))
            if index >= len(attempt):
                break
            m = attempt[index]
            replacement = '%s%s%s' % (m.group('indent'), m.group('decl'), m.group('tail'))
            candidate = text[:m.start()] + replacement + text[m.end():]
            src.write_text(candidate)
            if compile_one(src) and obj.read_bytes() == baseline.read_bytes():
                text = candidate
                dropped += 1
                print('  dropped: %s' % m.group(0).strip())
            else:
                kept += 1
                print('  load-bearing: %s' % m.group(0).strip())
        src.write_text(text)
        compile_one(src)
        baseline.unlink()
    print('%d dropped, %d load-bearing' % (dropped, kept))


if __name__ == '__main__':
    main(sys.argv[1:])
