MKLPATH = /usr/lmp/intel-2019u4/compilers_and_libraries_2019.4.243/linux/mkl/lib/intel64

COMPILER_FLAGS_PBLAS = -L$(MKLPATH) \
   -lmkl_scalapack_lp64             \
   -lmkl_blacs_intelmpi_lp64        \
   -lmkl_intel_lp64                 \
   -lmkl_intel_thread -lmkl_core    \
   -liomp5 -lpthread -lm -ldl -O3 -Wall

TARGETS = mcmc_pi

all: $(TARGETS)

mcmc_pi: mcmc_pi.cpp
	mpicxx -L$(MKL_LIB_DIR) -I$(MKL_INCLUDE_DIR) -I./include mcmc_pi.cpp -o mcmc_pi $(COMPILER_FLAGS_PBLAS)

.PHONY : clean

clean:
	rm -f mcmc_pi