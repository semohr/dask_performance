#!/bin/bash
#$ -S /bin/bash
#$ -N io_pandas
#$ -pe mvapich2-sam 32
#$ -cwd         
#$ -o ./output/cpp_log_out.out
#$ -e ./output/cpp_log_out.err
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
python -u ./io.py $SGE_TASK_ID >> ../data/times_pandas_parallel-32.txt
echo "==========================="