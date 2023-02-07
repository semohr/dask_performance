import subprocess
from time import sleep

""" This script run a suit of benchmarks and saves the results in the data folder.
"""
if __name__ == "__main__":

    parallels = [32, 64, 96, 128, 160, 192, 224, 256, 288]

    # Run dask
    for parallel in parallels:

        """ Writer
        """

        # CPP
        p = subprocess.Popen(
            [
                "python3",
                "./io_operations/r_cpp_writer.py",
                str(parallel),
            ]
        )
        p.wait()
        sleep(70)
        print("Finished writer cpp for parallel {}".format(parallel))


        # Dask
        out = open(
            "./io_operations/data/writer_dask_parallel-{}.txt".format(parallel),
            "a+",
        )
        p = subprocess.Popen(
            [
                "python3",
                "./io_operations/r_dask_writer.py",
                str(parallel),
            ],
            stdout=out,
        )
        p.wait()
        sleep(70)
        print("Finished writer dask for parallel {}".format(parallel))


        """ Reader
        """

        # CPP
        p = subprocess.Popen(
            [
                "python3",
                "./io_operations/r_cpp_reader.py",
                str(parallel),
            ]
        )
        p.wait()
        sleep(60)
        print("Finished mpi for parallel {}".format(parallel))


        # Dask
        out = open(
            "./io_operations/data/reader_dask_parallel-{}.txt".format(parallel),
            "a+",
        )
        p = subprocess.Popen(
            [
                "python3",
                "./io_operations/r_dask_reader.py",
                str(parallel),
            ],
            stdout=out,
        )
        p.wait()
        sleep(60)
        print("Finished dask for parallel {}".format(parallel))

