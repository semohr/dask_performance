import os
import sys
from pathlib import Path

job_script = r"""
#!/bin/bash
#$ -S /bin/bash
#$ -N io_cpp
#$ -pe mvapich2-sam {{cores}}
#$ -cwd         
#$ -o ./output/cpp_log_out.out
#$ -e ./output/cpp_log_out.err

export LD_LIBRARY_PATH=/data.nst/smohr/anaconda3/envs/dask/lib

source /usr/lmp/intel/bin/compilervars.sh intel64


echo "==========================="
echo "Run"
cd /home/smohr/Repositories/dask_performance/benchmarks/io_operations/cpp
exe="./reader"
mpirun ${exe} >> ../data/reader_cpp_parallel-{{cores}}.txt
echo "==========================="
"""

"""
This script submits jobs the mpi jobs to the cluster.

Usage:
    python3 r_mpi.py <size> <samples>
"""
if __name__ == "__main__":
    cores = int(sys.argv[1])

    if cores % 32 != 0:
        print("Cores has to be multiple of 32")
        exit()

    job_script = job_script.replace("{{cores}}", str(cores))

    # CD to correct directory
    cur_dir = str(Path(__file__).parent.absolute())
    os.chdir(cur_dir + "/cpp")
    with open("temp_mp.job", "w") as f:
        f.write(job_script)

    # Submit job
    os.system("qsub temp_mp.job")

    # Delete job script
    os.system("rm temp_mp.job")
