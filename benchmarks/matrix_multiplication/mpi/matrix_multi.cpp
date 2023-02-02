#include <cstdlib>
#include <sys/time.h>
#include <iostream>
#include <mpi.h>
#include <algorithm>
#include "matrix.h"

using namespace std;

// See also https://github.com/mrigankdoshy/matrix-multiplication/blob/master/mpi.cpp

// Instantiate global variables used in the parallelization
int mpi_rank;         // mpi: process id number
int nProcesses;       // mpi: number of total processess
MPI_Status status;    // mpi: store status of a MPI_Recv
MPI_Request request;  // mpi: capture request of a MPI_Isend
int rowStart, rowEnd; // which rows of A that are calculated by the slave process
int granularity;      // granularity of parallelization (# of rows per processor)

#define ROW_START_TAG 0  // tag for communicating the start row of the workload for a slave
#define ROW_END_TAG 1    // tag for communicating the end row of the workload for a slave
#define A_ROWS_TAG 2     // tag for communicating the address of the data to be worked on to slave
#define C_ROWS_TAG 3     // tag for communicating the address of the calculated data to master
#define LOCAL_TIME_TAG 4 // tag for communicating the address of the local matrix calculation time to master

void FillMatricesRandomly(Matrix<double> &A, Matrix<double> &B)
{
    /* initialize the random number generator with the current time */
    srand(time(NULL));
    for (int i = 0; i < A.rows(); i++)
    {
        for (int j = 0; j < A.cols(); j++)
        {
            A(i, j) = ((double)rand()) / RAND_MAX;
        }
    }
    for (int i = 0; i < B.rows(); i++)
    {
        for (int j = 0; j < B.cols(); j++)
        {
            B(i, j) = ((double)rand()) / RAND_MAX;
        }
    }
}

int main(int argc, char *argv[])
{
    // Parse command line arguments
    // size , samples
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " size samples" << endl;
        cout << "size: size of the matrix" << endl;
        exit(1);
    }
    int size = atoi(argv[1]);
    int samples = atoi(argv[2]);

    // Declare matrices
    Matrix<double> A = Matrix<double>(size, size);
    Matrix<double> B = Matrix<double>(size, size);
    Matrix<double> C = Matrix<double>(size, size);
    struct timeval start_time, end_time;
    double local_time;

    // Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);   /* store the rank */
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses); /* store the number of processes */

    for (int i = 0; i < samples; i++)
    {
        // Master initializes work
        if (mpi_rank == 0)
        {
            FillMatricesRandomly(A, B);

            /* Begin Timing: used for total multiplication time: communication + calculations  */
            gettimeofday(&start_time, NULL);
            for (int i = 1; i < nProcesses; i++)
            { /* for each slave */
                // calculate granularity (-1 comes from excluding the master process)
                granularity = (size / (nProcesses - 1));
                rowStart = (i - 1) * granularity;

                if (((i + 1) == nProcesses) && ((size % (nProcesses - 1)) != 0))
                {                  // if rows of [A] cannot be equally divided among slaves
                    rowEnd = size; // last slave gets all the remaining rows
                }
                else
                {
                    rowEnd = rowStart + granularity; // rows of [A] are equally divisable among slaves
                }

                // send the low bound, without blocking, to the intended slave
                MPI_Isend(&rowStart, 1, MPI_INT, i, ROW_END_TAG, MPI_COMM_WORLD, &request);
                // next send the upper bound without blocking, to the intended slave
                MPI_Isend(&rowEnd, 1, MPI_INT, i, ROW_START_TAG, MPI_COMM_WORLD, &request);
                // finally send the allocated row granularity of [A] without blocking, to the intended slave
                MPI_Isend(&A(rowStart, 0), (rowEnd - rowStart) * size, MPI_DOUBLE, i, A_ROWS_TAG, MPI_COMM_WORLD, &request);
            }
        }

        // broadcast B (MPI_Bcast: Broadcasts a message from the process with rank "root" to all other processes of the communicator)
        MPI_Bcast(&B(0, 0), size * size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        // work done by slaves (not rank = 0)
        if (mpi_rank > 0)
        {
            // receive low bound from the master
            MPI_Recv(&rowStart, 1, MPI_INT, 0, ROW_END_TAG, MPI_COMM_WORLD, &status);
            // next receive upper bound from the master
            MPI_Recv(&rowEnd, 1, MPI_INT, 0, ROW_START_TAG, MPI_COMM_WORLD, &status);
            // finally receive row granularity of [A] to be processed from the master
            MPI_Recv(&A(rowStart, 0), (rowEnd - rowStart) * size, MPI_DOUBLE, 0, A_ROWS_TAG, MPI_COMM_WORLD, &status);

            // start time for local time: the amount of time to do matrix calculation for this process
            local_time = MPI_Wtime();

            for (int i = rowStart; i < rowEnd; i++)
            { // the given set of rows of A (parallelized loop)
                for (int j = 0; j < B.cols(); j++)
                { // iterate through columns of [B]
                    for (int k = 0; k < B.rows(); k++)
                    { // iterate through rows of [B]
                        C(i, j) += (A(i, k) * B(k, j));
                    }
                }
            }

            local_time = MPI_Wtime() - local_time; // end time for local time

            // send back the low bound first without blocking, to the master
            MPI_Isend(&rowStart, 1, MPI_INT, 0, ROW_END_TAG, MPI_COMM_WORLD, &request);
            // send the upper bound next without blocking, to the master
            MPI_Isend(&rowEnd, 1, MPI_INT, 0, ROW_START_TAG, MPI_COMM_WORLD, &request);
            // finally send the processed granularity of data without blocking, to the master
            MPI_Isend(&C(rowStart, 0), (rowEnd - rowStart) * size, MPI_DOUBLE, 0, C_ROWS_TAG, MPI_COMM_WORLD, &request);
            // send back the local calculation time without blocking, to the master
            MPI_Isend(&local_time, 1, MPI_DOUBLE, 0, LOCAL_TIME_TAG, MPI_COMM_WORLD, &request);
        }

        // Gather results from slaves
        if (mpi_rank == 0)
        {
            for (int i = 1; i < nProcesses; i++)
            { // untill all slaves have handed back the processed data
                // receive low bound from a slave
                MPI_Recv(&rowStart, 1, MPI_INT, i, ROW_END_TAG, MPI_COMM_WORLD, &status);
                // receive upper bound from a slave
                MPI_Recv(&rowEnd, 1, MPI_INT, i, ROW_START_TAG, MPI_COMM_WORLD, &status);
                // //receive processed data from a slave
                MPI_Recv(&C(rowStart, 0), (rowEnd - rowStart) * size, MPI_DOUBLE, i, C_ROWS_TAG, MPI_COMM_WORLD, &status);
            }
            gettimeofday(&end_time, NULL);
            // end time of the total matrix matrix multiplication
            // print the total time taken for the matrix multiplication in seconds
            cout << (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0 << endl;
        }
    }

    MPI_Finalize(); // finalize MPI operations
    return 0;
}