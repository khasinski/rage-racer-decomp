import unittest

from tools.scripts.audit_report import audit, check_ranges


def function(name, address, size):
    return {'name': name, 'address': str(address), 'size': str(size)}


class ReportRangeTest(unittest.TestCase):
    def test_missing_linked_unit_fails_before_reading_objects(self):
        with self.assertRaisesRegex(ValueError, 'linked objects'):
            audit({'units': []}, ['src/main/Missing.c.o'], None)

    def test_unreported_nonzero_bytes_fail_even_with_matching_function(self):
        with self.assertRaisesRegex(ValueError, 'nonzero'):
            check_ranges([function('first', 0, 4)], b'ABCDhidden')

    def test_overlap_cannot_inflate_denominator(self):
        with self.assertRaisesRegex(ValueError, 'overlapping'):
            check_ranges([function('first', 0, 8), function('second', 4, 4)],
                         b'ABCDEFGH')

    def test_zero_padding_is_explicitly_counted(self):
        self.assertEqual(check_ranges([function('first', 4, 4)],
                                      bytes(4) + b'ABCD' + bytes(8)), (4, 12))

    def test_missing_entire_text_section_fails(self):
        with self.assertRaisesRegex(ValueError, 'nonzero'):
            check_ranges([], b'ABCD')

    def test_data_is_covered_without_being_counted_as_code_or_padding(self):
        self.assertEqual(check_ranges([function('Code', 0, 4)],
            b'codeDATA' + bytes(4), [function('Data', 4, 4)]), (4, 4))

    def test_data_cannot_overlap_code(self):
        with self.assertRaisesRegex(ValueError, 'overlapping'):
            check_ranges([function('Code', 0, 8)], b'codeDATA',
                         [function('Data', 4, 4)])
