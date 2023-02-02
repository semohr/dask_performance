
#include <memory>
#include <iostream>
#include <list>
#include <mpi.h>
#include <dirent.h>
#include <sys/time.h>

#include <orc/OrcFile.hh>
#include <orc/Reader.hh>
#include <orc/Exceptions.hh>

double stoptime(void)
{
    struct timeval t;
    gettimeofday(&t, 0);
    return (double)t.tv_sec + t.tv_usec / 1000000.0;
}

std::vector<std::string> get_files_from_folder(std::string folder_path)
{
    std::vector<std::string> part_files;

    // get the list of part files in the folder
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(folder_path.c_str())) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            std::string filename = ent->d_name;
            if (filename.find(".orc") != std::string::npos)
            {
                part_files.push_back(folder_path + "/" + filename);
            }
        }
        closedir(dir);
    }
    else
    {
        // unable to open directory
        std::cerr << "Error: Unable to open directory " << folder_path << std::endl;
        exit(1);
    }

    return part_files;
}

int main(int argc, char *argv[])
{

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Get the list of part files
    double t = stoptime();
    std::string folder_path = "/scratch03.local/smohr/dask/orc";
    std::vector<std::string> part_files;

    part_files = get_files_from_folder(folder_path);

    // create a reader for each part file
    std::list<uint64_t> read_cols = {
        0,
    };

    // Chunking for files with mpi
    uint32_t files_per_rank = part_files.size() / (size - 1);
    uint32_t start_index = rank * files_per_rank;
    uint32_t end_index = start_index + files_per_rank;
    if (rank == size - 1)
    {
        end_index = part_files.size();
    }

    // Read the files assigned to this rank
    orc::ReaderOptions reader_opts;
    double local_sum = 0;
    for (uint32_t i = start_index; i < end_index; ++i)
    {
        // Create reader for each file
        std::string file_path = part_files[i];
        orc::RowReaderOptions row_reader_opts;
        row_reader_opts.include(read_cols);

        std::unique_ptr<orc::Reader> reader = orc::createReader(orc::readFile(file_path), reader_opts);
        std::unique_ptr<orc::RowReader> row_reader = reader->createRowReader(row_reader_opts);
        std::unique_ptr<orc::ColumnVectorBatch> batch = row_reader->createRowBatch(64);

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
    }
    // Sum up the local sums
    double global_sum = 0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    t = stoptime() - t;

    // Compute the max time
    double max_time = 0;
    MPI_Reduce(&t, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        std::cout << t << std::endl;
    }

    // Finalize the MPI environment.
    MPI_Finalize();

    return 0;
}