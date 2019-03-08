#include <stdio.h>
#include <unistd.h>
#include <mpi.h>
#if defined(NO_GETCPU)
int sched_getcpu( void ) { return -1; }
#else
#include <sched.h>
#endif

int main ( int argc, char** argv )
{
    int mpi_rank, mpi_size;
    char host[1024];

    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &mpi_rank );
    MPI_Comm_size( MPI_COMM_WORLD, &mpi_size );

    gethostname( host, 1024 );
#if defined(_OPENMP) 
    #pragma omp parallel
    {
        printf( "Hello from thread %2d of %2d in rank %3d of %3d running on cpu %2d on %s\n", 
            omp_get_thread_num(), 
            omp_get_num_threads(),
            mpi_rank, 
            mpi_size, 
            sched_getcpu(),
            host );
    }
#else
    printf( "Hello from rank %d of %d running on cpu %d on %s\n", 
        mpi_rank, 
        mpi_size, 
        sched_getcpu(),
        host );
#endif

    MPI_Finalize();
    return 0;
}
