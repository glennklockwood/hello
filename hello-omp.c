#include <stdio.h>
#if defined(NO_GETCPU)
int sched_getcpu( void ) { return -1; }
#else
#include <sched.h>
#endif


int main( int argc, char**argv )
{
    char host[1024];
    gethostname( host, 1024 );
#pragma omp parallel
    {
        printf( "Hello world from thread %d of %d running on cpu %2d on %s!\n", 
            omp_get_thread_num()+1, 
            omp_get_num_threads(),
            sched_getcpu(),
            host );
    }
    return 0;
}
