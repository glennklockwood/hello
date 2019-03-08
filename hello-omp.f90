      program hello
      implicit none
      integer omp_get_thread_num, omp_get_num_threads
      write(6,*) 'outside of a parallel region, omp_num_threads=', 
     &                                  omp_get_num_threads()
C$OMP PARALLEL
      write(6,10) omp_get_thread_num(), omp_get_num_threads()
C$OMP END PARALLEL
  10  format('hello from thread ',i3, ' of ',i10)
      end
