#!/bin/bash
#$ -S /bin/bash
#$ -N matrix_multiplication
#$ -pe mvapich2-sam 32 #Has to be multiple of 32
#$ -cwd         
#$ -o ./output/python_log_out.out
#$ -e ./output/python_log_out.err
#$ -t 1

# Activate conda env 
# >>>  conda initialize >>>
source /data.nst/smohr/anaconda3/bin/activate
conda activate dask
# >>>  conda initialize >>>

# multithreading in numpy
export OPENBLAS_NUM_THREADS=$NSLOTS
export MKL_NUM_THREADS=$NSLOTS
export NUMEXPR_NUM_THREADS=$NSLOTS
export OMP_NUM_THREADS=$NSLOTS

echo "==========================="
echo "Run"
python -u ./matrix_multiplication.py 4000 10 $SGE_TASK_ID >> ../data/times_numpy_size-{{size}}_parallel-32.txt
echo "==========================="