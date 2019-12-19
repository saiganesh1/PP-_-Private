#include<myhead.h>
 
void swap(int* a, int* b) 
{ 
	int t = *a; 
	*a = *b; 
	*b = t; 
} 
int *arr;

typedef struct {
	int low;
	int high;
} sarg;

void fillArray(int n)
{
	for (int i = 0; i < n; ++i)
	{
		arr[i] = rand()%2000;
	}
}

int partition (int arr[], int low, int high) 
{ 
	int pivot = arr[high]; // pivot 
	int i = (low - 1); // Index of smaller element 

	for (int j = low; j <= high- 1; j++) 
	{ 
		// If current element is smaller than or 
		// equal to pivot 
		if (arr[j] <= pivot) 
		{ 
			i++; // increment index of smaller element 
			swap(&arr[i], &arr[j]); 
		} 
	} 
	swap(&arr[i + 1], &arr[high]); 
	return (i + 1); 
} 


void* quickSort(void *arg) 
{ 
	int low = ((sarg *)arg)->low;
    int high = ((sarg *)arg)->high;

	if (low < high) 
	{ 
		int pi = partition(arr, low, high); 

        sarg arg1, arg2;
        pthread_t thread1, thread2;
        
        arg1.low = low;
        arg1.high = pi - 1;
      
        arg2.low = pi + 1;
        arg2.high = high;

        #pragma omp parallel sections
		{
			#pragma parallel section
			{
				quickSort(arg1);
			}

			#pragma parallel section
			{
				quickSort(arg2);
			}
		}

	} 
} 

/* Function to print an array */
void printArray(int arr[], int size) 
{ 
	int i; 
	for (i=0; i < size; i++) 
		printf("%d ", arr[i]); 
	printf("n"); 
} 

void main(int argc, char const *argv[])
{
	int n = atoi(argv[1]);
	arr = (int *)malloc(sizeof(int)*n);
	fillArray(n);


	//Enabling the Nesyed Parallelism
	omp_set_nested(1);

	sarg arg;
	arg.low = 0;
	arg.high = n-1;
	quickSort(&arg); 
	
	printf("Sorted array: n"); 
	printArray(arr, n); 



} 
