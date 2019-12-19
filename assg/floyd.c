#include <myhead.h>

int **G;
int  **D;
int  **through;

int n_vertices;
int n_threads = 1;

// We need a barrier to sync up the threads before doing the next iteration of
// calculations
pthread_barrier_t calc_barrier;

void *calculate_shorest_paths(void *arg)
{
	int id = *(int*) arg;

	// Determine the rows we will be calculating
	int start = id * n_vertices / n_threads;
	int end   = (id + 1) * n_vertices / n_threads;

	for (int k = 0; k < n_vertices; ++k)
	{
		// Wait for all threads before we do shortest path calculations on the
		// adjacency matrix for this intermediate city
		pthread_barrier_wait(&calc_barrier);

		for (int i = start; i < end; ++i)
		{
			for (int j = 0; j < n_vertices; ++j)
			{
				// If vertex k is on the shortest path from 
                // i to j, then update the value of dist[i][j] 

                if (D[i][k] + D[k][j] < D[i][j])
                {
                    D[i][j] = D[i][k] + D[k][j]; 
                	through[i][j] = k;
				}
			}
		}
	}

	printf("    Thread-%d  Finished all calculations for rows %2d => %2d\n",
		id, start + 1, end);

	return NULL;
}

void fill_random()
{
	for(int i=0;i<n_vertices;i++)
	{

		for (int j = 0; j < i; ++j)
		{
			G[i][j] = D[i][j] = D[j][i] = G[j][i] = 1 + rand()%100;
			through[i][j] = -1;
 		}
 		G[i][i] = D[i][i] = 0;
 		through[i][i] = -1;
	}
	
}



void print_path_directions(int a, int b)
{
	int intermediate = through[a][b];

	if (intermediate == -1)
	{
		printf("  %d -> %d = %d \n", a, b, D[a][b]);
		return;
	}
	else
	{
		print_path_directions(a, intermediate);
		print_path_directions(intermediate, b);
	}
}



int main(int argc, char *argv[])
{
	n_vertices = atoi(argv[2]);
	G = (int **)malloc(sizeof(int *)*n_vertices);
	D = (int **)malloc(sizeof(int *)*n_vertices);
	through = (int **)malloc(sizeof(int *)*n_vertices);
	
	for (int i = 0; i < n_vertices; ++i)
	{
		G[i] = malloc(n_vertices* sizeof(int));
		D[i] = malloc(n_vertices* sizeof(int));
		through[i] = malloc(n_vertices* sizeof(int));
	//	for (int j = 0; j < n_vertices; ++j)
		{
		//	scanf("%d",&G[i][j]);
	//		D[i][j] = G[i][j];
	//		through[i][j] = -1;
		}
		
	}

	fill_random();	
	
	if (argc > 1)
	{
		n_threads = atoi(argv[1]);
	}

	
	printf("Starting up %d threads to calculate shortest paths...\n", n_threads);

	pthread_t threads[n_threads];
	int thread_args[n_threads];

	pthread_barrier_init(&calc_barrier, NULL, n_threads);

	int t1 = time(NULL);

	for (int i = 0; i < n_threads; ++i)
	{
		thread_args[i] = i;
		pthread_create(&threads[i], NULL, calculate_shorest_paths, &thread_args[i]);
	}



	// Wait for calculations to finish
	for (int i =0; i < n_threads; ++i )
	{
		pthread_join(threads[i], NULL);
	}

	int t2 = time(NULL);

	

	printf(" Finished calculating shortest paths in %d seconds.\n\n",t2 - t1);

	return 0;
}
