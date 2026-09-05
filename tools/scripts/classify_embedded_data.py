#!/usr/bin/env python3
"""Move audited data objects in .text out of objdiff's function ledger.

objdiff v3.8 classifies report symbols by their containing section. Keep its
raw report intact, and publish a logical code/data ledger with the same units
and physical section extents. The manifest is checked against both ELF symbol
tables; data matching is computed directly against the retail executable.
"""

import argparse
import json
from pathlib import Path
import sys

from elftools.elf.elffile import ELFFile

sys.path.insert(0, str(Path(__file__).resolve().parent))
from gen_expected import parse_map_placement

ROOT = Path(__file__).resolve().parents[2]
BYTE_KEYS = ('total_code', 'matched_code', 'complete_code',
             'total_data', 'matched_data', 'complete_data')
COUNT_KEYS = ('total_functions', 'matched_functions', 'total_units', 'complete_units')


def checked_data_ranges(path, declarations):
    with path.open('rb') as handle:
        elf = ELFFile(handle)
        symbols = {s.name: s for s in elf.get_section_by_name('.symtab').iter_symbols()}
        ranges = []
        for name, (address, size) in declarations.items():
            symbol = symbols[name]
            if (symbol['st_info']['type'] != 'STT_OBJECT'
                    or not isinstance(symbol['st_shndx'], int)
                    or elf.get_section(symbol['st_shndx']).name != '.text'
                    or symbol['st_value'] != address or symbol['st_size'] != size):
                raise ValueError('%s: data declaration differs from ELF symbol %s' % (path, name))
            ranges.append({'name': name, 'address': str(address), 'size': str(size)})
        # A relocation would require comparing linked values rather than raw
        # bytes. None of these audited data objects contains a pointer relocation.
        for section in elf.iter_sections():
            if section['sh_type'] != 'SHT_REL' or elf.get_section(section['sh_info']).name != '.text':
                continue
            for rel in section.iter_relocations():
                if any(a <= rel['r_offset'] < a + s for a, s in declarations.values()):
                    raise ValueError('relocation inside audited embedded data')
        text = elf.get_section_by_name('.text').data()
    return ranges, text


def refresh_percentages(measures):
    for prefix, scope in [('matched', 'code'), ('complete', 'code'),
                          ('matched', 'data'), ('complete', 'data'),
                          ('matched', 'functions')]:
        total = int(measures.get('total_' + scope, 0))
        count = int(measures.get(prefix + '_' + scope, 0))
        measures[prefix + '_' + scope + '_percent'] = 100 * count / total if total else 100.0


def move_data(unit, ranges, base_text, retail_text):
    """Reclassify whole symbols only; refusing partial overlaps prevents hiding code."""
    removed = []
    for data in ranges:
        start, size = int(data['address']), int(data['size'])
        if size <= 0 or start < 0 or start + size > len(base_text) or len(base_text) != len(retail_text):
            raise ValueError('embedded data range outside text')
        for function in unit.get('functions', []):
            address, length = int(function.get('address', 0)), int(function['size'])
            if address < start + size and start < address + length:
                if (address, length, function['name']) != (start, size, data['name']):
                    raise ValueError('embedded data partially overlaps a function')
                removed.append(function)
    if len({f['name'] for f in removed}) != len(removed):
        raise ValueError('duplicate embedded data ranges')
    measures = unit['measures']
    code = sum(int(f['size']) for f in removed)
    matched = [f for f in removed if f.get('fuzzy_match_percent') == 100]
    changes = {'total_code': -code,
               'matched_code': -sum(int(f['size']) for f in matched),
               'complete_code': -code if unit['metadata'].get('complete') else 0,
               'total_functions': -len(removed), 'matched_functions': -len(matched),
               'total_data': sum(int(d['size']) for d in ranges),
               'matched_data': sum(sum(a == b for a, b in zip(
                   base_text[int(d['address']):int(d['address']) + int(d['size'])],
                   retail_text[int(d['address']):int(d['address']) + int(d['size'])])) for d in ranges)}
    for key, change in changes.items():
        value = int(measures.get(key, 0)) + change
        if value < 0:
            raise ValueError('data reclassification would make a negative measure')
        measures[key] = str(value) if key in BYTE_KEYS else value
    unit['functions'] = [f for f in unit.get('functions', []) if f not in removed]
    refresh_percentages(measures)


def sum_measures(measures, units):
    for key in BYTE_KEYS + COUNT_KEYS:
        total = sum(int(u['measures'].get(key, 0)) for u in units)
        measures[key] = str(total) if key in BYTE_KEYS else total
    refresh_percentages(measures)


def classify(version):
    build = ROOT / 'build' / version
    manifest = json.loads((ROOT / 'configs/embedded_data.json').read_text())
    placement = parse_map_placement((build / 'main.map').read_text())
    retail = (ROOT / 'assets' / version / 'main.exe').read_bytes()
    report = json.loads((build / 'report.raw.json').read_text())
    log = []
    for unit in report['units']:
        source = unit['metadata']['source_path']
        if source not in manifest:
            continue
        ranges, text = checked_data_ranges(build / (source + '.o'), manifest[source])
        target_ranges, target_text = checked_data_ranges(
            ROOT / 'expected' / version / 'build' / (source + '.o'), manifest[source])
        if ranges != target_ranges:
            raise ValueError('base and target data ranges differ')
        address, size = placement['build/%s/%s.o' % (version, source)]['.text']
        offset = address - 0x80010000 + 0x800
        reference = retail[offset:offset + size]
        for data in ranges:
            start, length = int(data['address']), int(data['size'])
            if target_text[start:start + length] != reference[start:start + length]:
                raise ValueError('target embedded data differs from retail')
        move_data(unit, ranges, text, reference)
        log.append({'source_path': source, 'data': ranges})
    sum_measures(report['measures'], report['units'])
    for category in report.get('categories', []):
        sum_measures(category['measures'], [u for u in report['units']
                     if category['id'] in u['metadata']['progress_categories']])
    (build / 'report.json').write_text(json.dumps(report) + '\n')
    (build / 'report.embedded-data.json').write_text(json.dumps(log, indent=2) + '\n')
    print('embedded data: %d bytes reclassified; raw report retained' %
          sum(int(d['size']) for entry in log for d in entry['data']))


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--version', default='PAL')
    classify(parser.parse_args().version)
