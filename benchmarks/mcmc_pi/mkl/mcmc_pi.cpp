#include <iostream>
#include <random>
#include <chrono>
#include <fstream>
#include <mpi.h>
#include <mkl.h>
#include <sys/time.h>
#include "pcg_random.hpp"

double stoptime(void)
{
    struct timeval t;
    gettimeofday(&t, 0);
    return (double)t.tv_sec + t.tv_usec / 1000000.0;
}

void update_pi(double pi)
{
    // Append pi to file
    std::ofstream pi_file;

    pi_file.open("pi.txt", std::ios::app);
    pi_file.precision(std::numeric_limits<double>::max_digits10);
    pi_file << pi << std::endl;
}

int main(int argc, char **argv)
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Check if the input arguments are correct
    if (argc != 2 && rank == 0)
    {
        printf("Usage: %s <trials> \n", argv[0]);
        return 1;
    }
    long double n = std::stod(argv[1]);
    // Stop time and print using chrono

    double t = stoptime();
    // Use PCG RNG
    pcg64 rng(std::chrono::system_clock::now().time_since_epoch().count() + rank);
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    long double local_n = n / size;
    long double hit_count = 0;
    for (double i = 0; i < local_n; ++i)
    {
        double x = dist(rng);
        double y = dist(rng);
        if (x * x + y * y < 1)
        {
            ++hit_count;
        }
    }
    long double total_hit_count;
    MPI_Reduce(&hit_count, &total_hit_count, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        t = stoptime() - t;
        std::cout << t << std::endl;
        // Write pi to file
        update_pi(4.0 * total_hit_count / n);
    }

    MPI_Finalize();
    return 0;
}