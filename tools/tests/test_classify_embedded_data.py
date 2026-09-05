import unittest

from tools.scripts.classify_embedded_data import move_data


class EmbeddedDataTest(unittest.TestCase):
    def unit(self):
        return {'metadata': {'complete': False},
                'measures': {'total_code': '8', 'matched_code': '8',
                             'total_functions': 2, 'matched_functions': 2},
                'functions': [{'name': 'Code', 'address': '0', 'size': '4', 'fuzzy_match_percent': 100},
                              {'name': 'Data', 'address': '4', 'size': '4', 'fuzzy_match_percent': 100}]}

    def test_data_stays_in_denominator_and_is_compared_to_retail(self):
        unit = self.unit()
        ranges = [{'name': 'Data', 'address': '4', 'size': '4'}]
        move_data(unit, ranges, b'codeDATA', b'codeDAT?')
        self.assertEqual(unit['measures']['total_code'], '4')
        self.assertEqual(unit['measures']['total_data'], '4')
        self.assertEqual(unit['measures']['matched_data'], '3')
        self.assertEqual(unit['measures']['matched_data_percent'], 75)
        self.assertEqual(unit['measures']['total_functions'], 1)
        self.assertEqual(unit['functions'][0]['name'], 'Code')

    def test_partial_overlap_cannot_hide_instructions(self):
        with self.assertRaisesRegex(ValueError, 'partially overlaps'):
            move_data(self.unit(), [{'name': 'Data', 'address': '2', 'size': '4'}],
                      b'codeDATA', b'codeDATA')

    def test_zero_storage_is_data_even_if_objdiff_treated_it_as_padding(self):
        unit = self.unit()
        move_data(unit, [{'name': 'Storage', 'address': '8', 'size': '4'}],
                  b'codeDATA' + bytes(4), b'codeDATA' + bytes(4))
        self.assertEqual(unit['measures']['total_code'], '8')
        self.assertEqual(unit['measures']['total_data'], '4')
