import importlib.util
import unittest
from pathlib import Path


SCRIPT = Path(__file__).resolve().parents[1] / "scripts" / "try_drop_raw.py"
SPEC = importlib.util.spec_from_file_location("try_drop_raw", SCRIPT)
try_drop_raw = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(try_drop_raw)


class RawAccessPatternTests(unittest.TestCase):
    def test_matches_member_and_index_accesses(self):
        source = "RAW(record->field) + RAW(rows[1].fields.x)"
        expressions = [
            match.group("expression")
            for match in try_drop_raw.RAW_ACCESS.finditer(source)
        ]
        self.assertEqual(expressions, ["record->field", "rows[1].fields.x"])

    def test_does_not_match_other_macros(self):
        self.assertIsNone(try_drop_raw.RAW_ACCESS.search("FIELD(record, value)"))


if __name__ == "__main__":
    unittest.main()
