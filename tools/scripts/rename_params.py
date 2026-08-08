#!/usr/bin/env python3
"""Rename one function's parameters (and colliding locals), verified per object.

Parameter names cannot change what the compiler emits, so a rename that moves
the object means the rename was wrong - almost always because the body already
had a local by that name and the two collapsed into one. The per-object check
catches that and puts the file back.

Import `rename` and call it with the old signature, the new one, and the
(old, new) pairs to apply inside the body.
"""
import pathlib, re, sys
sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
from objverify import snapshot, try_edit
from adopt_header_param_names import body_end

def rename(path, signature, new_signature, pairs):
    p = pathlib.Path(path)
    base = snapshot(p)
    if base is None:
        print('%s: baseline failed' % path); return False
    original = p.read_text()
    if signature not in original:
        print('MISS %s: %s' % (path, signature[:60])); base.unlink(); return False
    start = original.index(signature)
    end = body_end(original, original.index('{', start))
    # Rename inside the body first, then swap the signature in: doing it the
    # other way round lets a body rename rewrite the new signature too.
    chunk = original[start:end]
    for old, new in pairs:
        chunk = re.sub(r'\b%s\b' % old, new, chunk)
    head_end = chunk.index('{') + 1
    chunk = new_signature + chunk[head_end:]
    cand = original[:start] + chunk + original[end:]
    ok = try_edit(p, original, cand, base)
    base.unlink()
    print(('  ok   ' if ok else '  MOVED ') + path + ': ' + new_signature[:70])
    return ok
