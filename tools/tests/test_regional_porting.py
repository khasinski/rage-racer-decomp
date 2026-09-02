import struct
import tempfile
import unittest
from pathlib import Path

from tools.scripts.map_version_functions import LOAD_ADDRESS, normalize
from tools.scripts.port_pal_text_units import (
    infer_exact_address,
    infer_range_start,
    rewrite_main_subsegments,
)


def executable(*words: int) -> bytes:
    return bytes(0x800) + struct.pack(f"<{len(words)}I", *words)


class InstructionNormalizationTest(unittest.TestCase):
    def test_ignores_relocated_immediates(self):
        source = struct.pack("<II", 0x3C088009, 0x25080010)
        target = struct.pack("<II", 0x3C08800A, 0x2508F010)
        self.assertEqual(normalize(source), normalize(target))

    def test_retains_register_structure(self):
        left = struct.pack("<I", 0x01095021)  # addu t2,t0,t1
        right = struct.pack("<I", 0x01285021)  # addu t2,t1,t0
        self.assertNotEqual(normalize(left), normalize(right))


class AddressInferenceTest(unittest.TestCase):
    def test_maps_a_symbol_referenced_with_an_addend(self):
        source = executable(0x3C088009, 0x25080018)
        target = executable(0x3C088009, 0x2508F018)
        self.assertEqual(
            infer_exact_address(
                source, target, 0x800, 0x808, LOAD_ADDRESS, 0x80090010
            ),
            0x8008F010,
        )

    def test_maps_the_start_of_a_relocated_range(self):
        source = executable(0x3C088009, 0x25080024)
        target = executable(0x3C088009, 0x2508F024)
        self.assertEqual(
            infer_range_start(
                source,
                target,
                0x800,
                0x808,
                LOAD_ADDRESS,
                0x80090020,
                0x80090040,
            ),
            0x8008F020,
        )


class ConfigRewriteTest(unittest.TestCase):
    def test_preserves_rodata_and_text_section_kinds(self):
        original = """segments:
  - name: main
    type: code
    subsegments:
      - [0x800, asm, old]
      - [0x2000]
"""
        with tempfile.TemporaryDirectory() as temp:
            config = Path(temp) / "main.yaml"
            config.write_text(original)
            rewrite_main_subsegments(
                config,
                [
                    (0x900, 0x920, ".rodata", "PAL/table"),
                    (0x1100, 0x1120, "c", "PAL/code"),
                ],
                0x2000,
                0x1000,
            )
            result = config.read_text()
        self.assertIn("[0x800, rodata, main/000800_main]", result)
        self.assertIn("[0x920, rodata, main/000920_main]", result)
        self.assertIn("[0x1000, asm, main/001000_main]", result)
        self.assertIn("[0x1100, c, PAL/code]", result)


if __name__ == "__main__":
    unittest.main()
