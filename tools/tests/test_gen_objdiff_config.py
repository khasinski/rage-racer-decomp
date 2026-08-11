import unittest

from tools.scripts.gen_objdiff_config import (
    config,
    excluded,
    is_sdk,
    linked_objects,
    unit_name,
)

MAP = """
 .text          0x80013f48      0x654 build/PAL/src/main/PAL/main/pad/init_pad.c.o
 .rodata        0x80011404       0x14 build/PAL/src/main/PAL/main/pad/init_pad.c.o
 .text          0x80069c94       0x40 build/PAL/src/main/PAL/lib/libgte/leading_zero_count.c.o
 .data          0x8007b664        0x0 build/PAL/asm/PAL/main/data/main/6BE64.data.s.o
 .text          0x00000000        0x0 build/USA/src/main/USA/main/pad/init_pad.c.o
"""


class LinkedObjectsTest(unittest.TestCase):
    def test_lists_each_object_once_in_link_order(self):
        self.assertEqual(linked_objects(MAP, "PAL"), [
            "src/main/PAL/main/pad/init_pad.c.o",
            "src/main/PAL/lib/libgte/leading_zero_count.c.o",
            "asm/PAL/main/data/main/6BE64.data.s.o",
        ])

    def test_ignores_another_version(self):
        self.assertEqual(linked_objects(MAP, "USA"), ["src/main/USA/main/pad/init_pad.c.o"])


class UnitNameTest(unittest.TestCase):
    def test_drops_the_build_scaffolding(self):
        self.assertEqual(unit_name("src/main/PAL/main/pad/init_pad.c.o"), "PAL/main/pad/init_pad")

    def test_names_an_assembled_blob_too(self):
        self.assertEqual(unit_name("asm/PAL/main/data/main/6BE64.data.s.o"),
                         "PAL/main/data/main/6BE64.data")


class ExclusionTest(unittest.TestCase):
    def test_the_psyq_libraries_are_not_the_game(self):
        self.assertTrue(is_sdk("src/main/PAL/lib/libgte/leading_zero_count.c.o"))
        self.assertFalse(is_sdk("src/main/PAL/main/pad/init_pad.c.o"))

    def test_game_code_is_reported(self):
        self.assertIsNone(excluded("src/main/PAL/main/pad/init_pad.c.o", set()))

    def test_each_exclusion_says_why(self):
        self.assertEqual(excluded("src/main/PAL/lib/libgte/lzc.c.o", set()), "PsyQ library")
        self.assertEqual(
            excluded("src/main/PAL/main/render/terrain_submission.c.o",
                     {"PAL/main/render/terrain_submission"}),
            "objdiff cannot pair its symbols")


class ConfigTest(unittest.TestCase):
    def test_target_and_base_mirror_each_other(self):
        written = config(["src/main/PAL/main/pad/init_pad.c.o"], "PAL", "expected")
        unit = written["units"][0]
        self.assertEqual(unit["base_path"], "build/PAL/src/main/PAL/main/pad/init_pad.c.o")
        self.assertEqual(unit["target_path"],
                         "expected/PAL/build/src/main/PAL/main/pad/init_pad.c.o")

    def test_units_are_marked_complete(self):
        # The tree links to the original SHA-1, and `make check` proves it
        # before this ever runs.
        written = config(["src/main/PAL/main/pad/init_pad.c.o"], "PAL", "expected")
        self.assertTrue(written["units"][0]["metadata"]["complete"])

    def test_objdiff_is_told_not_to_build_the_target(self):
        # gen_expected.py owns that side; letting objdiff run make over it
        # would rebuild the base on top of it.
        written = config([], "PAL", "expected")
        self.assertFalse(written["build_target"])


if __name__ == "__main__":
    unittest.main()
