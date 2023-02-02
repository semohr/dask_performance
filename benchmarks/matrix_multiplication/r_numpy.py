import os
import sys
from pathlib import Path

job_script_template = r"""
#!/bin/bash
#$ -S /bin/bash
#$ -N matrix_multiplication
#$ -pe mvapich2-sam {{parallel}} #Has to be multiple of 32
#$ -cwd         
#$ -o ./output/python_log_out.out
#$ -e ./output/python_log_out.err
#$ -t 1

export MKL_NUM_THREADS=$NSLOTS
export OMP_NUM_THREADS=$NSLOTS
export NUMEXPR_NUM_THREADS=$NSLOTS
export OPENBLAS_NUM_THREADS=$NSLOTS

# Activate conda env 
# >>>  conda initialize >>>
source /data.nst/smohr/anaconda3/bin/activate
conda activate dask
# >>>  conda initialize >>>


echo "==========================="
echo "Run"
python -u ./matrix_multiplication.py {{size}} {{samples}} $SGE_TASK_ID >> ../data/times_numpy_size-{{size}}_parallel-{{parallel}}.txt
echo "==========================="
"""




"""
This script submits jobs the mpi jobs to the cluster.

Usage:
    python3 r_numpy.py <size> <samples>
"""
if __name__ == "__main__":
    size = int(sys.argv[1])
    samples = int(sys.argv[2])
    parallel = int(sys.argv[3])
    if parallel != 32:
        print("Parallel has to be for numpy = 32")
        exit()


    job_script = job_script_template.replace("{{size}}", str(size))
    job_script = job_script.replace("{{samples}}", str(samples))
    job_script = job_script.replace("{{parallel}}", str(parallel))

    # CD to correct directory
    cur_dir = str(Path(__file__).parent.absolute())
    os.chdir(cur_dir+"/np")
    with open("temp_mp.job", "w") as f:
        f.write(job_script)

    # Submit job
    os.system("qsub temp_mp.job")

    # Delete job script
    os.system("rm temp_mp.job")