#!/usr/bin/env python3
"""Build the target-side objects that objdiff compares this tree against.

objdiff needs two objects per translation unit. The base side is what `make
build` compiles from C. The target side has to come from the game itself - if
it came from our own source the comparison would be a mirror and would prove
nothing. This produces the target side by running splat a second time against
an empty source tree, so every function comes back as disassembly of the
original EXE, then compiling those stubs with the same compiler.

Symbol names preferentially come from the verified build's own object files.
configs/<version>/sym.main.txt has fallen behind the source: it may still call
func_80013F48 what the C now calls GameInitPad, and objdiff pairs symbols by
name. The regional symbol files still supply labels not exported by an object,
but only at names and addresses the verified build did not already occupy.

Only the names are borrowed. Every byte in the result is disassembled from
assets/<version>/main.exe.
"""

import argparse
import concurrent.futures
import os
import pathlib
import re
import shutil
import subprocess
import sys

import yaml
from elftools.elf.elffile import ELFFile

ROOT = pathlib.Path(__file__).resolve().parents[2]

# Only these reach the image; everything else in an object is debug scaffolding.
SECTIONS = ('.text', '.data', '.rodata', '.bss')

# gcc -gcoff litters every object with local line-number labels. They are not
# code, they never appear in the disassembly, and naming them would only teach
# splat thousands of bogus symbols. The .NON_MATCHING aliases are asm-differ's,
# emitted by include/macro.inc at the same address as the symbol they shadow,
# which splat rejects outright as a duplicate.
GCC_LOCAL = re.compile(r'(?:^(?:LM\d+|\$L|__gnu_compiled|gcc2_compiled|_MACRO_INC_GUARD))'
                       r'|(?:\.NON_MATCHING$)')

# splat writes the legacy three-argument form; this tree's macro takes the asm
# folder and the symbol, so the stubs need rewriting before they will compile.
INCLUDE_CALL = re.compile(r'INCLUDE_(ASM|RODATA)\(\s*[^,]+,\s*"([^"]+)"\s*,\s*(\w+)\s*\)')
CURRENT_INCLUDE_CALL = re.compile(
    r'INCLUDE_(?:ASM|RODATA)\(\s*"[^"]+"\s*,\s*(\w+)\s*\)')

MAP_PLACEMENT = re.compile(
    r'^\s(\.\w+)\s+0x([0-9a-f]+)\s+0x([0-9a-f]+)\s+(build/\S+\.o)$', re.MULTILINE)


def parse_map_placement(text):
    """Where the linker put each input object's sections: {object: {section: (addr, size)}}."""
    out = {}
    for section, addr, size, obj in MAP_PLACEMENT.findall(text):
        out.setdefault(obj, {})[section] = (int(addr, 16), int(size, 16))
    return out


def object_symbols(elf, placement):
    """Absolute (name, address, kind, size) for every symbol worth naming in one object.

    Offsets in an object file are section-relative, so each one is biased by
    where the linker placed that section. A symbol in a section the map does
    not mention never reached the image and is not worth naming.

    `kind` is the ELF symbol type, which decides more than it looks like it
    should: objdiff sorts symbols into code and data and refuses to pair one
    with the other, so a block this tree spells as a const array in .text has
    to stay data on the target side as well.
    """
    out = []
    table = elf.get_section_by_name('.symtab')
    if table is None:
        return out
    sections = {i: elf.get_section(i).name for i in range(elf.num_sections())}
    for symbol in table.iter_symbols():
        info = symbol['st_info']
        if info['type'] not in ('STT_FUNC', 'STT_OBJECT', 'STT_NOTYPE'):
            continue
        section = sections.get(symbol['st_shndx']) if isinstance(symbol['st_shndx'], int) else None
        name = symbol.name
        if section not in SECTIONS or not name or GCC_LOCAL.search(name):
            continue
        based = placement.get(section)
        if based is None:
            continue
        out.append((name, based[0] + symbol['st_value'], info['type'],
                    symbol['st_size'], section))
    return out


def unambiguous(symbols):
    """Drop any name the build uses at more than one address.

    A `static` helper can share its name with a different one next door. Both
    are correct locally, but a splat symbol file is flat, so feeding it the
    name twice would attach it to whichever address it read last. Leaving those
    few as func_<address> costs one unpaired symbol; guessing costs a wrong one.
    """
    seen = {}
    for symbol in symbols:
        seen.setdefault(symbol[0], set()).add(symbol[1])
    return [s for s in symbols if len(seen[s[0]]) == 1]


def symbol_file(symbols, segment=None):
    """The symbols as a splat symbol_addrs file, sorted so the diff stays readable.

    Only a symbol the compiler typed as a function is declared one here. The
    handwritten kernel stubs are const arrays placed in .text, and calling them
    functions would put them on the code side of objdiff's ledger while the
    base keeps them on the data side, which leaves the pair unresolvable.
    """
    lines = ['// Generated by tools/scripts/gen_expected.py from a build that',
             '// passed `make check`. Do not edit; do not commit.']
    for name, addr, kind, size, _ in sorted(unambiguous(symbols), key=lambda s: (s[1], s[0])):
        notes = []
        if kind == 'STT_FUNC':
            notes.append('type:func')
        if size:
            notes.append('size:0x%X' % size)
        if segment:
            notes.append('segment:%s' % segment)
        suffix = ' // %s' % ' '.join(notes) if notes else ''
        lines.append('%s = 0x%08X;%s' % (name, addr, suffix))
    return '\n'.join(lines) + '\n'


CONFIG_SYMBOL = re.compile(r'^([A-Za-z_.$][\w.$]*)\s*=\s*(0x[0-9A-Fa-f]+);')


def configured_function_addresses(configured_texts):
    addresses = set()
    for text in configured_texts:
        for line in text.splitlines():
            match = CONFIG_SYMBOL.match(line.strip())
            if match is not None and 'type:func' in line:
                addresses.add(int(match.group(2), 16))
    return addresses


def c_subsegment_addresses(config):
    """Addresses splat must regard as functions to create each C stub."""
    addresses = set()
    for segment in config.get('segments', []):
        if not isinstance(segment, dict) or segment.get('type', 'code') != 'code':
            continue
        vrom = segment.get('start')
        vram = segment.get('vram')
        if vrom is None or vram is None:
            continue
        for subsegment in segment.get('subsegments', []):
            if (isinstance(subsegment, list) and len(subsegment) >= 2
                    and subsegment[1] in ('c', '.c')):
                addresses.add(vram + subsegment[0] - vrom)
    return addresses


def merged_symbol_file(symbols, configured_texts, forced_function_addresses=(), segment=None):
    """Build symbols plus only the non-conflicting configured fallbacks.

    Some assembly objects do not export every function boundary needed by
    splat. The checked-in regional tables retain those boundaries, but cannot
    be passed alongside the generated table wholesale: splat rejects duplicate
    definitions, and an old generic name can mask a newer source name at the
    same address.
    """
    function_addresses = (configured_function_addresses(configured_texts)
                          | set(forced_function_addresses))
    symbols = [
        (name, address, 'STT_FUNC' if address in function_addresses else kind,
         size, section)
        for name, address, kind, size, section in unambiguous(symbols)
    ]
    occupied_names = {symbol[0] for symbol in symbols}
    occupied_addresses = {symbol[1] for symbol in symbols}
    fallbacks = []
    for text in configured_texts:
        for line in text.splitlines():
            match = CONFIG_SYMBOL.match(line.strip())
            if match is None:
                continue
            name, address_text = match.groups()
            address = int(address_text, 16)
            if name in occupied_names or address in occupied_addresses:
                continue
            fallback = line.strip()
            if segment:
                fallback += (' segment:%s' if '//' in fallback else ' // segment:%s') % segment
            fallbacks.append(fallback)
            occupied_names.add(name)
            occupied_addresses.add(address)
    generated = symbol_file(symbols, segment).rstrip()
    if fallbacks:
        generated += ('\n// Fallback labels not exported by the verified build.\n'
                      + '\n'.join(fallbacks))
    return generated + '\n'


DIFFER_ALIAS = re.compile(r'^nonmatching\s.*$\n?', re.MULTILINE)
CODE_LABEL = re.compile(r'^(glabel|endlabel) (\w+)$', re.MULTILINE)


def strip_differ_aliases(text):
    """Remove the `nonmatching` lines splat writes above each disassembled function.

    The macro declares a second symbol at the function's own address, for
    asm-differ's benefit. A C object never has one, so leaving them in gives
    objdiff two target symbols competing for one base symbol - which it reports
    as `Failed to find right side symbol for paired left side symbol` and
    refuses to score the unit at all.
    """
    return DIFFER_ALIAS.sub('', text)


def retype_data_in_text(text, kinds):
    """Relabel blocks the base does not type as functions but splat does.

    A handwritten kernel stub is a const array placed in .text. gcc leaves it
    untyped; splat has no way to say "data inside a code segment" and writes
    glabel, which types it as a function. objdiff keeps functions and data in
    separate ledgers and cannot pair one against the other, so a single such
    symbol makes the whole unit unscorable. Only the symbol's type changes
    here - the instruction words assemble byte for byte the same.
    """
    def relabel(match):
        macro, name = match.groups()
        kind = kinds.get(name)
        if kind is None:
            return match.group(0)
        if kind == 'STT_OBJECT':
            return '%s %s' % ('dlabel' if macro == 'glabel' else 'enddlabel', name)
        # Untyped in the base, so emit the label with no .type and no .size at
        # all; anything else would put a type on it that gcc never wrote.
        return '.global %s\n%s:' % (name, name) if macro == 'glabel' else ''

    return CODE_LABEL.sub(relabel, text)


# Both sides must be identifiers. The same file also binds names to raw
# addresses - `g_ScratchRenderMode = 0x1F800084` - and reading those as aliases
# would rewrite every matching literal in the disassembly into a symbol the
# assembler cannot resolve until link time.
ALIAS_ASSIGNMENT = re.compile(r'^([A-Za-z_]\w*) = ([A-Za-z_]\w*);\s*$', re.MULTILINE)
# An instruction is indented; a label macro and a directive start at column
# zero. Renaming a definition rather than a reference would move the symbol.
OPERAND_LINE = re.compile(r'^\s+\S')


def alias_renames(manual_syms):
    """{defined name: the alias the C reaches it by}, from the manual linker script.

    A handwritten block is defined under one name and called under another -
    `ChangeClearRCnt = ChangeClearRCntStub`. The C calls the alias, so the base
    object carries an undefined reference to it, while the disassembly calls
    the definition next door and carries none. objdiff pairs the two ends of a
    relocation, and an undefined symbol belongs to no section, so that single
    difference makes the unit unscorable.
    """
    return {target: alias for alias, target in ALIAS_ASSIGNMENT.findall(manual_syms)}


def apply_alias_renames(text, renames):
    """Call aliased blocks by the name the C uses, leaving their definitions alone."""
    out = []
    for line in text.splitlines(keepends=True):
        if OPERAND_LINE.match(line):
            for target, alias in renames.items():
                line = re.sub(r'\b%s\b' % re.escape(target), alias, line)
        out.append(line)
    return ''.join(out)


AUTO_SYMBOL = re.compile(r'^(D_[0-9A-Fa-f]+) = 0x([0-9A-Fa-f]+);', re.MULTILINE)
HILO = re.compile(r'%(hi|lo)\((D_[0-9A-Fa-f]+)\)')


def invented_constants(auto_syms, vram_start):
    """{name: value} for addresses splat named that are not addresses at all.

    A `lui`/`addiu` pair loading a plain number looks exactly like one loading
    an address, and the disassembler has to guess. Anything below where the
    executable is loaded cannot be a pointer into it, so the guess was wrong:
    the compiler wrote a constant and this has to say so too, or the pair reads
    as a relocation the base does not have.
    """
    return {name: int(value, 16) for name, value in AUTO_SYMBOL.findall(auto_syms)
            if int(value, 16) < vram_start}


def halves(value):
    """The lui/addiu pair for a constant, the way the assembler splits one."""
    low = value & 0xFFFF
    if low >= 0x8000:
        low -= 0x10000
    return (value - low) >> 16, low


def inline_constant_pairs(text, constants):
    def replace(match):
        part, name = match.groups()
        if name not in constants:
            return match.group(0)
        high, low = halves(constants[name])
        return '0x%X' % high if part == 'hi' else '%d' % low

    return HILO.sub(replace, text)


C_FUNCTION = re.compile(r'^[A-Za-z_][\w \t\*]*?\b[A-Za-z_]\w*\s*\([^;{]*\)\s*\{', re.MULTILINE)


def is_all_asm(text):
    """True when a source file defines no C at all, only assembly it includes."""
    return ('INCLUDE_ASM' in text or 'HANDWRITTEN_ASM' in text) and not C_FUNCTION.search(text)


def is_data_only(text):
    """True when a disassembled file holds constants rather than a function.

    Only these may be pulled in with INCLUDE_RODATA. A function dragged in that
    way lands in .rodata, where glabel's `.size label, . - label` straddles a
    section boundary and the assembler rejects it - and splat sometimes writes
    a trivial function as real C in the stub too, so including its assembly
    would define the symbol twice.
    """
    return 'glabel' not in text


def rewrite_stub(text, asm_root, unit, unit_asm_names):
    """Turn splat's stub into something this tree's INCLUDE_ASM macro accepts.

    Any constant file in the unit's directory that no INCLUDE_ASM mentions is
    rodata splat could not attach to a function. It still belongs in the
    object, so it gets pulled in explicitly - otherwise the target would be
    missing data the base has, and the unit would score below what it deserves.
    """
    # Regional configs already ask splat for this tree's current two-argument
    # macro form. Count those before adding unattached rodata; otherwise every
    # already-emitted jump table is included a second time.
    referenced = {match.group(1) for match in CURRENT_INCLUDE_CALL.finditer(text)}

    def replace(match):
        kind, folder, name = match.groups()
        referenced.add(name)
        return 'INCLUDE_%s("%s/%s", %s)' % (kind, asm_root, folder, name)

    out = INCLUDE_CALL.sub(replace, text)
    orphans = sorted(name for name in unit_asm_names if name not in referenced)
    if orphans:
        out += '\n' + '\n'.join(
            'INCLUDE_RODATA("%s/%s", %s);' % (asm_root, unit, name) for name in orphans) + '\n'
    return out


def splat_config(base_config, out_dir, extra_symbols):
    """The split config again, pointed at an empty source tree.

    src_path decides whether splat finds a C file for a subsegment. Aiming it
    somewhere empty is what makes splat emit every function as assembly instead
    of only the handful this tree has not decompiled yet. migrate_rodata is on
    so the constants a function references travel with it; left off, the 2.7 KB
    of per-unit rodata would land nowhere.

    Every path mirrors the real build one level down, so an object's path under
    expected/<version>/build is the same string as under build/<version>. The
    objdiff config and the coverage check both lean on that.
    """
    config = yaml.safe_load(base_config)
    options = config['options']
    options['asm_path'] = '%s/asm/%s' % (out_dir, options['asm_path'].split('/', 1)[1])
    options['src_path'] = '%s/src/main' % out_dir
    options['build_path'] = '%s/build' % out_dir
    options['ld_script_path'] = '%s/main.ld' % out_dir
    options['undefined_syms_auto_path'] = '%s/undefined_syms_auto.txt' % out_dir
    options['undefined_funcs_auto_path'] = '%s/undefined_funcs_auto.txt' % out_dir
    options['migrate_rodata_to_functions'] = True
    # The verified build is the authoritative, current symbol inventory. Do
    # not also retain the retail split's symbol files: most names occur in
    # both, and current splat rejects those duplicate definitions before it
    # can produce the target objects.
    options['symbol_addrs_path'] = [extra_symbols]
    return yaml.safe_dump(config, sort_keys=False)


def run(cmd, **kwargs):
    done = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True, errors='replace', **kwargs)
    if done.returncode != 0:
        sys.stderr.write(done.stdout + done.stderr)
        raise SystemExit('failed: %s' % ' '.join(str(c) for c in cmd))
    return done.stdout


def collect_symbols(placement):
    symbols = []
    for obj, sections in sorted(placement.items()):
        path = ROOT / obj
        if not path.exists():
            continue
        with path.open('rb') as handle:
            symbols.extend(object_symbols(ELFFile(handle), sections))
    return symbols


def compile_units(stubs, jobs):
    """cc.sh per stub, in parallel. Returns the units that failed."""
    def build(pair):
        stub, obj = pair
        obj.parent.mkdir(parents=True, exist_ok=True)
        done = subprocess.run(['tools/scripts/cc.sh', str(stub.relative_to(ROOT)),
                               str(obj.relative_to(ROOT))],
                              cwd=ROOT, capture_output=True, text=True, errors='replace')
        return (stub, done.stderr) if done.returncode != 0 else None

    with concurrent.futures.ThreadPoolExecutor(max_workers=jobs) as pool:
        return [bad for bad in pool.map(build, stubs) if bad is not None]


def check_coverage(placement, out_dir, version, readelf):
    """Every unit's target sections must be the size the linker gave the base.

    objdiff will happily report 100% on a unit whose target lost half its
    functions, because it only scores what both sides have. Comparing against
    the map catches that: the map is derived from the EXE this build reproduces
    byte for byte, so a size that disagrees means the target is incomplete.
    """
    problems = []
    for obj, sections in sorted(placement.items()):
        if '/src/' not in obj:
            continue
        target = ROOT / out_dir / 'build' / obj.split('build/%s/' % version, 1)[1]
        if not target.exists():
            problems.append((obj, 'target object missing'))
            continue
        sizes = {}
        for line in run([readelf, '-S', '-W', str(target)]).splitlines():
            m = re.search(r'\[\s*\d+\]\s+(\.\w+)\s+\w+\s+[0-9a-f]+\s+[0-9a-f]+\s+([0-9a-f]+)', line)
            if m:
                sizes[m.group(1)] = int(m.group(2), 16)
        for section in ('.text', '.rodata', '.data'):
            want = sections.get(section, (0, 0))[1]
            got = sizes.get(section, 0)
            if want != got:
                problems.append((obj, '%s: base 0x%X, target 0x%X' % (section, want, got)))
    return problems


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--version', default='PAL')
    parser.add_argument('--basename', default='main')
    parser.add_argument('--out', default='expected')
    parser.add_argument('--jobs', type=int, default=os.cpu_count() or 4)
    parser.add_argument('--python', default=sys.executable)
    parser.add_argument('--readelf', default='mipsel-none-elf-readelf')
    parser.add_argument('--objcopy', default='mipsel-none-elf-objcopy')
    # Where the executable is loaded. Anything the disassembler names below
    # this cannot be a pointer into the image.
    parser.add_argument('--vram-start', type=lambda v: int(v, 0), default=0x80010000)
    parser.add_argument('--as', dest='assembler', default='mipsel-none-elf-as')
    args = parser.parse_args(argv)

    version, basename = args.version, args.basename
    out_dir = '%s/%s' % (args.out, version)
    build_dir = ROOT / 'build' / version
    map_path = build_dir / ('%s.map' % basename)
    if not map_path.exists():
        raise SystemExit('%s missing - run `make build check` first' % map_path)

    placement = parse_map_placement(map_path.read_text())
    print('map: %d objects' % len(placement))

    shutil.rmtree(ROOT / out_dir, ignore_errors=True)
    (ROOT / out_dir).mkdir(parents=True)

    symbols = collect_symbols(placement)
    base_config = (ROOT / 'configs' / version / ('%s.yaml' % basename)).read_text()
    base_document = yaml.safe_load(base_config)
    base_options = base_document['options']
    configured_paths = base_options.get('symbol_addrs_path', [])
    if isinstance(configured_paths, str):
        configured_paths = [configured_paths]
    configured_texts = [(ROOT / path).read_text() for path in configured_paths]
    sym_path = '%s/sym.from_build.txt' % out_dir
    (ROOT / sym_path).write_text(merged_symbol_file(
        symbols, configured_texts, c_subsegment_addresses(base_document), basename))
    print('symbols: %d taken from the verified build' % len(symbols))

    config_path = ROOT / out_dir / 'splat.yaml'
    config_path.write_text(splat_config(base_config, out_dir, sym_path))
    run([args.python, '-m', 'splat', 'split', str(config_path.relative_to(ROOT))])

    asm_root = '%s/asm/%s/%s/nonmatchings' % (out_dir, version, basename)
    # Only the per-function disassembly is sanitised. The data blobs are
    # assembled from the same files on both sides, so they have to stay
    # byte-for-byte what the real build feeds its assembler.
    in_text = {name: kind for name, _, kind, _, section in symbols
               if section == '.text' and kind != 'STT_FUNC'}
    renames = alias_renames(
        (ROOT / 'linkers' / version / 'undefined_syms_manual.txt').read_text())
    constants = invented_constants(
        (ROOT / out_dir / 'undefined_syms_auto.txt').read_text(), args.vram_start)
    if constants:
        print('constants: %d address(es) splat named that are plain numbers' % len(constants))
    for source in (ROOT / asm_root).rglob('*.s'):
        text = strip_differ_aliases(source.read_text())
        text = apply_alias_renames(retype_data_in_text(text, in_text), renames)
        source.write_text(inline_constant_pairs(text, constants))

    # The constant blobs hold no code and were never decompiled: both sides
    # disassemble the same bytes out of the same EXE. `make split` then runs
    # symbolise_data_words.py over the tree's copy, which this pass does not,
    # so leaving them separate makes two spellings of one thing and nothing
    # pairs. Take the tree's copy, exactly as the base assembles it.
    blobs = 0
    for source in (ROOT / out_dir / 'asm').rglob('*.s'):
        if 'nonmatchings' in source.parts:
            continue
        original = ROOT / 'asm' / source.relative_to(ROOT / out_dir / 'asm')
        if original.exists():
            shutil.copyfile(original, source)
            blobs += 1
    print('data: %d blob(s) taken from the tree' % blobs)


    src_root = ROOT / out_dir / 'src' / basename
    real_root = ROOT / 'src' / basename
    stubs = []
    verbatim = 0
    for stub in sorted(src_root.rglob('*.c')):
        unit = str(stub.relative_to(src_root)).removesuffix('.c')
        real = real_root / ('%s.c' % unit)
        if real.exists() and is_all_asm(real.read_text()):
            # Nothing here was decompiled: the unit is one hand-written block
            # the tree keeps as assembly on purpose, and the base object is
            # built from that file. Splitting it again into one file per label
            # only invites the two disassemblies to disagree over which
            # addresses deserve a name. Use the same input for both sides.
            shutil.copyfile(real, stub)
            verbatim += 1
        else:
            unit_dir = ROOT / asm_root / unit
            names = ({p.stem for p in unit_dir.glob('*.s') if is_data_only(p.read_text())}
                     if unit_dir.exists() else set())
            stub.write_text(rewrite_stub(stub.read_text(), asm_root, unit, names))
        stubs.append((stub, ROOT / out_dir / 'build' /
                      stub.relative_to(ROOT / out_dir).with_suffix('.c.o')))
    print('units: %d stubs, %d taken verbatim (nothing in them is C)' % (len(stubs), verbatim))

    # The INCLUDE_ASM macro wraps each block in a throwaway function so the
    # compiler will carry the .include through. maspsx deletes the body but
    # leaves the name behind as an undefined reference, which the real objects
    # never have; left in, objdiff pairs one against a real function and gives
    # up on the unit.
    failures = compile_units(stubs, args.jobs)
    for stub, err in failures:
        sys.stderr.write('%s:\n%s\n' % (stub, err))
    if failures:
        raise SystemExit('%d unit(s) failed to compile' % len(failures))
    for _, obj in stubs:
        run([args.objcopy, '--wildcard', '--strip-symbol', '__maspsx_include_asm_hack_*',
             str(obj.relative_to(ROOT))])

    for source in sorted((ROOT / out_dir / 'asm').rglob('*.s')):
        if 'nonmatchings' in source.parts:
            continue
        obj = (ROOT / out_dir / 'build' /
               source.relative_to(ROOT / out_dir)).with_suffix('.s.o')
        obj.parent.mkdir(parents=True, exist_ok=True)
        run([args.assembler, '-EL', '-G0', '-march=r3000', '-mtune=r3000',
             '-no-pad-sections', '-Iinclude',
             '-I%s/asm/%s/%s' % (out_dir, version, basename),
             '-o', str(obj.relative_to(ROOT)), str(source.relative_to(ROOT))])

    problems = check_coverage(placement, out_dir, version, args.readelf)
    for obj, why in problems:
        sys.stderr.write('%s: %s\n' % (obj, why))
    if problems:
        raise SystemExit('%d unit(s) do not cover the original' % len(problems))
    print('coverage: every unit matches the size the linker gave it')
    return 0


if __name__ == '__main__':
    sys.exit(main())
