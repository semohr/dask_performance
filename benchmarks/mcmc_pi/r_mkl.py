import os
import sys
from pathlib import Path

job_script_template = r"""
#!/bin/bash
#$ -S /bin/bash
#$ -N mkl_pi
#$ -pe mvapich2-sam {{parallel}}
#$ -cwd         
#$ -o ./output/cpp_log_out.out
#$ -e ./output/cpp_log_out.err

source /usr/lmp/intel/bin/compilervars.sh intel64


echo "==========================="
echo "Run"
cd /home/smohr/Repositories/dask_performance/benchmarks/mcmc_pi/mkl
export OMP_NUM_THREADS=1
export MKL_NUM_THREADS=1
exe="./mcmc_pi"
mpirun ${exe} {{trials}} >> ../data/times_mkl_trials-{{trials}}_parallel-{{parallel}}.txt
echo "==========================="
"""

"""
This script submits jobs the mpi jobs to the cluster.

Usage:
    python3 r_mkl.py <trials> <parallel>
"""
if __name__ == "__main__":
    trials = int(float(sys.argv[1]))
    parallel = int(sys.argv[2])

    if parallel % 32 != 0:
        print("Parallel has to be multiple of 32")
        exit()


    job_script = job_script_template.replace("{{trials}}", str(trials))
    job_script = job_script.replace("{{parallel}}", str(parallel))


    # CD to correct directory
    cur_dir = str(Path(__file__).parent.absolute())
    os.chdir(cur_dir+"/mkl")
    with open("temp_mp.job", "w") as f:
        f.write(job_script)

    # Submit job
    os.system("qsub temp_mp.job")

    # Delete job script
    os.system("rm temp_mp.job")