import unittest
import array
import pysimd as simd


def arr(*args: list) -> array.array:
    return array.array('h', list(args))


class TestPySimd(unittest.TestCase):
    def test_available(self):
        a = arr(-1, 0, 1, 0, 2, 2, 2131, -21)
        simd.count_not_negatives(a)
        simd.naive_count_not_negatives(a)

    def test_correct_naive(self):
        self.assertEqual(6, simd.naive_count_not_negatives(
                arr(-1, 0, 1, 0, 2, 2, 2131, -21)
            )
        )
        self.assertEqual(5, simd.naive_count_not_negatives(
                arr(0, 0, 0, 0, 0)
            )
        )
        self.assertEqual(0, simd.naive_count_not_negatives(
                arr()
            )
        )
        self.assertEqual(0,
            simd.naive_count_not_negatives(
                arr(-1)
            )
        )
        self.assertEqual(2, simd.naive_count_not_negatives(
                arr(-1, 0, -2, -342, 1)
            )
        )

    def test_correct_simd_len_divisible_by_8(self):
        self.assertEqual(6, simd.count_not_negatives(
                arr(-1, 0, 1, 0, 2, 2, 2131, -21)
            )
        )
        self.assertEqual(0, simd.count_not_negatives(
                arr()
            )
        )
        self.assertEqual(8, simd.count_not_negatives(
                arr(0, 0, 0, 0, 0, 0, 0, 0)
            )
        )

    def test_correct_simd(self):
        self.assertEqual(0, simd.count_not_negatives(
                arr()
            )
        )
        self.assertEqual(2, simd.count_not_negatives(
                arr(1, 1)
            )
        )
    

if __name__ == '__main__':
    unittest.main()
