#include<myheader.h>
#define SIZE 15
int g[SIZE][SIZE];
// {
//      	            {0, 1, 1, 1, 0 ,0 ,0}, 
//                     {1, 0, 1, 0, 0, 0, 0}, 
//                     {1, 1, 0, 1, 1, 0, 0}, 
//                     {1, 0, 1, 0, 1, 0, 0}, 
//                     {0, 0, 1, 1, 0, 0, 0},
//                     {0, 0, 0, 0, 0, 0, 1},
//                     {0, 0, 0, 0, 0, 1, 0}

//                     }; 
int disj[SIZE];

int thread_no;

int getroot(int i)
{
	while(disj[i]!=i)
	{
		i=disj[i];
	}

	return i;
}


void initialise()
{
	int i,j;
    srand(time(0));
	for(i=0;i<SIZE;i++)
	{
		for(j=0;j<i;j++)
		{
			g[i][j]=g[j][i]=rand()%2;
		}
		g[i][i]=0;
	}
}

void dsu_union(int i,int j)
{
	int k1=getroot(i);
	int k2=getroot(j);
	disj[k2]=k1;
}

void dfs2(int src,int* visit)
{
	visit[src]=1;
    int i;
	for(i=0;i<SIZE;i++)
	{
		if(g[src][i]==1 && visit[i]==0)
		{
			dsu_union(src,i);
			dfs2(i,visit);
		}
	}
}

void dfs1(int src,int *visit)
{
	visit[src]=1;
	int i;

	for(i=0;i<SIZE;i++)
	{
		if(g[src][i]==1 && visit[i]==0)
		{
			int r1=getroot(src);
			int r2=getroot(i);

			if(r1!=r2)
			  dsu_union(src,i);
            
            dfs1(i,visit);
		}
	}
}

void* dfs(void* arg)
{
   int para = (*(int*)arg);
   int strt = (para*SIZE)/thread_no;
   int end = ((para+1)*SIZE)/thread_no;

   if(para==thread_no-1)   //taking care of the last split if size % n_thread != 0
   	end=SIZE;

   int i;
   int visit[SIZE];
   memset(visit,0,sizeof(visit));

   for(i=strt;i<end;i++)
   {
      if(visit[i] == 0)
      {
      	if(para==0)
      		dfs2(i,visit);
      	else
      	    dfs1(i,visit);
      }
   }

}

int sear(int* compo,int end,int cur)
{
	int i;
	for(i=0;i<end;i++)
	{
		if(compo[i]==cur)
			return 1;
	}   

	return 0;
}

int main(int argc,char* argv[])
{
    if(argc<2)
    {
    	fprintf(stderr,"%s\n","please enter no of threads");
    	return 1;
    }
     
    initialise();
    int i,j;
    printf("graph is\n");

    for(i=0;i<SIZE;i++)
    {
    	for(j=0;j<SIZE;j++)
    		printf("%d ",g[i][j]);
    	printf("\n");
    }
     
   
    thread_no = atoi(argv[1]);
    
 
    for(j=0;j<SIZE;j++)
    {
    	disj[j]=j;
    }
    

    
    pthread_t th[thread_no];
    int arr[thread_no];
    
    for(i=1;i<thread_no;i++)
    {
    	arr[i]=i;
    	pthread_create(&th[i],NULL,dfs,&arr[i]);
    }
   	
   	int k=0;
    dfs((void*)&k);
    
    for(i=1;i<thread_no;i++)
    {
    	pthread_join(th[i],NULL);

    }

    printf("printing connected components\n");

    int compo[SIZE],end=0;
     
    for(i=0;i<SIZE;i++)
    {
    	int cur=getroot(i);
    	if(sear(compo,end,cur)==0)
    	{
    		for(j=i;j<SIZE;j++)
        	{
              if(getroot(j)==cur)
              {
            	printf("%d ",j);
              }

    	    }

    	   printf("\n");
           compo[end++]=cur;
       }
    }
	return 0;
}