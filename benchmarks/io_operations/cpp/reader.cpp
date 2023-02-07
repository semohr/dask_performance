#include <orc/OrcFile.hh>
#include <orc/Type.hh>
#include <orc/Vector.hh>
#include <orc/Writer.hh>

#include <iostream>

#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define WORKTAG 1
#define DIETAG 2

const std::string BASEPATH = "/scratch03.local/smohr/dask/orc/";
u_int64_t NROWS = 10000000000;
u_int32_t ROWS_PER_FILE = 31250000;
u_int32_t BATCH_SIZE = 1000;

double stoptime(void);
void master();
void worker();
int main(int argc, char *argv[])
{

    int myrank;
    MPI_Init(&argc, &argv);                 /* initialize MPI */
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank); /* process rank, 0 to N-1 */
    if (myrank == 0)
    {
        double t = stoptime();
        master();
        std::cout << stoptime() - t << std::endl;
    }
    else
    {
        worker();
    }
    MPI_Finalize(); /* cleanup MPI */
    return 0;
}

double stoptime(void)
{
    struct timeval t;
    gettimeofday(&t, 0);
    return (double)t.tv_sec + t.tv_usec / 1000000.0;
}

double readFile(std::string fname)
{
    double local_sum = 0;
    orc::ReaderOptions options;
    orc::RowReaderOptions row_options;

    std::unique_ptr<orc::Reader> reader = orc::createReader(orc::readFile(fname), options);
    std::unique_ptr<orc::RowReader> row_reader = reader->createRowReader(row_options);
    std::unique_ptr<orc::ColumnVectorBatch> batch = row_reader->createRowBatch(BATCH_SIZE);

    auto *fields = dynamic_cast<orc::StructVectorBatch *>(batch.get());
    auto *col0 = dynamic_cast<orc::DoubleVectorBatch *>(fields->fields[0]);
    double *buffer1 = col0->data.data();
    while (row_reader->next(*batch))
    {
        for (uint32_t r = 0; r < batch->numElements; ++r)
        {
            local_sum += buffer1[r];
        }
    }
    return local_sum;
}

// Naive work queue
typedef struct work_s
{
    // filename
    int file_id;
} work_t;

typedef struct result_s
{
    double file_sum;
} result_t;

work_t *workarray;
int work_id, nworks;

void init_work(int n)
{
    int i;
    workarray = (work_t *)malloc(n * sizeof(work_t));
    work_id = 0;
    nworks = n;
    for (i = 0; i < n; i++)
    {
        workarray[i].file_id = i;
    }
}

work_t *get_next_work_request()
{
    if (work_id >= nworks)
        return NULL;

    work_t *w = &workarray[work_id];
    work_id++;
    return w;
}

// Worker
void worker()
{
    int rank;
    result_t result; // success
    work_t work;     // filename
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for (;;)
    {
        MPI_Recv(&work, sizeof(work_t), MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        /* Check the tag of the received message */
        if (status.MPI_TAG == DIETAG)
        {
            return;
        }
        // Do the work
        std::string fname = BASEPATH + "part." + std::to_string(work.file_id) + ".orc";
        double local_sum = readFile(fname);

        result.file_sum = local_sum;
        MPI_Send(&result, sizeof(result_t), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
}

void master()
{
    int nworkers, rank;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &nworkers);

    int ntasks = 320;

    init_work(ntasks);

    work_t *work;

    // Seed the workers
    for (rank = 1; rank < nworkers; ++rank)
    {
        work = get_next_work_request();

        if (work == NULL)
            break;
        MPI_Send(work, sizeof(work_t), MPI_CHAR, rank, WORKTAG, MPI_COMM_WORLD);
    }

    // Results
    double sum = 0.0;

    /*
     * Receive a result from any worker and dispatch a new work
     * until work requests have been exhausted.
     */
    result_t res;
    work = get_next_work_request();
    while (work != NULL)
    {
        MPI_Recv(&res,             /* message buffer */
                 sizeof(result_t), /* one data item .. */
                 MPI_CHAR,         /* of a struct */
                 MPI_ANY_SOURCE,   /* receive from any sender */
                 MPI_ANY_TAG,      /* any type of message */
                 MPI_COMM_WORLD,   /* always use this */
                 &status);         /* received message info */

        MPI_Send(work, sizeof(work_t), MPI_CHAR, status.MPI_SOURCE, WORKTAG, MPI_COMM_WORLD);
        work = get_next_work_request();
        sum += res.file_sum;
    }

    // Receive results for outstanding work requests
    int outstanding;
    if (ntasks < nworkers)
        outstanding = ntasks;
    else
        outstanding = nworkers;

    for (rank = 1; rank < outstanding; ++rank)
    {
        MPI_Recv(&res,             /* message buffer */
                 sizeof(result_t), /* one data item .. */
                 MPI_CHAR,         /* of a struct */
                 MPI_ANY_SOURCE,   /* receive from any sender */
                 MPI_ANY_TAG,      /* any type of message */
                 MPI_COMM_WORLD,   /* always use this */
                 &status);         /* received message info */
        sum += res.file_sum;
    }

    // Tell all the workers to exit by sending an empty message with the DIETAG.
    for (rank = 1; rank < nworkers; ++rank)
    {
        MPI_Send(0, 0, MPI_CHAR, rank, DIETAG, MPI_COMM_WORLD);
    }
}