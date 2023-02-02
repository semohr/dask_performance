import subprocess
from time import sleep

""" This script run a suit of benchmarks and saves the results in the data folder.
"""
if __name__ == "__main__":

    # 2^8 to 2^14
    # array_size = [ 8192,  2, 32768]
    """ """
    array_size = [1024, 2048, 4096, 8192, 12288, 16384]
    # array_size = [16384]
    samples = 5
    # parallels = [32, 64, 128]
    parallels = [160]
    # parallels = [288, 320]
    # Run dask
    """
    # Run mpi
    for size in array_size:
        p = subprocess.Popen(["python3", "./matrix_multiplication/r_mpi.py", str(size), str(samples), str(parallel)])
        p.wait()
        sleep(1)
        print("Finished mpi for size {}".format(size))
    """
    for parallel in parallels:
        # Run dask

        for size in array_size:
            out = open(
                "./matrix_multiplication/data/times_dask_size-{}_parallel-{}.txt".format(
                    size, parallel
                ),
                "a+",
            )
            p = subprocess.Popen(
                [
                    "python3",
                    "./matrix_multiplication/r_dask.py",
                    str(size),
                    str(samples),
                    str(parallel),
                ],
                stdout=out,
            )
            p.wait()
            sleep(1)
            print("Finished dask for size {}".format(size))

        # Run mkl
        """
        for size in array_size:
            p = subprocess.Popen(
                [
                    "python3",
                    "./matrix_multiplication/r_mkl.py",
                    str(size),
                    str(samples),
                    str(parallel),
                ]
            )
            p.wait()
            sleep(1)
            print("Finished mkl for size {}".format(size))
        """

        """
        # Run numpy

        if parallel != 32:
            continue

        for size in array_size:
            p = subprocess.Popen(
                [
                    "python3",
                    "./matrix_multiplication/r_numpy.py",
                    str(size),
                    str(samples),
                    str(32),
                ]
            )
            p.wait()
            sleep(1)
            print("Finished numpy for size {}".format(size))

        """
