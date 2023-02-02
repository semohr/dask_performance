#!/usr/bin/env python3
import sys
import time
import os

import dask.array as da
from dask_jobqueue import SGECluster
from dask.distributed import Client


def calc_pi(trials=100_000_000):
    x = da.random.uniform(-1, 1, trials)
    y = da.random.uniform(-1, 1, trials)
    radius = x**2 + y**2
    counts = da.sum(radius < 1.0)
    return 4 * counts / trials


os.environ["OMP_NUM_THREADS"] = "32"
os.environ["MKL_NUM_THREADS"] = "32"
os.environ["NUMEXPR_NUM_THREADS"] = "32"
os.environ["OPENBLAS_NUM_THREADS"] = "32"

""" This script is used to benchmark the performance of dask

Usage:
    python3 r_dask.py <trials> <parallel>
"""
if __name__ == "__main__":
    trials = int(float(sys.argv[1]))
    parallel = int(sys.argv[2])

    if parallel % 32 != 0:
        print("Parallel must be a multiple of 32")
        exit(1)

    # Setup dask
    cluster = SGECluster(
        cores=32,
        walltime="1500000",
        processes=1,
        memory="192GB",  # for memory requests, this must be specified
        interface="ib0",  # specify the network interface to use,
        job_extra_directives=["-pe mvapich2-sam 32", "-S /bin/bash"],
        job_script_prologue=[
            "export OMP_NUM_THREADS=32",
            "export MKL_NUM_THREADS=32",
            "export NUMEXPR_NUM_THREADS=32",
            "export OPENBLAS_NUM_THREADS=32",
        ],
        death_timeout=100,
        shebang="#!/bin/bash",
        local_directory="/scratch03.local/smohr/dask",
        log_directory="/scratch03.local/smohr/dask/logs",
    )
    client = Client(cluster)
    cluster.scale(parallel // 32)

    # Test computation
    x = da.random.uniform(-1, 1, 1000).compute()

    # Calculate pi and measure time
    pi = calc_pi(trials)
    start = time.time()
    pi = pi.compute()
    end = time.time()
    print(end - start)

    cluster.close()

    # Append pi to file with max precision
    with open("pi.txt", "a") as f:
        f.write(f"{pi:.10f}\n")
