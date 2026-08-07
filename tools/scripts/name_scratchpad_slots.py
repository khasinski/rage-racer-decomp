#!/usr/bin/env python3
"""Rewrite raw scratchpad address casts into the game/scratchpad.h slot names.

The renderer reaches the PS1 scratchpad by literal address, which every
decompiled call site spelled out as `*(T *)0x1F8000NN`. The names mean the
same thing to the compiler - each macro expands to the identical cast - so
this is a readability change only, and `make check` proves it.

Usage: name_scratchpad_slots.py <path>...
"""

import re
import sys
import pathlib

# Slots whose macro already carries the dereference, keyed by (address, type).
WORD_SLOTS = {
    ('0x1F800000', 's32'): 'SCRATCH_PRIM_CURSOR_WORD',
    ('0x1F800004', 's32'): 'SCRATCH_OT_BASE_WORD',
    ('0x1F800008', 's32'): 'SCRATCH_VIEW_X',
    ('0x1F80000C', 's32'): 'SCRATCH_VIEW_Y',
    ('0x1F800010', 's32'): 'SCRATCH_VIEW_Z',
    ('0x1F800018', 's32'): 'SCRATCH_VIEW_ANGLE_X',
    ('0x1F80001C', 's32'): 'SCRATCH_VIEW_ANGLE_Y',
    ('0x1F800020', 's32'): 'SCRATCH_VIEW_ANGLE_Z',
    ('0x1F800048', 's32'): 'SCRATCH_COURSE_BANK',
    ('0x1F800050', 's32'): 'SCRATCH_MODEL_MODELS',
    ('0x1F800054', 's32'): 'SCRATCH_MODEL_TABLE1',
    ('0x1F800058', 's32'): 'SCRATCH_MODEL_NORMALS',
    ('0x1F80005C', 's32'): 'SCRATCH_CELL_TABLE',
    ('0x1F800060', 's32'): 'SCRATCH_CELL_FACES',
    ('0x1F800064', 's32'): 'SCRATCH_OT_SHIFT',
    ('0x1F800068', 's32'): 'SCRATCH_MIRROR',
    ('0x1F80006C', 's32'): 'SCRATCH_FACE_OT_SHIFT',
    ('0x1F800070', 'u8'): 'SCRATCH_FT4_R',
    ('0x1F800071', 'u8'): 'SCRATCH_FT4_G',
    ('0x1F800072', 'u8'): 'SCRATCH_FT4_B',
    ('0x1F800073', 'u8'): 'SCRATCH_FT4_CODE',
    ('0x1F800074', 'u8'): 'SCRATCH_GT4_R',
    ('0x1F800075', 'u8'): 'SCRATCH_GT4_G',
    ('0x1F800076', 'u8'): 'SCRATCH_GT4_B',
    ('0x1F800077', 'u8'): 'SCRATCH_GT4_CODE',
    ('0x1F800078', 'u16'): 'SCRATCH_CLIP_X0',
    ('0x1F80007A', 'u16'): 'SCRATCH_CLIP_Y0',
    ('0x1F80007C', 'u16'): 'SCRATCH_CLIP_X1',
    ('0x1F80007E', 'u16'): 'SCRATCH_CLIP_Y1',
    ('0x1F800084', 's32'): 'SCRATCH_ENV_MODE4',
}

# Slots reached through a double pointer, where the pointee type varies.
CURSOR_SLOTS = {
    '0x1F800000': 'SCRATCH_PRIM_CURSOR_AS',
    '0x1F800004': 'SCRATCH_OT_BASE_AS',
}

def normalise(addr):
    """0x1f800004 and 0X1F800004 name the same slot."""
    return '0x' + addr[2:].upper()


TYPE = r'(?:const\s+|volatile\s+|unsigned\s+|struct\s+)*[A-Za-z_][A-Za-z_0-9]*'


def rewrite(text):
    # *(T **)0xADDR -> SCRATCH_..._AS(T)   and   (T **)0xADDR -> &SCRATCH_..._AS(T)
    def cursor(m):
        star, typ, addr = m.group(1), m.group(2).strip(), normalise(m.group(3))
        macro = CURSOR_SLOTS.get(addr)
        if macro is None:
            return m.group(0)
        call = '%s(%s)' % (macro, typ)
        return call if star else '&' + call

    text = re.sub(
        r'(\*?)\(\s*(%s)\s*\*\s*\*\s*\)\s*(0x1[Ff]8000[0-9A-Fa-f]{2})' % TYPE,
        cursor, text)

    # *(T *)0xADDR -> MACRO   and   (T *)0xADDR -> &MACRO
    def word(m):
        star, typ, addr = m.group(1), m.group(2).strip(), normalise(m.group(3))
        macro = WORD_SLOTS.get((addr, typ))
        if macro is None:
            return m.group(0)
        return macro if star else '&' + macro

    text = re.sub(
        r'(\*?)\(\s*(%s)\s*\*\s*\)\s*(0x1[Ff]8000[0-9A-Fa-f]{2})' % TYPE,
        word, text)

    # The two whole-block views.
    text = text.replace('(GameScratchpadRenderState *)0x1F800000', 'SCRATCHPAD')
    text = re.sub(r'\(\s*(%s)\s*\*\s*\)\s*0x1[Ff]800000\b' % TYPE,
                  lambda m: '(%s *)SCRATCHPAD_ADDR' % m.group(1).strip(), text)

    # The view matrix, spelled with either pointer type.
    text = re.sub(r'\(\s*(?:Matrix|void)\s*\*\s*\)\s*0x1[Ff]800028\b',
                  'SCRATCH_VIEW_MATRIX_GTE', text)

    # Struct overlays of the view position block.
    text = re.sub(r'\(\s*(%s)\s*\*\s*\)\s*0x1[Ff]800008\b' % TYPE,
                  lambda m: '(%s *)&SCRATCH_VIEW_X' % m.group(1).strip(), text)
    text = re.sub(r'\(\s*(%s)\s*\*\s*\)\s*0x1[Ff]80000C\b' % TYPE,
                  lambda m: '(%s *)&SCRATCH_VIEW_Y' % m.group(1).strip(), text)
    text = re.sub(r'\(\s*(%s)\s*\*\s*\)\s*0x1[Ff]800010\b' % TYPE,
                  lambda m: '(%s *)&SCRATCH_VIEW_Z' % m.group(1).strip(), text)
    return text


INCLUDE = '#include "game/scratchpad.h"\n'


def ensure_include(text):
    """Every user of a SCRATCH_ name includes the header that defines it."""
    if INCLUDE in text:
        return text
    if 'SCRATCH_' not in text and 'SCRATCHPAD' not in text:
        return text
    lines = text.split('\n')
    last = None
    for i, line in enumerate(lines):
        if line.startswith('#include'):
            last = i
    if last is None:
        return INCLUDE + text
    # Keep the block sorted the way the tree spells it: common.h, then game/,
    # then psyq/.
    start = last
    while start > 0 and lines[start - 1].startswith('#include'):
        start -= 1
    block = lines[start:last + 1]
    block.append(INCLUDE.rstrip('\n'))
    quoted = [b for b in block if b.startswith('#include "')]
    other = [b for b in block if not b.startswith('#include "')]

    def key(inc):
        name = inc.split('"')[1]
        return (0 if name == 'common.h' else 1 if name.startswith('game/') else 2, name)

    block = other + sorted(set(quoted), key=key)
    lines[start:last + 1] = block
    return '\n'.join(lines)


def main(argv):
    changed = []
    for arg in argv:
        for path in sorted(pathlib.Path(arg).rglob('*.c')) or [pathlib.Path(arg)]:
            src = path.read_text()
            out = ensure_include(rewrite(src))
            if out != src:
                path.write_text(out)
                changed.append(str(path))
    for name in changed:
        print(name)
    print('%d file(s) rewritten' % len(changed))


if __name__ == '__main__':
    main(sys.argv[1:])
