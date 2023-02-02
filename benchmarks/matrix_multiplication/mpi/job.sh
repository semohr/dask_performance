#!/bin/bash
#$ -S /bin/bash
#$ -N matrix_multiplication
#$ -pe mvapich2-sam 64 #Has to be multiple of 32
#$ -cwd         
#$ -o ./output/cpp_log_out.out
#$ -e ./output/cpp_log_out.err

# Activate conda env 
# >>>  conda initialize >>>
source /data.nst/smohr/anaconda3/bin/activate
conda activate mpi_mvapich2
# >>>  conda initialize >>>


echo "==========================="
echo "Make"
make
echo "==========================="

echo "==========================="
echo "Run"
exe="./matrix_multi"
mpirun ${exe} 1000 100 > ../data/times_mpi_1000.txt
echo "==========================="