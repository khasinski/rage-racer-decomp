#!/usr/bin/env python3
"""Focused tests for the GCC allocation-dump reconstruction."""

import importlib.util
import sys
import unittest
from pathlib import Path


SCRIPT = Path(__file__).resolve().parents[1] / "scripts" / "alloc_diff.py"
SPEC = importlib.util.spec_from_file_location("alloc_diff", SCRIPT)
assert SPEC is not None and SPEC.loader is not None
alloc_diff = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = alloc_diff
SPEC.loader.exec_module(alloc_diff)


class AllocDiffTests(unittest.TestCase):
    def test_reg_may_share_members_are_grouped_before_priority(self) -> None:
        lreg = """
Register 71 used 5 times across 20 insns; GR_REGS or none.
Register 72 used 7 times across 30 insns; GR_REGS or none.
Register 73 used 6 times across 10 insns; GR_REGS or none.

(insn 1 0 2 (set (reg/v:SI 72)
        (reg/v:SI 71)) 141 {movsi_internal2} (nil))
(insn 2 1 0 (set (reg/v:SI 73)
        (const_int 0)) 141 {movsi_internal2} (nil))
"""
        greg = """
;; 2 regs to allocate: 72 73

;; Register dispositions:
71 in 16  72 in 16  73 in 17

"""
        lreg = ";; Function test\n\n" + lreg
        greg = ";; Function test\n\n" + greg
        order, allocnos, dispositions = alloc_diff.reconstruct_allocnos(lreg, greg, "test")

        self.assertEqual(order, [72, 73])
        self.assertEqual(allocnos[72].members, (71, 72))
        self.assertEqual(allocnos[72].refs, 12)
        self.assertEqual(allocnos[72].live, 30)
        self.assertEqual(allocnos[72].priority, 12000)
        self.assertEqual(allocnos[73].priority, 12000)
        self.assertEqual(allocnos[72].number, 0)
        self.assertEqual(allocnos[73].number, 1)
        self.assertEqual(dispositions[71], dispositions[72])

    def test_priority_multiplies_multiword_size(self) -> None:
        lreg = """
Register 71 used 5 times across 20 insns; GR_REGS or none.
Register 72 used 5 times across 20 insns; GR_REGS or none.

(insn 1 0 2 (set (reg/v:DI 71)
        (const_int 0)) 141 {movdi} (nil))
(insn 2 1 0 (set (reg/v:SI 72)
        (const_int 0)) 141 {movsi} (nil))
"""
        greg = """
;; 2 regs to allocate: 71 72

;; Register dispositions:
71 in 16  72 in 18

"""
        lreg = ";; Function test\n\n" + lreg
        greg = ";; Function test\n\n" + greg
        order, allocnos, _ = alloc_diff.reconstruct_allocnos(lreg, greg, "test")

        self.assertEqual(order, [71, 72])
        self.assertEqual(allocnos[71].size, 2)
        self.assertEqual(allocnos[71].priority, 10000)
        self.assertEqual(allocnos[72].priority, 5000)

    def test_selects_requested_function_and_uses_dumped_byte_size(self) -> None:
        lreg = """
;; Function decoy

Register 99 used 1 times across 1 insns; GR_REGS or none.

;; Function test

Register 71 used 5 times across 20 insns; 8 bytes; GR_REGS or none.
Register 72 used 5 times across 20 insns; GR_REGS or none.
"""
        greg = """
;; Function decoy

;; 1 regs to allocate: 99

;; Register dispositions:
99 in 16

;; Function test

;; 2 regs to allocate: 71 72

;; Register dispositions:
71 in 16  72 in 18

"""
        order, allocnos, _ = alloc_diff.reconstruct_allocnos(lreg, greg, "test")

        self.assertEqual(order, [71, 72])
        self.assertEqual(allocnos[71].size, 2)
        self.assertEqual(allocnos[71].priority, 10000)
        self.assertEqual(allocnos[72].size, 1)
        self.assertEqual(allocnos[72].priority, 5000)

    def test_shared_allocno_uses_last_members_size(self) -> None:
        lreg = """
;; Function test

Register 71 used 5 times across 20 insns; GR_REGS or none.
Register 72 used 5 times across 20 insns; 8 bytes; GR_REGS or none.

(insn 1 0 0 (set (reg/v:SI 71)
        (reg/v:DI 72)) 141 {movdi} (nil))
"""
        greg = """
;; Function test

;; 1 regs to allocate: 71

;; Register dispositions:
71 in 16  72 in 16

"""
        order, allocnos, _ = alloc_diff.reconstruct_allocnos(lreg, greg, "test")

        self.assertEqual(order, [71])
        self.assertEqual(allocnos[71].members, (71, 72))
        self.assertEqual(allocnos[71].size, 2)
        self.assertEqual(allocnos[71].priority, 30000)


if __name__ == "__main__":
    unittest.main()
