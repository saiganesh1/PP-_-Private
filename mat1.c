#include <myhead.h>
#define size 1024
	int A[size][size]; //= {{0}};
	int B[size][size]; //= {{0}};
	int C[size][size]; //= {{0}};
void main(int argc, char const *argv[])
{
	
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			C[i][j] = 0;
			A[i][j] = 0;
			B[i][j] = 0;
		}
	}
	printf("uyhcds\n");
	for (int i = 0; i < size; ++i)
	{
		for (int k = 0; k < size; ++k)
		{
			for (int j = 0; j < size; ++j)
			{
				C[i][j] += A[i][k]*B[k][j];
			}
		}
	}
}