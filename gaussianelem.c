#include<myhead.h>

double **mat;
double *x;

struct argmnt 
{
  int start,end;
  int j,n;
};

void fill_random(int n)
{
	for (int i=0; i < n; i++)
	{

		for (int j = 0; j < n + 1; ++j)
		{
			mat[i][j] = (double)rand();
		}
	}
	
}

void back_sub(int n)
{
  int i,j;
  for(i=n-1;i>=0;i--)
  {
    x[i]=mat[i][n];
    for(j=i+1;j<n;j++)
    {
      x[i]=x[i]-mat[i][j]*x[j];
    }
    x[i]=x[i]/mat[i][i];
  }
  for(i=0;i<n;i++)
  {
    printf("%lf\n",x[i]);
  }
}

void *forw_elem(void *arg1)
{
  int i,j,j1;
  double k;
  struct argmnt *arg=arg1;
  j1=arg->j;
  int n=arg->n;
  for(i=arg->start;i<=arg->end;i++)
  {

    if(i>j1)
    {
      k=mat[i][j1]/mat[j1][j1];
      for(j=0;j<n+1;j++)
      {
        mat[i][j]=mat[i][j]-mat[j1][j]*k;
      }  
    }
  }
}

void partialpivot(int j,int n)
{
    int   i,k,m,rowx;
    double xfac, temp, temp1, amax;
    
    amax = (double) fabs(mat[j][j]) ;
    m = j;
    for (i=j+1; i<n; i++){   /* Find the row with largest pivot */
    	xfac = (double) fabs(mat[i][j]);
    	if(xfac > amax) {amax = xfac; m=i;}
    }

    if(m != j) {  /* Row interchanges */
    	rowx = rowx+1;
    	temp1 = mat[j][n];
    	mat[j][n]  = mat[m][n];
    	mat[m][n]  = temp1;
    	for(k=j; k<n; k++) {
    		temp = mat[j][k];
    		mat[j][k] = mat[m][k];
    	        mat[m][k] = temp;
    	}
    } 
}

int main()
{
  int n,n1,i,j;
  printf("Enter no of threads :");
  scanf("%d",&n);
  
  printf("Size of matrix :");
  scanf("%d",&n1);
  
  printf("Enter matrix :");
  for(i=0;i<n1;i++)
  {
    for(j=0;j<n1+1;j++)
    {
      scanf("%lf",&mat[i][j]);
    }
  }
 
  mat = (double **)malloc(sizeof(double *)*n1);
  for (int i = 0; i < n1; ++i)
  {
		mat[i] = (double *)malloc(sizeof(double)*(n1 + 1)); 
  }
  x = (double *)malloc(sizeof(double)*n1);

  fill_random(n1);
  pthread_t tid[n];
  
  struct argmnt arg[n];
  int k;
  
  	struct timeval time_start;
	struct timeval time_end;
  
  gettimeofday(&time_start, NULL);

  for(j=0;j<n1;j++)
  {
    partialpivot(j,n1);
    int n2=n1/n;
    k=0;
    n2=1;
    for(i=0;i<n;i++)
    {
        arg[i].n=n1;
        arg[i].j=j;
        arg[i].start=k;
        if(i<n-1)
        {
          arg[i].end=k+n2-1;
        }
        else
        {
          arg[i].end=n1-1;
        }
        k=k+n2;
        pthread_create(&tid[i],NULL,forw_elem,(void *)&arg[i]);
    }
    
    for(i=0;i<n;i++)
    { 
        pthread_join(tid[i],NULL);
    }
  }
  
  back_sub(n1);

  gettimeofday(&time_end, NULL);

  long long execution_time = 1000000LL
		* (time_end.tv_sec  - time_start.tv_sec)
		+ (time_end.tv_usec - time_start.tv_usec);

	printf(" Finished calculating shortest paths in %lld micro seconds.\n\n",
		execution_time);
  return 0;
}
