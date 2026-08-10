#!/usr/bin/env python3
"""Replace RAW(member) accesses with ordinary C when the object stays exact."""

import pathlib
import re
import sys

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
from objverify import ROOT, compile_one, snapshot, try_edit


RAW_ACCESS = re.compile(r"\bRAW\((?P<expression>[^()\n]+)\)")


def main(argv):
    dropped = kept = 0
    for name in argv:
        src = (ROOT / name).resolve()
        baseline = snapshot(src)
        if baseline is None:
            print("%s: baseline build failed, skipping" % name)
            continue

        text = src.read_text()
        print("%s: %d RAW access(es)" % (name, len(RAW_ACCESS.findall(text))))
        index = 0
        while True:
            matches = list(RAW_ACCESS.finditer(text))
            if index >= len(matches):
                break
            match = matches[index]
            candidate = text[:match.start()] + match.group("expression") + text[match.end():]
            if try_edit(src, text, candidate, baseline):
                text = candidate
                dropped += 1
                print("  dropped: %s" % match.group(0))
            else:
                kept += 1
                index += 1
                print("  load-bearing: %s" % match.group(0))

        src.write_text(text)
        compile_one(src)
        baseline.unlink()

    print("%d dropped, %d load-bearing" % (dropped, kept))


if __name__ == "__main__":
    main(sys.argv[1:])
