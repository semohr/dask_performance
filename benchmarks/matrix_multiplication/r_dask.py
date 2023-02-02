#!/usr/bin/env python3
import sys
import time
import os

import dask.array as da
from dask_jobqueue import SGECluster
from dask.distributed import Client, wait

""" This script is used to benchmark the performance of dask

Usage:
    python3 r_dask.py <size> <samples>
"""
if __name__ == "__main__":
    size = int(sys.argv[1])
    samples = int(sys.argv[2])
    parallel = int(sys.argv[3])

    if parallel % 32 != 0:
        print("Parallel must be a multiple of 32")
        exit(1)

    cluster = SGECluster(
        cores=32,
        walltime="1500000",
        processes=32,
        memory="192GB",  # for memory requests, this must be specified
        interface="ib0",  # specify the network interface to use,
        job_extra_directives=["-pe mvapich2-sam 32", "-S /bin/bash"],
        job_script_prologue=[
            "export OMP_NUM_THREADS=1",
            "export MKL_NUM_THREADS=1",
            "export NUMEXPR_NUM_THREADS=1",
            "export OPENBLAS_NUM_THREADS=1",
        ],
        death_timeout=150,
        shebang="#!/bin/bash",
        local_directory="/scratch03.local/smohr/dask",
        log_directory="/scratch03.local/smohr/dask/logs",
    )
    client = Client(cluster)
    cluster.scale(cores=parallel)

    # Test computation
    x = da.random.uniform(-1, 1, 1000).compute()

    for i in range(samples):
        # Precompute the random numbers
        a = da.random.random((size, size)).persist()
        b = da.random.random((size, size)).persist()

        # Transferring the data to the cluster
        wait([a, b])
        start = time.time()
        c = da.matmul(a, b).compute()
        end = time.time()
        print(end - start)

    cluster.close()
