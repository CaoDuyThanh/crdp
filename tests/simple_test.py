import unittest
from crdp import rdp


class SimpleTest(unittest.TestCase):
    def test_empty_line(self):
        result = rdp([], 0)
        self.assertEqual(len(result), 0)

    def test_1_line(self):
        points = [[0, 5], [5, 5], [10, 5], [15, 5]]
        result = rdp(points, 0.1)
        self.assertEqual(len(result), 2)
        self.assertListEqual(result, [[0, 5], [15, 5]])

    def test_zig_zag_line(self):
        points = [[0, 5], [5, 7], [10, 3], [15, 5]]
        result = rdp(points, 0.1)
        self.assertEqual(len(result), 4)
        self.assertListEqual(result, [[0, 5], [5, 7], [10, 3], [15, 5]])

        points = [[0, 5], [5, 7], [10, 3], [15, 5]]
        result = rdp(points, 2.1)
        self.assertEqual(len(result), 2)
        self.assertListEqual(result, [[0, 5], [15, 5]])

    def test_a_ring(self):
        points = [[0, 0], [5, 0], [5, 5], [0, 5], [0, 0]]
        result = rdp(points, 0.1)
        self.assertEqual(len(result), 5)
        self.assertListEqual(result, [[0, 0], [5, 0], [5, 5], [0, 5], [0, 0]])
