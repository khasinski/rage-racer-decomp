import unittest

from tools.scripts.gen_expected import (
    alias_renames,
    apply_alias_renames,
    is_data_only,
    parse_map_placement,
    retype_data_in_text,
    rewrite_stub,
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


class IsDataOnlyTest(unittest.TestCase):
    def test_a_disassembled_function_is_not_data(self):
        self.assertFalse(is_data_only("glabel Fn\n    jr $ra\n"))

    def test_a_constant_block_is_data(self):
        self.assertTrue(is_data_only(".section .rodata\ndlabel D_1\n.word 0\n"))


if __name__ == "__main__":
    unittest.main()
