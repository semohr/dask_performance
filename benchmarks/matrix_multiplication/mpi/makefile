# Use the Open MPI-provided wrapper compilers.

MPICXX = mpic++

# Using -g is not necessary, but it is helpful for example programs,
# especially if users want to examine them with debuggers.  Note that
# gmake requires the CXXFLAGS macro, while other versions of make
# (such as Sun's make) require the CCFLAGS macro.
CXXFLAGS += -pedantic -Wall -O2 -std=c++11 -I./include 

TARGETS = matrix_multi

# Default target: build all the files
all: $(TARGETS)

matrix_multi: matrix_multi.cpp matrix.h
	$(MPICXX) $(CXXFLAGS) $(LDFLAGS) $? $(LDLIBS) -o $@

clean:
	rm -f *.o $(TARGETS)