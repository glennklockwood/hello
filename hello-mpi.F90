program hello
use mpi
integer :: myrank, mysize, ierror
!$ integer :: omp_get_thread_num, omp_get_num_threads
   
call MPI_INIT(ierror)
call MPI_COMM_SIZE(MPI_COMM_WORLD, mysize, ierror)
call MPI_COMM_RANK(MPI_COMM_WORLD, myrank, ierror)

#if defined(_OPENMP)
!$OMP PARALLEL
write(6,*) 'Hello world from thread ', omp_get_thread_num(), &
           ' of ', omp_get_num_threads(), ' in rank ', &
           myrank, ' of ', mysize
!$OMP END PARALLEL
#else
write(6,*) 'Hello world from rank ', myrank, ' of ', mysize
#endif

call MPI_FINALIZE(ierror)
end program hello
