#!/usr/bin/env python3
"""Try to remove the `"memory"` clobbers that pin store order in matched code.

Each one was added to stop gcc 2.6.3 reordering a store past a load. Many stop
being needed once the surrounding code is spelled differently - most often when
a neighbouring `p->field` becomes a raw cast, which changes what the compiler
believes may alias - so none of them should be assumed necessary.

Two removals are tried per barrier, weakest first: drop just the "memory"
clobber (keeping any operands, which may still be doing the work), then drop
the whole statement. The first that leaves the object's code and data identical
wins.

Usage: try_drop_barriers.py <file.c>...
"""

import pathlib
import re
import sys

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
from objverify import ROOT, compile_one, snapshot, try_edit

BARRIER = re.compile(
    r'^(?P<indent>[ \t]*)(?:__asm__|asm)\s*(?:volatile|__volatile__)?\s*'
    r'\((?P<body>[^;]*?"memory"[^;]*?)\)\s*;[ \t]*\n', re.M)


def without_clobber(statement):
    """The same statement with the clobber list emptied, or None."""
    head, sep, clobbers = statement.rpartition(':')
    if not sep or '"memory"' not in clobbers:
        return None
    kept = [c.strip() for c in clobbers.split(',') if '"memory"' not in c]
    if not kept:
        # No other clobbers: the colon can go too, unless it is the separator
        # for an operand list that must stay.
        return head.rstrip().rstrip(':').rstrip() if head.count(':') >= 2 else None
    return '%s: %s' % (head, ', '.join(kept))


def main(argv):
    dropped = weakened = kept = 0
    for name in argv:
        src = (ROOT / name).resolve()
        baseline = snapshot(src)
        if baseline is None:
            print('%s: baseline build failed, skipping' % name)
            continue

        text = src.read_text()
        print('%s: %d barrier(s)' % (name, len(BARRIER.findall(text))))
        index = 0
        while True:
            matches = list(BARRIER.finditer(text))
            if index >= len(matches):
                break
            m = matches[index]
            statement = m.group(0)
            relaxed = without_clobber(statement.rstrip().rstrip(';'))

            attempts = []
            if relaxed is not None:
                attempts.append(('clobber', '%s;\n' % relaxed))
            attempts.append(('statement', ''))

            for label, replacement in attempts:
                candidate = text[:m.start()] + replacement + text[m.end():]
                if try_edit(src, text, candidate, baseline):
                    text = candidate
                    if label == 'clobber':
                        weakened += 1
                        print('  clobber dropped: %s' % statement.strip())
                    else:
                        dropped += 1
                        print('  barrier dropped: %s' % statement.strip())
                    break
            else:
                kept += 1
                index += 1
                print('  load-bearing: %s' % statement.strip())

        src.write_text(text)
        compile_one(src)
        baseline.unlink()
    print('%d barriers dropped, %d clobbers dropped, %d load-bearing'
          % (dropped, weakened, kept))


if __name__ == '__main__':
    main(sys.argv[1:])
