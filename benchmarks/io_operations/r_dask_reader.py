#!/usr/bin/env python3
import sys
import time
import os

import dask.array as da
import dask.dataframe as dd
from dask_jobqueue import SGECluster
from dask.distributed import Client, wait


""" This script is used to benchmark the performance of dask

Usage:
    python3 r_dask.py <trials> <parallel>
"""
if __name__ == "__main__":
    parallel = int(sys.argv[1])

    offset = 0
    if parallel < 32:
        offset = 32 - parallel

    # Setup dask
    cluster = SGECluster(
        cores=32,
        walltime="1500000",
        processes=32 - offset,
        memory="192GB",  # for memory requests, this must be specified
        interface="ib0",  # specify the network interface to use,
        job_extra_directives=["-pe mvapich2-sam 32", "-S /bin/bash"],
        job_script_prologue=[
            "export OMP_NUM_THREADS=1",
            "export MKL_NUM_THREADS=1",
            "export NUMEXPR_NUM_THREADS=1",
            "export OPENBLAS_NUM_THREADS=1",
        ],
        death_timeout=100,
        shebang="#!/bin/bash",
        local_directory="/scratch03.local/smohr/dask",
        log_directory="/scratch03.local/smohr/dask/logs",
    )
    client = Client(cluster)
    # Run tasks
    cluster.scale(cores=parallel)

    # Test computation
    x = da.random.uniform(-1, 1, 1000).compute()

    # Create a Dask dataframe with 10 million rows
    df = dd.read_orc("/scratch03.local/smohr/dask/orc")
    start = time.time()
    #Save to orc
    df.sum().compute()
    end = time.time()
    print(end - start)
