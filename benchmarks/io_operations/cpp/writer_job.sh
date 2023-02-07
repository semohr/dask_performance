#!/bin/bash
#$ -S /bin/bash
#$ -N io_cpp
#$ -pe mvapich2-sam 32
#$ -cwd         
#$ -o ./output/cpp_log_out.out
#$ -e ./output/cpp_log_out.err

export LD_LIBRARY_PATH=/data.nst/smohr/anaconda3/envs/dask/lib

source /usr/lmp/intel/bin/compilervars.sh intel64


echo "==========================="
echo "Run"
cd /home/smohr/Repositories/dask_performance/benchmarks/io_operations/cpp
rm /scratch03.local/smohr/dask/orc/*.orc
exe="./writer"
mpirun ${exe} >> ../data/writer_cpp_parallel-32.txt
echo "==========================="