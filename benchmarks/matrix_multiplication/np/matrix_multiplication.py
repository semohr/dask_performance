#!/usr/bin/env python3
import sys
import time
import numpy as np

""" This script is used to benchmark the performance of numpy.

Usage:
    python3 r_numpy.py <size> <samples>
"""
if __name__ == "__main__":
    size = int(sys.argv[1])
    samples = int(sys.argv[2])
    id = int(sys.argv[3])
    if id != 1:
        exit(0)

    for i in range(samples):
        a = np.random.random((size, size))
        b = np.random.random((size, size))
        start = time.time()
        c = np.matmul(a, b)
        end = time.time()
        print(end - start)
