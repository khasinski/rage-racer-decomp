import json
import re
import struct
import tempfile
import unittest
from pathlib import Path

import yaml

from tools.scripts.map_version_functions import (
    LOAD_ADDRESS,
    normalize,
    read_source_ranges,
    source_path,
)
from tools.scripts.port_pal_text_units import (
    infer_exact_address,
    infer_range_start,
    read_regional_rodata,
    read_regional_units,
    rewrite_main_subsegments,
    validate_text_coverage,
)

ROOT = Path(__file__).resolve().parents[2]


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

    def test_reads_regional_c_units(self):
        with tempfile.TemporaryDirectory() as temp:
            manifest = Path(temp) / "regional_text_units.json"
            manifest.write_text(
                '[{"start":"0x1200","end":"0x1280","path":"JAP10/code"}]'
            )
            self.assertEqual(
                read_regional_units(manifest),
                [(0x1200, 0x1280, "c", "JAP10/code")],
            )

    def test_reads_regional_rodata(self):
        with tempfile.TemporaryDirectory() as temp:
            manifest = Path(temp) / "regional_text_units.json"
            manifest.write_text(
                '[{"start":"0x1200","end":"0x1280","path":"JAP10/code",'
                '"rodata_start":"0x900","rodata_end":"0x924"}]'
            )
            self.assertEqual(
                read_regional_rodata(manifest),
                [(0x900, 0x924, ".rodata", "JAP10/code")],
            )

    def test_validates_contiguous_source_coverage(self):
        segments = [
            (0x1000, 0x1080, "c", "PAL/first"),
            (0x1080, 0x1100, "c", "USA/second"),
        ]
        self.assertEqual(validate_text_coverage(segments, 0x1000), (0x1100, 0x100))

    def test_rejects_a_source_coverage_gap(self):
        segments = [
            (0x1000, 0x1080, "c", "PAL/first"),
            (0x1090, 0x1100, "c", "USA/second"),
        ]
        with self.assertRaisesRegex(ValueError, "coverage gap"):
            validate_text_coverage(segments, 0x1000)


class FunctionSourceRangeTest(unittest.TestCase):
    def test_assigns_functions_to_c_units(self):
        config_text = """segments:
  - [0x800, c, PAL/first]
  - [0x880, asm, gap]
  - [0x900, c, PAL/second]
  - [0x980]
"""
        with tempfile.TemporaryDirectory() as temp:
            config = Path(temp) / "main.yaml"
            config.write_text(config_text)
            ranges = read_source_ranges(config)
        self.assertEqual(source_path(LOAD_ADDRESS + 4, ranges), "PAL/first")
        self.assertIsNone(source_path(LOAD_ADDRESS + 0x84, ranges))
        self.assertEqual(source_path(LOAD_ADDRESS + 0x104, ranges), "PAL/second")


class RegionalSourceLayoutTest(unittest.TestCase):
    def test_regional_sources_do_not_include_pal_implementations(self):
        offenders = []
        for version in ("USA", "JAP10", "JAP11"):
            for path in (ROOT / "src" / "main" / version).rglob("*.c"):
                text = path.read_text()
                if 'PAL/main/' in text or '#include "../../../PAL/' in text:
                    offenders.append(path.relative_to(ROOT).as_posix())
        self.assertEqual(offenders, [])


class RegionalCoverageManifestTest(unittest.TestCase):
    def test_every_regional_text_range_has_source(self):
        asm_segment = re.compile(r"\[0x([0-9A-Fa-f]+),\s*asm,")
        for version in ("USA", "JAP10", "JAP11"):
            with self.subTest(version=version):
                config_dir = ROOT / "configs" / version
                report = json.loads((config_dir / "portable_text.json").read_text())
                text_start = int(report["text_start"], 16)
                text_end = int(report["text_end"], 16)

                self.assertEqual(report["source_coverage_percent"], 100.0)
                self.assertEqual(report["source_text_bytes"], text_end - text_start)
                self.assertEqual(
                    report["source_units"],
                    report["portable_units"] + report["regional_units"],
                )

                regional = json.loads(
                    (config_dir / "regional_text_units.json").read_text()
                )
                for unit in [*report["selected"], *regional]:
                    stem = ROOT / "src" / "main" / unit["path"]
                    sources = [stem.with_suffix(".c"), stem.with_suffix(".s")]
                    self.assertTrue(
                        any(source.is_file() for source in sources),
                        " or ".join(str(source) for source in sources),
                    )

                for match in asm_segment.finditer((config_dir / "main.yaml").read_text()):
                    address = int(match.group(1), 16)
                    self.assertFalse(text_start <= address < text_end)

    def test_sources_do_not_include_other_c_files(self):
        offenders = []
        for version in ("PAL", "USA", "JAP10", "JAP11"):
            source_root = ROOT / "src/main" / version
            for path in source_root.rglob("*.c"):
                if '#include "' in path.read_text() and '.c"' in path.read_text():
                    offenders.append(path.relative_to(ROOT).as_posix())
        self.assertEqual(offenders, [])


class RegionalDataLayoutTest(unittest.TestCase):
    LAYOUTS = {
        "USA": (0x6BB74, 0x8B800, 0x8009B000, 0x801F4338),
        "JAP10": (0x6B6C0, 0x8B000, 0x8009A800, 0x801F3CA4),
        "JAP11": (0x6B7A8, 0x8B000, 0x8009A800, 0x801F3DBC),
    }

    def test_regional_tail_is_data_and_bss_is_accounted_for(self):
        for version, (data_start, image_end, bss_start, bss_end) in self.LAYOUTS.items():
            with self.subTest(version=version):
                document = yaml.safe_load(
                    (ROOT / "configs" / version / "main.yaml").read_text()
                )
                main = next(segment for segment in document["segments"]
                            if segment.get("name") == "main")
                subsegments = main["subsegments"]

                data = next(sub for sub in subsegments
                            if isinstance(sub, list) and sub[0] == data_start)
                self.assertEqual(data[1], "data")

                bss = next(sub for sub in subsegments
                           if isinstance(sub, dict) and sub.get("type") == "bss")
                self.assertEqual(bss["start"], image_end)
                self.assertEqual(bss["vram"], bss_start)
                self.assertEqual(bss["bss_size"], bss_end - bss_start)
                self.assertEqual(main["bss_size"], bss_end - bss_start)


if __name__ == "__main__":
    unittest.main()
