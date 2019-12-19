#include <myhead.h>
int n_vertices;
int n_threads;
int *mstSet;
int *arr;
int **G;
int *key;
int *parent;
int u;
extern int errno;

// A utility function to find the vertex with 
// minimum key value, from the set of vertices 
// not yet included in MST 
int minKey() 
{ 
	// Initialize min value 
	
	int min = INT_MAX, min_index; 

	for (int v = 0; v < n_vertices; v++)
	{

		if (mstSet[v] == 0 && key[v] < min) 
		{
			min = key[v], min_index = v; 
		}
	}
	return min_index; 
}


// A utility function to print the 
// constructed MST stored in parent[] 
int printMST() 
{ 
printf("Edge \tWeight\n"); 
for (int i = 1; i < n_vertices; i++) 
	printf("%d - %d \t%d \n", parent[i], i, G[i][parent[i]]); 
} 


void * updateDistances(void *arg)
{
	int id = *(int *)arg;
	int start = id * n_vertices / n_threads;
	int end   = (id + 1) * n_vertices / n_threads;
	printf("start = %d ; end =  %d \n", start,end-1);
	for (int v = start; v < end; ++v )
	{
		if (G[u][v] && mstSet[v] == 0 && G[u][v] < key[v])
		{
			parent[v] = u;
			key[v] = G[u][v]; 
		}
	}

	return NULL;
}

void fill_random()
{
	for(int i=0;i<n_vertices;i++)
	{

		for (int j = 0; j < i; ++j)
		{
			G[i][j] = G[j][i] = 1 + rand()%500;
			
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
	key = (int *)malloc(n_vertices* sizeof(int));
	parent = (int *)malloc(n_vertices* sizeof(int));
	mstSet = (int *)malloc(n_vertices* sizeof(int));
	arr = (int *)malloc(n_threads* sizeof(int));
		
	for (int i = 0; i < n_threads; ++i)
	{
		arr[i] = i;
	}
	
	for (int i = 0; i < n_vertices; ++i)
	{
		G[i] = (int *)malloc(n_vertices* sizeof(int));
		
		key[i] = INT_MAX;
		mstSet[i] = 0;
		parent[i] = -1;

		//for (int j = 0; j < n_vertices; ++j)
		//scanf("%d",&G[i][j]);
			
	}

	fill_random();

		


	key[0] = 0;	 
	parent[0] = -1; // First node is always root of MST 
	
	struct timeval time_start;
	struct timeval time_end;

	gettimeofday(&time_start, NULL);


	// The MST will have V vertices 
	for (int count = 0; count < n_vertices-1; count++) 
	{ 
		// Pick the minimum key vertex from the 
		// set of vertices not yet included in MST 
		u = minKey(); 

		// Add the picked vertex to the MST Set 
		mstSet[u] = 1; 

	
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
	gettimeofday(&time_end, NULL);

long long execution_time = 1000000LL
		* (time_end.tv_sec  - time_start.tv_sec)
		+ (time_end.tv_usec - time_start.tv_usec);

	printf(" Finished calculating shortest paths in %lld micro seconds.\n\n",
		execution_time);

	//printMST();
}
