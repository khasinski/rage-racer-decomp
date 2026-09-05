#!/usr/bin/env python3
"""Write objdiff.json: one unit per object the linker actually consumes.

Every object goes in. decomp.dev requires a report to account for every
function in the binary so that projects on the site are measured the same way,
so the game and the libraries are separated with a progress category rather
than by dropping the libraries: `game` is the work of this project, `psyq` is
Sony's library code that had to be matched to relink the executable. Both
numbers stay visible and they add up to the whole executable.

The unit list comes from the map rather than from a glob over src/, because the
two disagree in both directions - the map also carries the assembled data
blobs, and a stray .c that no segment references would be counted as unmatched
work that does not exist.

Every path under expected/<version>/build mirrors build/<version>, so a unit
only needs its name once. Completion describes C source recovery, independently
of matching: included assembly is retained in the report but is not marked as
decompiled C. Source-form categories expose inline assembly and data as well.
"""

import argparse
import json
import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parents[2]

MAP_PLACEMENT = re.compile(
    r'^\s\.\w+\s+0x[0-9a-f]+\s+0x[0-9a-f]+\s+(build/\S+\.o)$', re.MULTILINE)

SCHEMA = 'https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json'


def linked_objects(map_text, version):
    """Objects the map places, in link order, as paths relative to the build dir."""
    prefix = 'build/%s/' % version
    seen = []
    for obj in MAP_PLACEMENT.findall(map_text):
        if obj.startswith(prefix) and obj not in seen:
            seen.append(obj)
    return [obj[len(prefix):] for obj in seen]


CATEGORIES = [
    {'id': 'game', 'name': 'Game code'},
    {'id': 'psyq', 'name': 'PsyQ libraries'},
    {'id': 'source_c', 'name': 'C units (header intrinsics allowed)'},
    {'id': 'source_inline', 'name': 'C units with inline ASM or compiler constraints'},
    {'id': 'source_assembly', 'name': 'Units with retained ASM or raw opcodes'},
    {'id': 'source_data', 'name': 'Extracted data and BSS layout'},
]


def source_form(relative):
    """Classify source syntax, not inferred original authorship or byte match.

    A mixed unit stays in the assembly category in its entirety. Consequently
    these per-unit measures are not estimates of the number of C instructions.
    Header intrinsics are allowed in the C categories; pins and barriers do not
    imply that the original author wrote assembly.
    """
    path = ROOT / relative.removesuffix('.o')
    if relative.startswith('asm/'):
        return 'source_data'
    if path.suffix == '.s':
        return 'source_assembly'
    # Preserve string contents while removing comments, so a mention of
    # HANDWRITTEN_ASM in a historical note cannot change the classification.
    text = path.read_text()
    text = re.sub(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|/\*.*?\*/|//[^\n]*',
                  lambda m: ' ' if m[0].startswith(('/*', '//')) else m[0],
                  text, flags=re.DOTALL)
    if re.search(r'\b(?:HANDWRITTEN_ASM|INCLUDE_ASM)\s*\(', text):
        return 'source_assembly'
    literals = re.findall(r'"(?:\\.|[^"\\])*"', text)
    if any(re.search(r'\.(?:include|incbin|word|byte)\b', literal) for literal in literals):
        return 'source_assembly'
    # BIOS trampolines are also stored as C arrays of raw instruction words.
    # Only provably zero padding is exempt from retained-assembly status.
    for initializer in re.findall(
            r'section\s*\(\s*"\.text"\s*\)\s*\)\s*\)\s*=\s*([^;]+);', text):
        remainder = re.sub(r'\b(?:0[xX]0+|0+)[uUlL]*\b', '',
                           re.sub(r'[{},]', ' ', initializer)).strip()
        if remainder:
            return 'source_assembly'
    if re.search(r'\b(?:asm|__asm__)\s*(?:volatile\s*)?\(', text):
        return 'source_inline'
    return 'source_c'


def category(relative):
    """Which half of the binary an object belongs to.

    Sony's libraries live under lib/ and everything this project wrote lives
    under main/. Both are reported; the category only decides which subtotal
    the unit lands in.
    """
    return 'psyq' if '/lib/' in relative else 'game'


def unit_name(relative):
    """A readable name for the site: drop the build scaffolding around it."""
    name = relative.removesuffix('.o')
    for lead in ('src/main/', 'asm/'):
        if name.startswith(lead):
            name = name[len(lead):]
    return name.removesuffix('.c').removesuffix('.s')


def config(objects, version, expected_dir):
    units = []
    for relative in objects:
        form = source_form(relative)
        units.append({
            'name': unit_name(relative),
            'target_path': '%s/%s/build/%s' % (expected_dir, version, relative),
            'base_path': 'build/%s/%s' % (version, relative),
            'metadata': {
                'complete': form in ('source_c', 'source_inline'),
                'source_path': relative.removesuffix('.o'),
                'progress_categories': [category(relative), form],
            },
        })
    return {
        '$schema': SCHEMA,
        'min_version': '2.0.0',
        'custom_make': 'make',
        'custom_args': ['-j'],
        'build_target': False,
        'build_base': True,
        'watch_patterns': ['src/**/*.c', 'src/**/*.h', 'src/**/*.s', 'include/**/*.h',
                           'asm/**/*.s', 'configs/**/*.yaml', 'configs/**/*.txt', 'Makefile'],
        'progress_categories': CATEGORIES,
        'units': units,
    }


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--version', default='PAL')
    parser.add_argument('--basename', default='main')
    parser.add_argument('--expected', default='expected')
    parser.add_argument('--output', default='objdiff.json')
    args = parser.parse_args(argv)

    map_path = ROOT / 'build' / args.version / ('%s.map' % args.basename)
    if not map_path.exists():
        raise SystemExit('%s missing - run `make build check` first' % map_path)

    objects = linked_objects(map_path.read_text(), args.version)
    written = config(objects, args.version, args.expected)
    (ROOT / args.output).write_text(json.dumps(written, indent=2) + '\n')
    counts = {}
    for relative in objects:
        counts[category(relative)] = counts.get(category(relative), 0) + 1
    print('%s: %d units (%s)' % (args.output, len(objects),
                                 ', '.join('%s %d' % kv for kv in sorted(counts.items()))))
    return 0


if __name__ == '__main__':
    sys.exit(main())
