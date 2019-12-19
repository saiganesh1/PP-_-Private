#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<fcntl.h>

double mat[100][100];

double x[100];

struct argmnt{
 int n1,val;
 int start,end;
};

void *back_sub(void *val1)
{
  int i,j; 
  struct argmnt *arg=val1;
  int N=arg->n1;
  i=arg->val;
  for(j=arg->start;j<=arg->end;j++)
  {
     x[i] -= mat[i][j]*x[j];
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
  
  struct argmnt arg[n];
  int k1,k=n-1;
  int n2;
  
  pthread_t t[n];
  for(j=n1-1;j>=0;j--)
  {
    x[j]=mat[j][n1];
    k1=j+1;
    if(n1-k1>=n)
    {
      n2=(n1-k1)/n;
      i=0;
      k=j+1;
      for(k1=0;k1<n;k1++)
      {
        arg[i].val=j;
        arg[i].n1=n1;
        arg[i].start=k;
        if(k1<n-1)
        {
          arg[i].end=k+n2-1;
        }
        else
        {
          arg[i].end=n1-1;
        }
        k=k+n2;
        pthread_create(&t[i],NULL,back_sub,(void *)&arg[i]);      
        i++;
      }
      
      for(k=0;k<i;k++)
      {
        pthread_join(t[k],NULL);
      }          
    }
    else
    { 
      i=0;
      k1=j+1;
      while(k1<n1)
      {
        arg[i].val=j;
        arg[i].n1=n1;
        arg[i].start=k1;
        arg[i].end=k1;
        k1=k1+1;
        pthread_create(&t[i],NULL,back_sub,(void *)&arg[i]);      
        i++;
      }
      for(k=0;k<i;k++)
      {
        pthread_join(t[k],NULL);
      }
    }
    x[j]=x[j]/mat[j][j];
  }
  
  
  for(i=0;i<n;i++)
  {
     printf("%lf\n",x[i]);
  }
  
  return 0;
}
