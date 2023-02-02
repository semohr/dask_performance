#!/usr/bin/env python3
import sys
import time
import numpy as np

from multiprocessing.pool import ThreadPool

def mean(runs):
    return sum(runs)/len(runs)


def calc_pi(trials=1_000_000):
    x = np.random.uniform(-1, 1, trials)
    y = np.random.uniform(-1, 1, trials)
    radius = x**2 + y**2
    counts = np.sum(radius < 1.0)
    return 4 * counts / trials


""" This script is used to benchmark the performance of numpy.

Usage:
    python3 r_numpy.py <trials> <id>
"""
if __name__ == '__main__':
    trials = int(sys.argv[1])
    id = int(sys.argv[2])

    if id != 1:
        exit(0)

    # Start time

    local_trials = trials//32

    # Start 32 processes
    pool = ThreadPool(32)
    start = time.time()
    results = pool.map(calc_pi, [local_trials]*32)

    # Calculate pi and measure time
    pi = mean(results)
    end = time.time()
    print(end - start)

    # Calculate new pi using old result if exists in pi.txt
    # Append pi to file with max precision
    with open("pi.txt", "a") as f:
        f.write(f"{pi:.10f}")

