#!/usr/bin/env python3
"""Validate the report's denominator against every linked object's text.

Function names and boundaries are annotations, not independent discoveries.
This gate prevents those annotations from hiding nonzero bytes or counting
the same range twice. Unreported zero padding is permitted and counted.
"""

import argparse
import json
from pathlib import Path
import sys

from elftools.elf.elffile import ELFFile

sys.path.insert(0, str(Path(__file__).resolve().parent))
from gen_objdiff_config import linked_objects
from classify_embedded_data import checked_data_ranges

ROOT = Path(__file__).resolve().parents[2]


def check_ranges(functions, text, data=()):
    cursor = total = padding = 0
    names = set()
    for function in sorted([*functions, *data], key=lambda f: int(f.get('address', 0))):
        start = int(function.get('address', 0))
        size = int(function.get('size', 0))
        end = start + size
        if function['name'] in names:
            raise ValueError('duplicate function name')
        names.add(function['name'])
        if start < cursor or size <= 0 or end > len(text):
            raise ValueError('overlapping, empty or out-of-bounds function range')
        if any(text[cursor:start]):
            raise ValueError('nonzero text bytes omitted before function')
        padding += start - cursor
        if function not in data:
            total += size
        cursor = end
    if any(text[cursor:]):
        raise ValueError('nonzero text bytes omitted after final function')
    return total, padding + len(text) - cursor


def audit(report, objects, build):
    manifest = json.loads((ROOT / 'configs/embedded_data.json').read_text())
    expected = {obj.removesuffix('.o') for obj in objects}
    actual = [unit['metadata']['source_path'] for unit in report['units']]
    if len(actual) != len(set(actual)) or set(actual) != expected:
        raise ValueError('report units differ from linked objects')
    total_code = total_functions = padding = 0
    for unit in report['units']:
        source = unit['metadata']['source_path']
        with (build / (source + '.o')).open('rb') as handle:
            elf = ELFFile(handle)
            section = elf.get_section_by_name('.text')
            text = section.data() if section else b''
        try:
            data, _ = checked_data_ranges(build / (source + '.o'), manifest[source]) if source in manifest else ([], b'')
            size, omitted = check_ranges(unit.get('functions', []), text, data)
        except ValueError as error:
            raise ValueError('%s: %s' % (source, error)) from error
        measures = unit['measures']
        functions = len(unit.get('functions', []))
        if size != int(measures.get('total_code', 0)) or functions != int(measures.get('total_functions', 0)):
            raise ValueError('%s: unit totals differ from function ranges' % source)
        total_code += size
        total_functions += functions
        padding += omitted
    if (total_code != int(report['measures']['total_code'])
            or total_functions != int(report['measures']['total_functions'])):
        raise ValueError('overall totals differ from audited units')
    if (int(report['measures'].get('matched_code', 0)) != total_code
            or int(report['measures'].get('matched_functions', 0)) != total_functions):
        raise ValueError('report code is not an exact match; inspect raw objdiff output')
    return total_functions, total_code, padding


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--version', default='PAL')
    args = parser.parse_args()
    build = ROOT / 'build' / args.version
    objects = linked_objects((build / 'main.map').read_text(), args.version)
    report = json.loads((build / 'report.json').read_text())
    functions, code, padding = audit(report, objects, build)
    print('report audit: %d function ranges, %d code bytes, %d zero-padding bytes; '
          'all linked units accounted for' % (functions, code, padding))


if __name__ == '__main__':
    main()
