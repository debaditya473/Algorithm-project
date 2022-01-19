#include<stdio.h>
#include<stdlib.h>

#define INFINITY 1000

void dijkstra(int** Graph, int n, int start)
{
    int* visited, *pred,*distance;
    int** cost;
    int count,mincost, next;

    visited = (int*)malloc(n*sizeof(int));
    pred = (int*)malloc(n*sizeof(int));
    distance = (int*)malloc(n*sizeof(int));

    cost = (int**)malloc(n*sizeof(int*));
    for (int i=0;i<n;i++){
        cost[i] = (int*)malloc(n*sizeof(int));
    }

    for (int i=0;i<n;i++){
        for (int j=0;j<n;j++){
            if (Graph[i][j] == 0){
                cost[i][j] = INFINITY;
            } else{
                cost[i][j] = Graph[i][j];
            }
        }
    }

    /*
    for (int i=0;i<n;i++){
        for (int j=0;j<n;j++){
            printf("%d ", cost[i][j]);
        }
        printf("\n");
    }
    */
    
    for(int i=0;i<n;i++)
	{
		distance[i]=cost[start][i];
		pred[i]=start;
		visited[i]=0;
	}

    distance[start]=0;
	visited[start]=1;
	count=1;

    while (count<n-1){
        mincost = INFINITY;

        for (int i=0;i<n;i++)
        {
            if (distance[i] < mincost && visited[i] == 0)
            {
                mincost = distance[i];
                next = i;
            }

            visited[next]=1;

            for(i=0;i<n;i++)
            {
                if(!visited[i])
                {
                    if(mincost+cost[next][i]<distance[i])
                    {
                        distance[i]=mincost+cost[next][i];
                        pred[i]=next;
                    }
                }		
            }
        }
        count++;
    }

    for(int i=0;i<n;i++)
    {
        if(i!=start)
        {
            printf("\nDistance to node %d=%d",i,distance[i]);
            printf("\nPath=%d",i);
            
            int j=i;
            do{
                j=pred[j];
                printf(" -- %d",j);
            } while(j!=start);
        }
    }
}


int main(){
    int **G,n,u;
	printf("Enter no. of vertices:");
	scanf("%d",&n);
    G = (int**)malloc(n*sizeof(int*));
    for (int i=0;i<n;i++){
        G[i] = (int*)malloc(n*sizeof(int));
    }
	printf("\nEnter the adjacency matrix:\n");
	
	for(int i=0;i<n;i++)
		for(int j=0;j<n;j++)
			scanf("%d",&G[i][j]);
	
	printf("\nEnter the starting node:");
	scanf("%d",&u);
	dijkstra(G,n,u);
	
	return 0;
}