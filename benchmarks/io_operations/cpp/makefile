MKLPATH = /usr/lmp/intel-2019u4/compilers_and_libraries_2019.4.243/linux/mkl/lib/intel64

COMPILER_FLAGS_PBLAS = -L$(MKLPATH) -lorc -lm -Wl,-rpath -O3 -Wl,$(MKLPATH) -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5 -lpthread -lm -ldl

TARGETS = main

all: $(TARGETS)

main: main.cpp
	mpicxx -L$(MKL_LIB_DIR) -I$(MKL_INCLUDE_DIR) -I./include main.cpp -o io_benchmark $(COMPILER_FLAGS_PBLAS)

.PHONY : clean

clean:
	rm -f main