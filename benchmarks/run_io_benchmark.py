import subprocess
from time import sleep

""" This script run a suit of benchmarks and saves the results in the data folder.
"""
if __name__ == "__main__":

    parallels = [32, 64, 96, 128, 160, 192, 224, 256, 288, 320]

    # Run dask
    for parallel in parallels:
        """
        out = open(
            "./io_operations/data/times_dask_parallel-{}.txt".format(parallel),
            "a+",
        )
        p = subprocess.Popen(
            [
                "python3",
                "./io_operations/r_dask.py",
                str(parallel),
            ],
            stdout=out,
        )
        p.wait()
        sleep(120)
        print("Finished dask for parallel {}".format(parallel))
        """

        # Run mpi
        p = subprocess.Popen(
            [
                "python3",
                "./io_operations/r_cpp.py",
                str(parallel),
            ]
        )
        p.wait()
        sleep(120)
        print("Finished mpi for parallel {}".format(parallel))
