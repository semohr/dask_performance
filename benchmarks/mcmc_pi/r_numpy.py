import os
import sys
from pathlib import Path

job_script_template = r"""
#!/bin/bash
#$ -S /bin/bash
#$ -N numpy_pi
#$ -pe mvapich2-sam {{parallel}} #Has to be multiple of 32
#$ -cwd         
#$ -o ./output/python_log_out.out
#$ -e ./output/python_log_out.err
#$ -t 1


# Activate conda env 
# >>>  conda initialize >>>
source /data.nst/smohr/anaconda3/bin/activate
conda activate dask
# >>>  conda initialize >>>


echo "==========================="
echo "Run"
python -u ./mcmc_pi.py {{trials}} $SGE_TASK_ID >> ../data/times_numpy_trials-{{trials}}_parallel-{{parallel}}.txt
echo "==========================="
"""

"""
This script submits jobs the mpi jobs to the cluster.

Usage:
    python3 r_numpy.py <trials> <parallel>
"""
if __name__ == "__main__":
    trials = int(float(sys.argv[1]))
    parallel = int(sys.argv[2])
    if parallel != 32:
        print("Parallel has to be for numpy = 32")
        exit()


    job_script = job_script_template.replace("{{trials}}", str(trials))
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