#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<time.h>

struct argmnt
{
  int start,end;
  int n,n2;
};

int cand[100],rand1[100],mis[100];
int G[10][10];

pthread_barrier_t t1;

void *maximal_IS(void *val)
{
  int min=-1,temp=0;
  struct argmnt *arg=val;
  int i,j;
 pthread_barrier_wait(&t1);
 while(1)
 {
  for(i=arg->start;i<=arg->end;i++)
  {
     temp=0;
     if(cand[i]==1)
     {
     min=rand1[i];
     for(j=0;j<arg->n;j++)
     {
       if(G[i][j]==1&&cand[j]==1)
       {
           if(min>rand1[j])//checks if all its vertices have less random number
           { temp=1; }
       }
       if(temp==1)
       {
         break;
       }
    }
     if(temp==0)  // if yes add it to MIS set and remove it from cand set
     {
       if(cand[i]==1)
      {
        printf("%d ",i);
        mis[i]=1;
       for(j=0;j<arg->n;j++)
       {
         if(G[i][j]==1)
         {
          cand[j]=0;  }
       }
       cand[i]=0;
      }
     }
    }
  }
  temp=0;
  for(i=arg->start;i<=arg->end;i++) //If there is no candidate set remaining stop
  {
    if(cand[i]==1)
    { temp=1;
      break; }
  } 
  if(temp==0)
  {break;}
 }
}

int main()
{
  int n,m1,m2,n1,m;
  printf("Enter no of vertices : ");
  scanf("%d",&n);mahipal
  printf("Enter no of edges :");
  scanf("%d",&m);
  srand(time(0));
  int i,j=n;
  for(i=0;i<n;i++)
  {
    cand[i]=1;  //initially all vertex are present in candidate sets
    rand1[i]=j;//(rand())%(n+1);  // random number for each vertex
    mis[i]=0;   // maximal independent set is null at start
j--;
  }
  
  for(i=0;i<n;i++)
  {
    for(j=0;j<n;j++)
    {
      G[i][j]=0;
    }
  }
  
  for(i=0;i<m;i++)
  {
    scanf("%d %d",&m1,&m2);  //making edges 
    G[m1][m2]=1;  
    G[m2][m1]=1;
  }
  printf("Enter no of threads : ");
  scanf("%d",&n1);
  
  int n2=n/n1;
  
  struct argmnt arg[n1+1];
  int k=0;
  /*
  for(i=0;i<n;i++)
  { 
    printf("%d ",rand1[i]);
  }
  */

  pthread_t tid[n1+1];
  
  printf("Maximal independent set is :");
  
  pthread_barrier_init(&t1,NULL,n1);
  for(i=0;i<n1;i++)  // loop runs for given no of threads and it initiates starting and ending vertex for each thread
  {
    arg[i].start=k;
    if(i<n1-1)
    {
     arg[i].end=k+n2-1;
    }
    else
    {
     arg[i].end=n-1;
    }
    arg[i].n=n;
    pthread_create(&tid[i],NULL,maximal_IS,(void *)&arg[i]);
    k=k+n2;
  }
  
  for(i=0;i<n1;i++)
  {
    pthread_join(tid[i],NULL);
  }
   
  printf("\n");

  pthread_barrier_destroy(&t1);
  return 0;
}
