#!/bin/bash
#$ -S /bin/bash
#$ -N matrix_multiplication
#$ -pe mvapich2-sam 128
#$ -cwd
#$ -o ./output/cpp_log_out.out
#$ -e ./output/cpp_log_out.err

source /usr/lmp/intel/bin/compilervars.sh intel64

export I_MPI_JOB_RESPECT_PROCESS_PLACEMENT=off

echo "==========================="
echo "Run"
cd /home/smohr/Repositories/dask_performance/benchmarks/matrix_multiplication/mkl
export OMP_NUM_THREADS=32
export MKL_NUM_THREADS=32
exe="./matrix_multiplication"
mpirun -rr ${exe} 10000 10 >> ../data/times_mpi_10000.txt
echo "==========================="
