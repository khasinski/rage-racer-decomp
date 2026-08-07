#!/usr/bin/env python3
"""Compile one translation unit and say whether an edit left it bit-identical.

Shared by the cleanup scripts. The rule everywhere is the same: take a
baseline object before the edit, delete the object, rebuild, compare. Deleting
first matters - a stale .o has twice made a failed edit look like a pass.
"""

import pathlib
import shutil
import subprocess

ROOT = pathlib.Path(__file__).resolve().parents[2]
BUILD = ROOT / 'build' / 'PAL'
CC = ROOT / 'tools' / 'scripts' / 'cc.sh'


def object_for(src):
    return BUILD / (str(pathlib.Path(src).resolve().relative_to(ROOT)) + '.o')


def compile_one(src):
    src = pathlib.Path(src).resolve()
    obj = object_for(src)
    obj.parent.mkdir(parents=True, exist_ok=True)
    if obj.exists():
        obj.unlink()
    done = subprocess.run([str(CC), str(src.relative_to(ROOT)), str(obj.relative_to(ROOT))],
                          cwd=ROOT, capture_output=True, text=True, errors='replace')
    return done.returncode == 0 and obj.exists()


def snapshot(src):
    """Build `src` and stash the object; returns the stash path or None."""
    if not compile_one(src):
        return None
    obj = object_for(src)
    keep = obj.with_suffix('.o.baseline')
    shutil.copy(obj, keep)
    return keep


def matches(src, baseline):
    """True when `src` still compiles to exactly the baseline object."""
    return compile_one(src) and object_for(src).read_bytes() == baseline.read_bytes()


def try_edit(src, original, candidate, baseline):
    """Write `candidate`; keep it if the object is unchanged, else restore."""
    src = pathlib.Path(src)
    src.write_text(candidate)
    if matches(src, baseline):
        return True
    src.write_text(original)
    compile_one(src)
    return False
