from functools import reduce
import timeit
import csv

from array import array
from random import randint
from pysimd import count_not_negatives, naive_count_not_negatives

arr = None

def generate_random_array(size: int) -> array:
    global arr
    arr = array('h', [randint(-32768, 32767) for _ in range(size)])


def python_reduce_map() -> int:
    return reduce(int.__add__, map(lambda x: 1 if x >= 0 else 0, arr), 0)


def python_for_loop() -> int:
    cnt = 0
    for e in arr:
        if e >= 0:
            cnt += 1
    return cnt


def c_for_loop() -> int:
    return naive_count_not_negatives(arr)


def c_simd() -> int:
    return count_not_negatives(arr)


def time(method) -> int:
    return timeit.timeit(
        method, 
        number = 1
    )


if __name__ == '__main__':
    methods = (
        python_for_loop, python_reduce_map,
        c_for_loop, c_simd
    )
    sizes = (
        0, 100, 1000, 10_000, 50_000, 100_000, 300_000, 
        500_000, 1_000_000, 2_000_000, 5_000_000, 10_000_000    
    )
    table = [
        ("method", *sizes)
    ]
    for method in methods:
        print(f'Method {method.__name__} is running')
        row = [method.__name__]
        for size in sizes:
            print(f'For size: {size}')
            generate_random_array(size)
            sum = 0; N = 5
            for attempt in range(N):
                spent = time(method)
                sum += spent
            average = sum / N
            row.append(average)
        table.append(row)
    
    with open('profiling-results.csv', 'w', newline='') as file:
        csv.writer(file).writerows(table)
