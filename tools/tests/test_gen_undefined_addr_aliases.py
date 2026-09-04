import tempfile
import unittest
from pathlib import Path

from tools.scripts.gen_undefined_addr_aliases import assigned_dependencies


class LinkerScriptDependencyTest(unittest.TestCase):
    def test_reads_symbols_used_only_by_assignment_expressions(self):
        with tempfile.TemporaryDirectory() as temp:
            script = Path(temp) / "manual.txt"
            script.write_text(
                "table_hi = jtbl_8007D294_main >> 16;\n"
                "table_lo = jtbl_8007D294_main & 0xFFFF;\n"
            )
            self.assertEqual(
                assigned_dependencies([script]),
                {"jtbl_8007D294_main"},
            )

    def test_ignores_comments_and_assignment_left_sides(self):
        with tempfile.TemporaryDirectory() as temp:
            script = Path(temp) / "manual.txt"
            script.write_text(
                "/* fake = ignored_symbol; */\n"
                "result = real_symbol + 4; // ignored_tail\n"
            )
            self.assertEqual(assigned_dependencies([script]), {"real_symbol"})


if __name__ == "__main__":
    unittest.main()
