#!/usr/bin/env python3
"""Write objdiff.json: one unit per object the linker actually consumes.

The unit list has to come from the map rather than from a glob over src/,
because the two disagree in both directions - the map also carries the
assembled data blobs, and a stray .c that no segment references would be
counted as unmatched work that does not exist.

Every path under expected/<version>/build mirrors build/<version>, so a unit
only needs its name once. Units are marked complete because this tree links to
the original SHA-1; `make check` is what makes that claim, and it runs before
this does.
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


def is_sdk(relative):
    """True for the PsyQ libraries, which are Sony's code rather than the game.

    They had to be matched to relink the executable, but reporting them would
    credit this project with someone else's work. tools/scripts/progress_report.py
    draws the same line, so the site and the badge agree.
    """
    return '/lib/' in relative


def excluded(relative, unscorable):
    """Why this object stays out of the report, or None if it belongs in it."""
    if is_sdk(relative):
        return 'PsyQ library'
    if unit_name(relative) in unscorable:
        return 'objdiff cannot pair its symbols'
    return None


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
        units.append({
            'name': unit_name(relative),
            'target_path': '%s/%s/build/%s' % (expected_dir, version, relative),
            'base_path': 'build/%s/%s' % (version, relative),
            'metadata': {
                'complete': True,
                'source_path': relative.removesuffix('.o'),
            },
        })
    return {
        '$schema': SCHEMA,
        'min_version': '2.0.0',
        'custom_make': 'make',
        'custom_args': ['-j'],
        'build_target': False,
        'build_base': True,
        'watch_patterns': ['src/**/*.c', 'src/**/*.h', 'include/**/*.h', 'asm/**/*.s',
                           'configs/**/*.yaml', 'configs/**/*.txt', 'Makefile'],
        'units': units,
    }


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--version', default='PAL')
    parser.add_argument('--basename', default='main')
    parser.add_argument('--expected', default='expected')
    parser.add_argument('--output', default='objdiff.json')
    # Nothing needs this today. It stays because objdiff refuses to score a
    # whole unit when it cannot pair one symbol, and the failure names no unit,
    # so having somewhere to put one beats rediscovering that under pressure.
    parser.add_argument('--skip', action='append', default=[],
                        help='unit objdiff cannot score; repeatable')
    args = parser.parse_args(argv)

    map_path = ROOT / 'build' / args.version / ('%s.map' % args.basename)
    if not map_path.exists():
        raise SystemExit('%s missing - run `make build check` first' % map_path)

    objects = linked_objects(map_path.read_text(), args.version)
    unscorable = set(args.skip)
    kept, dropped = [], {}
    for relative in objects:
        why = excluded(relative, unscorable)
        if why is None:
            kept.append(relative)
        else:
            dropped.setdefault(why, []).append(unit_name(relative))

    written = config(kept, args.version, args.expected)
    (ROOT / args.output).write_text(json.dumps(written, indent=2) + '\n')
    print('%s: %d of %d units' % (args.output, len(kept), len(objects)))
    # Say what was left out and why. A report that quietly drops units reads as
    # though it covered everything.
    for why, names in sorted(dropped.items()):
        print('  excluded, %s: %d unit(s)%s' % (
            why, len(names), '' if len(names) > 3 else ' - %s' % ', '.join(names)))
    return 0


if __name__ == '__main__':
    sys.exit(main())
