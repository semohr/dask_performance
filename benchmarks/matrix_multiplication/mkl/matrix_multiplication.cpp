#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <mkl.h>
#include <mpi.h>

#define min(x, y) (((x) < (y)) ? (x) : (y))

double stoptime(void)
{
  struct timeval t;
  gettimeofday(&t, 0);
  return (double)t.tv_sec + t.tv_usec / 1000000.0;
}

// Pass the matrices to be filled with random numbers
void FillMatricesRandomly(double *A, int width, int height)
{
  /* initialize the random number generator with the current time */
  for (int i = 0; i < width * height; i++)
  {
    // rand [0,1)
    A[i] = i;
  }
}

int main(int argc, char *argv[])
{

  int num_procs, rank;
  // Initialize MPI
  MPI_Init(&argc, &argv);

  // Get the number of processors and the rank of the current processor
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Check if the input arguments are correct
  if (argc != 3 && rank == 0)
  {
    printf("Usage: %s <matrix size> <number of samples>\n", argv[0]);
    return 1;
  }

  // Define the matrices and the variables for matrix multiplication
  double *A, *B, *C, *local_C, *local_A;
  u_int32_t m, n, k, rows_per_proc;
  double alpha = 1.0;
  double beta = 0.0;
  double t;

  // Get the size of the matrices and the number of samples
  int size = atoi(argv[1]);
  int samples = atoi(argv[2]);
  m = n = k = size;

  // Calculate the number of rows per processor
  rows_per_proc = m / num_procs;
  if (rank == num_procs - 1)
  {
    rows_per_proc += m % num_procs;
  }
  // Allocate memory for the matrices / spill the matrices to disk
  A = (double *)mkl_malloc(m * k * sizeof(double), 64);
  B = (double *)mkl_malloc(k * n * sizeof(double), 64);
  C = (double *)mkl_malloc(m * n * sizeof(double), 64);
  local_A = (double *)mkl_malloc(rows_per_proc * k * sizeof(double), 64);
  local_C = (double *)mkl_malloc(rows_per_proc * n * sizeof(double), 64);

  for (int i = 0; i < samples; i++)
  {
    // Initialize the matrices with random numbers
    if (rank == 0)
    {
      FillMatricesRandomly(A, m, k);
      FillMatricesRandomly(B, k, n);
      // Initialize the result matrix with 0
      for (u_int64_t j = 0; j < m * n; j++)
      {
        C[i] = 0.0;
      }
    }
    t = stoptime();

    // Initialize the local result matrix with 0
    for (u_int64_t j = 0; j < rows_per_proc * n; j++)
    {
      local_C[i] = 0.0;
    }

    // Scatter the matrix A to all processors
    MPI_Scatter(A, rows_per_proc * k, MPI_DOUBLE, local_A, rows_per_proc * k, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    // Free the memory
    // Wait for all processors to finish the scatter
    // Send B to all processors
    MPI_Bcast(B, k * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Perform the local matrix multiplication
    // localC = localA * B
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, rows_per_proc, n, k, alpha, local_A, k, B, n, beta, local_C, n);

    // Gather the local results to the global matrix C
    MPI_Gather(local_C, rows_per_proc * n, MPI_DOUBLE, C, rows_per_proc * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Print the result matrix
    t = stoptime() - t;
    if (rank == 0)
    {
      printf("%f\n", t);
    }
  }

  mkl_free(A);
  mkl_free(B);
  mkl_free(C);
  mkl_free(local_A);
  mkl_free(local_C);

  // Finalize MPI
  MPI_Finalize();

  return 0;
}