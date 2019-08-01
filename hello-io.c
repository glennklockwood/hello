#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <mpi.h>

#define BUF_SIZE 4096
#define PPN 16

int main ( int argc, char** argv )
{
    int fd;
    int mpi_rank, mpi_size;
    char buf[BUF_SIZE];
    char hostname[1024];
    char fname[4096];
    size_t buf_size;
    ssize_t written;
    buf_size = BUF_SIZE * sizeof(*buf);

    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &mpi_rank );
    MPI_Comm_size( MPI_COMM_WORLD, &mpi_size );
    gethostname( hostname, 1024 );

    int node_num = (mpi_rank / PPN);
    if (node_num % 2 == 0) /* even-numbered nodes */
    {
        int rank_in_node = mpi_rank % PPN;

        sprintf(fname, "hello.node%d.proc%d", node_num, rank_in_node);
        fd = open(fname, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (fd == -1) {
            fprintf(stderr, "Rank %2d on %s could not open %s for writing\n", mpi_rank, hostname, fname);
            MPI_Abort(MPI_COMM_WORLD, 1);
            return 1;
        }

        written = write(fd, buf, buf_size);
        if (written != buf_size) {
            fprintf(stderr, "Rank %2d on %s tried to write %ld bytes but only wrote %ld bytes\n", mpi_rank, hostname, buf_size, written);
            MPI_Abort(MPI_COMM_WORLD, 1);
            return 1;
        }
        close(fd);
        printf("Rank %2d on %s wrote %ld bytes to %s\n", mpi_rank, hostname, written, fname);

        MPI_Barrier(MPI_COMM_WORLD);
    }
    else /* odd-numbered nodes */
    {
        int rank_in_node = mpi_rank % PPN;
        MPI_Barrier(MPI_COMM_WORLD);

        sprintf(fname, "hello.node%d.proc%d", node_num - 1, rank_in_node);
        fd = open(fname, O_RDWR);
        if (fd == -1) {
            fprintf(stderr, "Rank %2d on %s could not open %s for reading\n", mpi_rank, hostname, fname);
            MPI_Abort(MPI_COMM_WORLD, 1);
            return 1;
        }

        written = read(fd, buf, buf_size);
        if (written != buf_size) {
            fprintf(stderr, "Rank %2d on %s tried to read %ld bytes but only read %ld bytes\n", mpi_rank, hostname, buf_size, written);
            MPI_Abort(MPI_COMM_WORLD, 1);
            return 1;
        }
        close(fd);

        printf("Rank %2d on %s read %ld bytes from %s\n", mpi_rank, hostname, written, fname);
    }

    MPI_Finalize();
    return 0;
}
