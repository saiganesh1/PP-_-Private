#include <myhead.h>

int **G;
int **transitive;

int n_vertices;
int n_threads = 1;

void DFS(int u,int v)
{
	
    // Mark the current node as visited
	transitive[u][v] = 1;
  	
  	printf(" visiting %d\n", v);
    // Recur for all the vertices adjacent 
    // to this vertex 
   
    for (int i = 0; i < n_vertices; ++i)
    {

        if (!transitive[u][i] && G[v][i])
        {
            DFS(u,i); 
        }
    }

    return;
}

void *dfs_util(void *arg)
{
	#pragma omp parallel shared(n_threads) private(id,start,end)
	{
		n_threads = omp_get_num_threads();
		id = omp_get_thread_num();
		start = id * n_vertices / n_threads;
		end   = (id + 1) * n_vertices / n_threads;
		printf("thread %d from %d  to %d \n",id,start,end );
		for (int i = start; i < end; ++i)
		{
			DFS(i,i);
			printf("exitting\n");
		}
	}
}

void fill_random()
{
	for(int i=0;i<n_vertices;i++)
	{
		for (int j = 0; j < i; ++j)
		{
			G[i][j] = 1 + rand()%100;
		}
	}
}

int main(int argc, char *argv[])
{
	n_vertices = atoi(argv[1]);

	G = (int **)malloc(sizeof(int *)*n_vertices);
	transitive = (int **)malloc(sizeof(int *)*n_vertices);
	
	for (int i = 0; i < n_vertices; ++i)
	{
		G[i] = malloc(n_vertices* sizeof(int));
		transitive[i] = malloc(n_vertices* sizeof(int));
		for (int j = 0; j < n_vertices; ++j)
		{
			scanf("%d",&G[i][j]);
			transitive[i][j] = 0;
		}
	}
	printf("Starting up %d threads to calculate transitive closure ..\n", n_threads);

	pthread_t threads[n_threads];
	int thread_args[n_threads];

	struct timeval time_start;
	struct timeval time_end;

	gettimeofday(&time_start, NULL);

	for (int i = 0; i < n_threads; ++i)
	{
		thread_args[i] = i;
		pthread_create(&threads[i], NULL, dfs_util, &thread_args[i]);
	}



	// Wait for calculations to finish
	for (int i =0; i < n_threads; ++i )
	{
		pthread_join(threads[i], NULL);
	}

	gettimeofday(&time_end, NULL);

	long long execution_time = 1000000LL
		* (time_end.tv_sec  - time_start.tv_sec)
		+ (time_end.tv_usec - time_start.tv_usec);

	printf(" Finished calculating shortest paths in %lld micro seconds.\n\n",
		execution_time);

	for (int i = 0; i < n_vertices; ++i)
	{
		for (int j = 0; j < n_vertices; ++j)
		{
			printf("%d , ",transitive[i][j]);
		}
		printf("\n");
	}

	return 0;
}
