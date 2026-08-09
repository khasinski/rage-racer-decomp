import tempfile
import unittest
from pathlib import Path

from tools.scripts.code_debt import count_debt


class CodeDebtTest(unittest.TestCase):
    def test_counts_each_scaffolding_family(self):
        source = r'''
extern s32 misplaced;
#define GAME_SAMPLE_TYPE s16
#define GAME_SAMPLE_DECL extern s32 sample
void f(u8 *base, void *ptr) {
    register s32 value asm("$4");
    register s32 named asm("v0");
    value = *(s16 *)(base + 0x10);
    value += *(s32 *)(base - 8);
    value += (*(u8 *)(&object->member)) - 1;
    ptr = (void *)((u8 *)ptr + 4);
    value += (s32)ptr;
    value += FIELD32(base, 4);
    asm volatile("");
    value += ({ s32 temporary = 2; temporary; });
    asm(".globl func_80001234\nfunc_80001234 = f + 4");
    value += object->field_20 + object->unk14;
}
'''
        with tempfile.TemporaryDirectory() as directory:
            Path(directory, "sample.c").write_text(source)
            counts = count_debt(Path(directory))

        self.assertEqual(counts["byte_pointer_arithmetic"], 1)
        self.assertEqual(counts["raw_offset_dereferences"], 2)
        self.assertEqual(counts["pointer_integer_casts"], 1)
        self.assertEqual(counts["field_macros"], 1)
        self.assertEqual(counts["register_pins"], 2)
        self.assertEqual(counts["empty_barriers"], 1)
        self.assertEqual(counts["statement_expressions"], 1)
        self.assertEqual(counts["asm_aliases"], 1)
        self.assertEqual(counts["unknown_fields"], 2)
        self.assertEqual(counts["externs_in_c"], 1)
        self.assertEqual(counts["declaration_overrides"], 2)

    def test_ignores_comments(self):
        with tempfile.TemporaryDirectory() as directory:
            Path(directory, "sample.c").write_text(
                "/* extern s32 x; *(s32 *)(p + 0x10); asm(\"alias\"); */\n"
            )
            counts = count_debt(Path(directory))

        self.assertTrue(all(count == 0 for count in counts.values()))


if __name__ == "__main__":
    unittest.main()
