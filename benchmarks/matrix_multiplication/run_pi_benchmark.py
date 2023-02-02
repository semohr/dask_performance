import subprocess
from time import sleep
""" This script run a suit of benchmarks and saves the results in the data folder.
"""
if __name__ == "__main__":

    # 2^8 to 2^14
    #array_size = [64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 24576, 32768]
    trials = [1e11, 1e12, 1e13, 1e14]
    # 32 steps stating from 64
    parallels =  [96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416]
    #parallels = [32]
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
        for trial in trials:
            if parallel == 96:
                continue

            out = open("./mcmc_pi/data/times_dask_trial-{}_parallel-{}.txt".format(int(trial), parallel), "a+")
            p = subprocess.Popen(["python3", "./mcmc_pi/r_dask.py", str(trial), str(parallel)], stdout=out)
            p.wait()
            sleep(1)
            print(f"Finished dask for trial {trial} parallel {parallel}")
    
        # Run mkl
        for trial in trials:
            p = subprocess.Popen(["python3", "./mcmc_pi/r_mkl.py", str(trial), str(parallel)])
            p.wait()
            sleep(1)
            print(f"Finished mkl for trial {trial} parallel {parallel}")

        # Run numpy
        if parallel != 32:
           continue

        for trial in trials:
            p = subprocess.Popen(["python3", "./mcmc_pi/r_numpy.py", str(trial), str(32)])
            p.wait()
            sleep(1)
            print("Finished numpy for trial {}".format(trial))

