import os
import sys
from pathlib import Path
from time import sleep

job_script_template = r"""
#!/bin/bash
#$ -S /bin/bash
#$ -N matrix_multiplication
#$ -pe mvapich2-sam {{parallel}}
#$ -cwd         
#$ -o ./output/cpp_log_out_{{parallel}}.out
#$ -e ./output/cpp_log_out_{{parallel}}.err

source /usr/lmp/intel/bin/compilervars.sh intel64

export I_MPI_JOB_RESPECT_PROCESS_PLACEMENT=off

echo "==========================="
echo "Run"
cd /home/smohr/Repositories/dask_performance/benchmarks/matrix_multiplication/mkl
export OMP_NUM_THREADS=32
export MKL_NUM_THREADS=32
exe="./matrix_multiplication"
mpirun -rr ${exe} {{size}} {{samples}} >> ../data/times_mkl_size-{{size}}_parallel-{{parallel}}.txt
echo "==========================="
"""


"""
This script submits jobs the mpi jobs to the cluster.

Usage:
    python3 r_mpi.py <size> <samples>
"""
if __name__ == "__main__":
    size = int(sys.argv[1])
    samples = int(sys.argv[2])
    parallel = int(sys.argv[3])

    if parallel % 32 != 0:
        print("Parallel has to be multiple of 32")
        exit()

    job_script = job_script_template.replace("{{size}}", str(size))
    job_script = job_script.replace("{{samples}}", str(samples))
    job_script = job_script.replace("{{parallel}}", str(parallel))

    # CD to correct directory
    cur_dir = str(Path(__file__).parent.absolute())
    os.chdir(cur_dir + "/mkl")
    with open("temp_mp.job", "w") as f:
        f.write(job_script)

    # Submit job
    os.system("qsub temp_mp.job")
    sleep(1)

    # Delete job script
    os.system("rm temp_mp.job")
