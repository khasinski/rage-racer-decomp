import unittest

import yaml

from tools.scripts.gen_expected import (
    alias_renames,
    apply_alias_renames,
    c_subsegment_addresses,
    halves,
    inline_constant_pairs,
    invented_constants,
    is_all_asm,
    is_data_only,
    merged_symbol_file,
    parse_map_placement,
    retype_data_in_text,
    rewrite_stub,
    splat_config,
    strip_differ_aliases,
    symbol_file,
    unambiguous,
)

MAP = """
 .text          0x80013f48      0x654 build/PAL/src/main/PAL/main/pad/init_pad.c.o
 .rodata        0x80011404       0x14 build/PAL/src/main/PAL/main/menu/title_screen.c.o
 .debug_line    0x00000000      0x117 build/PAL/src/main/PAL/main/pad/init_pad.c.o
LOAD build/PAL/src/main/PAL/main/pad/init_pad.c.o
"""


class ParseMapPlacementTest(unittest.TestCase):
    def test_collects_address_and_size_per_section(self):
        placement = parse_map_placement(MAP)
        self.assertEqual(
            placement["build/PAL/src/main/PAL/main/pad/init_pad.c.o"][".text"],
            (0x80013F48, 0x654),
        )
        self.assertEqual(
            placement["build/PAL/src/main/PAL/main/menu/title_screen.c.o"][".rodata"],
            (0x80011404, 0x14),
        )

    def test_keeps_debug_sections_apart_from_the_image(self):
        # They are in the map but never placed in the image, so they must not
        # be mistaken for a section a symbol can be biased against.
        placement = parse_map_placement(MAP)
        init_pad = placement["build/PAL/src/main/PAL/main/pad/init_pad.c.o"]
        self.assertEqual(init_pad[".debug_line"], (0, 0x117))

    def test_ignores_lines_that_are_not_a_placement(self):
        self.assertNotIn("LOAD", "".join(parse_map_placement(MAP)))


class UnambiguousTest(unittest.TestCase):
    def test_drops_a_name_used_at_two_addresses(self):
        symbols = [
            ("helper", 0x100, "STT_FUNC", 4, ".text"),
            ("helper", 0x200, "STT_FUNC", 4, ".text"),
            ("only", 0x300, "STT_FUNC", 4, ".text"),
        ]
        self.assertEqual([s[0] for s in unambiguous(symbols)], ["only"])

    def test_keeps_a_name_repeated_at_one_address(self):
        symbols = [
            ("alias", 0x100, "STT_FUNC", 4, ".text"),
            ("alias", 0x100, "STT_FUNC", 4, ".text"),
        ]
        self.assertEqual(len(unambiguous(symbols)), 2)


class SymbolFileTest(unittest.TestCase):
    def test_types_only_what_the_compiler_called_a_function(self):
        written = symbol_file([
            ("GameInitPad", 0x80013F48, "STT_FUNC", 0x38, ".text"),
            ("BiosExit", 0x80063D9C, "STT_NOTYPE", 0, ".text"),
        ])
        self.assertIn("GameInitPad = 0x80013F48; // type:func size:0x38", written)
        self.assertIn("BiosExit = 0x80063D9C;", written)
        self.assertNotIn("BiosExit = 0x80063D9C; // type:func", written)

    def test_sorts_by_address(self):
        written = symbol_file([
            ("second", 0x200, "STT_FUNC", 4, ".text"),
            ("first", 0x100, "STT_FUNC", 4, ".text"),
        ])
        self.assertLess(written.index("first"), written.index("second"))

    def test_can_assign_generated_symbols_to_the_executable_segment(self):
        written = symbol_file(
            [("Fn", 0x100, "STT_FUNC", 4, ".text")], segment="main")
        self.assertIn("type:func size:0x4 segment:main", written)

    def test_configured_symbols_only_fill_unoccupied_names_and_addresses(self):
        symbols = [("CurrentName", 0x100, "STT_FUNC", 4, ".text")]
        configured = """\
OldName = 0x00000100; // type:func
CurrentName = 0x00000200; // type:func
MissingBoundary = 0x00000300; // type:func
"""
        written = merged_symbol_file(symbols, [configured])
        self.assertIn("CurrentName = 0x00000100;", written)
        self.assertIn("MissingBoundary = 0x00000300; // type:func", written)
        self.assertNotIn("OldName", written)
        self.assertNotIn("0x00000200", written)

    def test_configured_function_type_upgrades_the_current_name(self):
        symbols = [("CurrentName", 0x100, "STT_NOTYPE", 0, ".text")]
        written = merged_symbol_file(
            symbols, ["OldName = 0x00000100; // type:func\n"])
        self.assertIn("CurrentName = 0x00000100; // type:func", written)
        self.assertNotIn("OldName", written)

    def test_c_subsegment_start_is_forced_to_a_function_for_splat(self):
        symbols = [("EmbeddedAsm", 0x80010100, "STT_NOTYPE", 0, ".text")]
        written = merged_symbol_file(symbols, [], {0x80010100})
        self.assertIn("EmbeddedAsm = 0x80010100; // type:func", written)


class CSubsegmentAddressesTest(unittest.TestCase):
    def test_converts_rom_offsets_to_vram(self):
        config = {
            "segments": [{
                "type": "code", "start": 0x800, "vram": 0x80010000,
                "subsegments": [[0x900, "c", "first"], [0xA00, "rodata", "data"]],
            }]
        }
        self.assertEqual(c_subsegment_addresses(config), {0x80010100})


class SplatConfigTest(unittest.TestCase):
    def test_replaces_stale_symbol_files_with_verified_build_symbols(self):
        base = """
options:
  asm_path: asm/PAL/main
  symbol_addrs_path:
    - configs/PAL/sym.main.txt
    - configs/PAL/sym.bss.main.txt
"""
        config = yaml.safe_load(splat_config(base, "expected/PAL", "expected/PAL/sym.from_build.txt"))
        self.assertEqual(config["options"]["symbol_addrs_path"],
                         ["expected/PAL/sym.from_build.txt"])


class StripDifferAliasesTest(unittest.TestCase):
    def test_removes_the_alias_line_and_nothing_else(self):
        text = "nonmatching Lzc, 0x18\nglabel Lzc\n    jr $ra\n"
        self.assertEqual(strip_differ_aliases(text), "glabel Lzc\n    jr $ra\n")

    def test_leaves_an_instruction_mentioning_the_word_alone(self):
        text = "    addiu $a0, $zero, 0x2 /* nonmatching */\n"
        self.assertEqual(strip_differ_aliases(text), text)


class RetypeDataInTextTest(unittest.TestCase):
    def test_untyped_block_loses_its_function_type_and_size(self):
        text = "glabel func_80069C94\n    nop\nendlabel func_80069C94\n"
        out = retype_data_in_text(text, {"func_80069C94": "STT_NOTYPE"})
        self.assertIn(".global func_80069C94\nfunc_80069C94:", out)
        self.assertNotIn("glabel", out)
        self.assertNotIn("endlabel", out)

    def test_object_block_becomes_a_data_label(self):
        text = "glabel g_Table\n    nop\nendlabel g_Table\n"
        out = retype_data_in_text(text, {"g_Table": "STT_OBJECT"})
        self.assertIn("dlabel g_Table", out)
        self.assertIn("enddlabel g_Table", out)

    def test_a_real_function_is_untouched(self):
        text = "glabel Lzc\n    jr $ra\nendlabel Lzc\n"
        self.assertEqual(retype_data_in_text(text, {"other": "STT_NOTYPE"}), text)


class AliasRenameTest(unittest.TestCase):
    MANUAL = (
        "/* comment */\n"
        "ChangeClearRCnt = ChangeClearRCntStub;\n"
        "func_80028874 = func_80028120_mode_2 + 0x154;\n"
        "g_ScratchRenderMode = 0x1F800084;\n"
    )

    def test_reads_only_plain_assignments(self):
        # An assignment with an offset names a different address, so following
        # it would rewrite a call to point somewhere it never pointed. An
        # address literal is not an alias at all.
        self.assertEqual(alias_renames(self.MANUAL),
                         {"ChangeClearRCntStub": "ChangeClearRCnt"})

    def test_an_address_literal_is_never_rewritten(self):
        # The disassembly spells scratchpad accesses as literals, and the
        # symbol only exists once the linker script is read.
        renames = alias_renames(self.MANUAL)
        line = "    lui $at, (0x1F800084 >> 16)\n"
        self.assertEqual(apply_alias_renames(line, renames), line)

    def test_renames_inside_an_address_annotated_instruction(self):
        # Every line the disassembler writes carries a /* offset word */ prefix.
        text = "    /* 31BD0 800413D0 0C000000 */  jal ChangeClearRCntStub\n"
        out = apply_alias_renames(text, {"ChangeClearRCntStub": "ChangeClearRCnt"})
        self.assertIn("jal ChangeClearRCnt\n", out)

    def test_renames_the_call_but_not_the_definition(self):
        text = ("glabel ChangeClearRCntStub\n"
                "    jal ChangeClearRCntStub\n")
        out = apply_alias_renames(text, {"ChangeClearRCntStub": "ChangeClearRCnt"})
        self.assertIn("glabel ChangeClearRCntStub", out)
        self.assertIn("jal ChangeClearRCnt\n", out)

    def test_does_not_rename_a_longer_name_that_starts_the_same(self):
        out = apply_alias_renames("    jal ChangeClearRCntStubEx\n",
                                  {"ChangeClearRCntStub": "ChangeClearRCnt"})
        self.assertEqual(out, "    jal ChangeClearRCntStubEx\n")


class RewriteStubTest(unittest.TestCase):
    def test_rewrites_to_the_two_argument_macro_with_a_full_path(self):
        text = 'INCLUDE_ASM(const s32, "PAL/main/pad/init_pad", GameInitPad);\n'
        out = rewrite_stub(text, "expected/asm", "PAL/main/pad/init_pad", set())
        self.assertIn('INCLUDE_ASM("expected/asm/PAL/main/pad/init_pad", GameInitPad);', out)

    def test_pulls_in_constants_no_function_claimed(self):
        text = 'INCLUDE_ASM(const s32, "u", Fn);\n'
        out = rewrite_stub(text, "asm", "u", {"Fn", "D_80011870"})
        self.assertIn('INCLUDE_RODATA("asm/u", D_80011870);', out)
        self.assertEqual(out.count("D_80011870"), 1)

    def test_does_not_pull_in_a_function_the_stub_already_includes(self):
        text = 'INCLUDE_ASM(const s32, "u", Fn);\n'
        self.assertNotIn("INCLUDE_RODATA", rewrite_stub(text, "asm", "u", {"Fn"}))

    def test_does_not_duplicate_current_form_rodata(self):
        text = 'INCLUDE_RODATA("asm/u", Table);\n'
        out = rewrite_stub(text, "asm", "u", {"Table"})
        self.assertEqual(out.count("Table"), 1)


class InventedConstantTest(unittest.TestCase):
    AUTO = ("D_7FFFFF = 0x7FFFFF;\n"
            "D_80000004 = 0x80000004;\n"
            "g_MsgInsertController = 0x80010000;\n")

    def test_only_addresses_below_the_image_are_constants(self):
        # 0x80000004 is a real kernel address; 0x7FFFFF is 0x800000 - 1, which
        # the compiler built with lui/addiu and the disassembler mistook for a
        # pointer.
        self.assertEqual(invented_constants(self.AUTO, 0x80000000), {"D_7FFFFF": 0x7FFFFF})

    def test_splits_a_constant_the_way_the_assembler_does(self):
        # The low half is signed, so the high half is carried by one.
        self.assertEqual(halves(0x7FFFFF), (0x80, -1))
        self.assertEqual(halves(0x12340000), (0x1234, 0))
        self.assertEqual(halves(0x00001234), (0, 0x1234))

    def test_rewrites_the_pair_as_literals(self):
        text = ("    lui        $s0, %hi(D_7FFFFF)\n"
                "    addiu      $s0, $s0, %lo(D_7FFFFF)\n")
        out = inline_constant_pairs(text, {"D_7FFFFF": 0x7FFFFF})
        self.assertIn("lui        $s0, 0x80\n", out)
        self.assertIn("addiu      $s0, $s0, -1\n", out)

    def test_leaves_a_real_symbol_alone(self):
        text = "    lui $at, %hi(D_8009E694)\n"
        self.assertEqual(inline_constant_pairs(text, {"D_7FFFFF": 0x7FFFFF}), text)


class IsAllAsmTest(unittest.TestCase):
    def test_a_unit_that_only_includes_assembly(self):
        self.assertTrue(is_all_asm('#include "common.h"\n\nHANDWRITTEN_ASM("a/b", entry);\n'))

    def test_a_unit_that_mixes_c_with_assembly_is_not(self):
        # Copying such a unit's source over the target would make its C compare
        # against itself.
        source = 'INCLUDE_ASM("a/b", Fn);\n\nvoid Other(void) {\n}\n'
        self.assertFalse(is_all_asm(source))

    def test_plain_c_is_not(self):
        self.assertFalse(is_all_asm("void Other(void) {\n}\n"))


class IsDataOnlyTest(unittest.TestCase):
    def test_a_disassembled_function_is_not_data(self):
        self.assertFalse(is_data_only("glabel Fn\n    jr $ra\n"))

    def test_a_constant_block_is_data(self):
        self.assertTrue(is_data_only(".section .rodata\ndlabel D_1\n.word 0\n"))


if __name__ == "__main__":
    unittest.main()
