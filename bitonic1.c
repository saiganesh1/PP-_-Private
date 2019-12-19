#include <myhead.h>

struct timeval start_time, end_time;
long long execution_time;


int N,n;          // data array size
int *a;         // data array to be sorted
int threadCount=0;
int threadlayers;
const int ASCENDING  = 1;
const int DESCENDING = 0;

//Structure
typedef struct
{
    int lo;
    int cnt;
    int dir;
    int layer;
} sarg;

void exchange(int i, int j)
{
  int t;
  t = a[i];
  a[i] = a[j];
  a[j] = t;
}

void compare(int i, int j, int dir)
{
  if (dir ==(a[i]>a[j]) ) 
    exchange(i,j);
}
//const is must
int desc(const void *a,const void *b)
{
    int* arg1 = (int *)a;
    int* arg2 = (int *)b;
    if( *arg1 > *arg2 ) return -1;
    else if( *arg1 == *arg2 ) return 0;
    return 1;
}

int asc(const void *a,const void *b)
{
    int* arg1 = (int *)a;
    int* arg2 = (int *)b;
    if( *arg1 < *arg2 ) return -1;
    else if( *arg1 == *arg2 ) return 0;
    return 1;
}
void bitonicMerge(int lo, int cnt, int dir) 
{
  if (cnt>1) 
  {
    int k=cnt/2;
    int i;
    for (i=lo; i<lo+k; i++)
      compare(i, i+k, dir);
    bitonicMerge(lo, k, dir);
    bitonicMerge(lo+k, k, dir);
  }
}

//Merge
void * PbitonicMerge( void * arg)
{
    int lo = ((sarg *)arg)->lo;
    int cnt = ((sarg *)arg)->cnt;
    int dir = ((sarg *)arg)->dir;
    int layer = ((sarg *)arg)->layer;

    if( cnt > 1 )
    {
        int k = cnt / 2;
        
        for(int i = lo; i < lo + k; ++i )
        {
            compare(i,i + k,dir);
        }
        if( layer <= 0 )
        {
            bitonicMerge(lo,k,dir);
            bitonicMerge(lo + k,k,dir);
            return NULL;
        }

        sarg arg1, arg2;
        pthread_t thread1, thread2;
        arg1.lo = lo;
        arg1.cnt = k;
        arg1.dir = dir;
        arg1.layer = layer - 1;
        arg2.lo = lo + k;
        arg2.cnt = k;
        arg2.dir = dir;
        arg2.layer = layer - 1;
        pthread_create( &thread1, NULL, PbitonicMerge, &arg1 );
        pthread_create( &thread2, NULL, PbitonicMerge, &arg2 ); 
        
        pthread_join( thread1, NULL );
        pthread_join( thread2, NULL );
    }
    return NULL;
}


//Bitonic sort
void * BitonicSort( void * arg)
{
    int lo = ((sarg *)arg)->lo;
    int cnt = ((sarg *)arg)->cnt;
    int dir = ((sarg *)arg)->dir;
    int layer = ((sarg *)arg)->layer;

    if ( cnt > 1 )
    {
        int k = cnt / 2;
        if( layer >= threadlayers ) 
        {
            qsort(a+lo,k,sizeof(int),asc);
            qsort(a+(lo+k),k,sizeof(int),desc);
        }
        else
        {
            sarg arg1;
            pthread_t thread1;
            arg1.lo = lo;
            arg1.cnt = k;
            arg1.dir = ASCENDING;
            arg1.layer = layer + 1;
            //printf("Creating a new Accending thread\n");
            threadCount++;
            pthread_create(&thread1,NULL,BitonicSort,&arg1);
            
            sarg arg2;
            pthread_t thread2;
            arg2.lo = lo + k;
            arg2.cnt = k;
            arg2.dir = DESCENDING;
            arg2.layer = layer + 1;
            //printf("Creating a new Decending thread\n");
            threadCount++;
            pthread_create(&thread2,NULL,BitonicSort,&arg2);
                       
            pthread_join( thread1, NULL );
            pthread_join( thread2, NULL );
        }
        sarg arg3;
        arg3.lo = lo;
        arg3.cnt = cnt;
        arg3.dir = dir;
        arg3.layer = threadlayers - layer;
        PbitonicMerge( &arg3 );
    }
    return 0;
}
int main( int argc, char **argv )
{
    
    N = 1 << atoi( argv[ 1 ] ); // Size of aray. 2^First argument.
    n = atoi( argv[ 2 ] );
    threadlayers = atoi( argv[ 2 ] ); // number of threads you want. 2^second argument
    if( threadlayers != 0 && threadlayers != 1 ) 
    {
	--threadlayers;
    }

    a = (int *) malloc( N * sizeof( int ) );

     
    for (int i = 0; i < N; i++)
    {
        a[i] = rand() % N; // (N - i);
    }
    
    gettimeofday( &start_time, NULL );

    sarg arg;
    arg.lo = 0;
    arg.cnt = N;
    arg.dir = ASCENDING;
    arg.layer = 0;    
    
    BitonicSort( &arg );

    gettimeofday( &end_time, NULL );
    
    execution_time = 1000000LL
        * (end_time.tv_sec  - start_time.tv_sec)
        + (end_time.tv_usec - start_time.tv_usec);

   
    
    for (int i = 0; i < N; i++)
    {
        printf("%d\n", a[i]);
    }
  printf("\n");

   printf( "Total time = %lld\n", execution_time );
    printf("Total threads used: %d\n",threadCount);

 }