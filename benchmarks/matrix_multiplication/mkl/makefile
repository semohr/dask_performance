MKLPATH = /usr/lmp/intel-2019u4/compilers_and_libraries_2019.4.243/linux/mkl/lib/intel64

COMPILER_FLAGS_PBLAS = -L$(MKLPATH) \
   -lmkl_scalapack_lp64             \
   -lmkl_blacs_intelmpi_lp64        \
   -lmkl_intel_lp64                 \
   -lmkl_intel_thread -lmkl_core    \
   -liomp5 -lpthread -lm -ldl -O3 -Wall

TARGETS = matrix_multiplication

all: $(TARGETS)

matrix_multiplication: matrix_multiplication.cpp
	mpicxx -L$(MKL_LIB_DIR) -I$(MKL_INCLUDE_DIR) matrix_multiplication.cpp -o matrix_multiplication $(COMPILER_FLAGS_PBLAS)

.PHONY : clean

clean:
	rm -f matrix_multiplication