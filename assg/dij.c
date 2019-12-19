#include <myhead.h>
/*
* sptSet = Contains the vertices which are in the shortest path tree Set
* G = input Graph which is filled Randomly
* dist = distance array which is meant for storing all shortest distances from a given source to all
		vertices
*/

int n_vertices;
int n_threads;
int *sptSet;
int *arr;
int **G;
int *dist;
int *parent;
int u;
extern int errno;


// A utility function to find the vertex with minimum distance value, from 
// the set of vertices not yet included in shortest path tree 
int minDistance() 
{ 
	// Initialize min value 
	int min = INT_MAX, min_index; 

	for (int v = 0; v < n_vertices; v++) 
	{
		if (sptSet[v] == 0 && dist[v] <= min)
		{
			min = dist[v];
			min_index = v;	
		}
	}
	return min_index; 
} 

// A utility function to print the constructed distance array 
int printSolution() 
{ 
	printf("Vertex Distance from Source\n"); 
	for (int i = 0; i < n_vertices; i++) 
		printf("%d tt %d\n", i, dist[i]); 
} 


void * updateDistances(void *arg)
{
	int id = *(int *)arg;
	int start = id * n_vertices / n_threads;
	int end   = (id + 1) * n_vertices / n_threads;
	printf("start = %d ; end =  %d \n", start,end-1);
	for (int v = start; v < end; ++v )
	{

		// Update dist[v] only if is not in sptSet, there is an edge from 
		// u to v, and total weight of path from src to v through u is 
		// smaller than current value of dist[v] 
		if (sptSet[v] == 0 && G[u][v] && dist[u] != INT_MAX && dist[u]+G[u][v] < dist[v]) 
			dist[v] = dist[u] + G[u][v];
	}

	return NULL;
}

void fill_random()
{
	for(int i=0;i<n_vertices;i++)
	{

		for (int j = 0; j < i; ++j)
		{
			G[i][j] = G[j][i] = rand()%500;
			
 		}
 		G[i][i] = 0;
	}
	
}


void main(int argc, char const *argv[])
{
	n_threads =  atoi(argv[1]);
	n_vertices = atoi(argv[2]);
	printf("%d, %d \n",n_vertices ,n_threads);
	G = (int **)malloc(sizeof(int *)*n_vertices);
	dist = (int *)malloc(n_vertices* sizeof(int));
	parent = (int *)malloc(n_vertices* sizeof(int));
	sptSet = (int *)malloc(n_vertices* sizeof(int));
	arr = (int *)malloc(n_threads* sizeof(int));
		
	for (int i = 0; i < n_threads; ++i)
	{
		arr[i] = i;
	}
	
	for (int i = 0; i < n_vertices; ++i)
	{
		G[i] = (int *)malloc(n_vertices* sizeof(int));
		
		dist[i] = INT_MAX;
		sptSet[i] = 0;
		parent[i] = -1;

		//for (int j = 0; j < n_vertices; ++j)
		//scanf("%d",&G[i][j]);
			
	}

	fill_random();

	dist[0] = 0;	 
	parent[0] = -1; // First node is always root of MST 
	
	struct timeval time_start;
	struct timeval time_end;

	gettimeofday(&time_start, NULL);

	// Find shortest path for all vertices 
	for (int count = 0; count < n_vertices-1; count++) 
	{ 
		// Pick the minimum distance vertex from the set of vertices not 
		// yet processed. u is always equal to src in the first iteration. 
		u = minDistance(); 

		// Mark the picked vertex as processed 
		sptSet[u] = 1; 

		pthread_t threads[n_threads];
		
		for (int i = 0; i < n_threads; ++i)
		{
			pthread_create(&threads[i],NULL,updateDistances,&arr[i]);
		//	printf("%s\n",strerror(errno) );
		}
		for (int i = 0; i < n_threads; ++i)
		{
			pthread_join(threads[i],NULL);
		}
	} 

	// print the constructed distance array 
	gettimeofday(&time_end, NULL);


long long execution_time = 1000000LL
		* (time_end.tv_sec  - time_start.tv_sec)
		+ (time_end.tv_usec - time_start.tv_usec);+

	printf("Finished calculating shortest paths in %lldµ seconds.\n\n",execution_time);

	//printSolution(); 
}
