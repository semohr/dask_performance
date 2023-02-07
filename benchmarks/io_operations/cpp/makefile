MKLPATH = /usr/lmp/intel-2019u4/compilers_and_libraries_2019.4.243/linux/mkl/lib/intel64
ORCPATH = /data.nst/smohr/anaconda3/envs/dask


INCLUDES = -I$(MKLPATH) -I$(ORCPATH)/include
LIBS = -L$(MKLPATH) -L$(ORCPATH)/libs -L/usr/lib64 -lorc -lm -Wl,-rpath -Wl,$(MKLPATH) -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5 -lpthread -lm -ldl -std=c++17 -O2
TARGETS = reader, writer

all: $(TARGETS)

reader: reader.cpp
	mpicxx $(INCLUDES) $(LIBS) reader.cpp -o reader $(COMPILER_FLAGS_PBLAS)

writer: writer.cpp
	mpicxx $(INCLUDES) $(LIBS)  writer.cpp -o writer $(COMPILER_FLAGS_PBLAS)


.PHONY : clean

clean:
	rm -f reader