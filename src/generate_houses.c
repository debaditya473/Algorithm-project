#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<math.h>

void create_houses(int** A, int n)
{
	int i,j;
    // A[i][j]=(rand() % (upper – lower + 1)) + lower ----> gives edge weight = a random number between 1 and 100.
    int upper= 100;
    int lower=1;
    for(i=0;i< n ;i++)
    {
        for(j=0;j<i;j++)
        {
            A[i][j]= (rand() % (upper - lower + 1)) + lower;
            A[j][i]= A[i][j];
        }
    }
}

void gen_input(int n, int ***Adj)
{
    //
    int **adjacency = (int**)malloc((n) * sizeof(int *));//0 indexing
    for (int i=0; i<(n); i++)
    {
        adjacency[i] = (int *)malloc((n)* sizeof(int));//0 indexing
    }

    for(int i=0;i<(n);i++)
    {
        for(int j=0;j<(n);j++)
        {
            adjacency[i][j]=0;
        }
    }

    create_houses(adjacency,n);

    *Adj = adjacency;
}

int main()
{
	int n;
    printf("Enter Number of Houses = ");
    scanf("%d",&n);

    int **Adjacency;
    gen_input(n, &Adjacency);


    //printing adjacency matrix
    printf("\n ++++ Adjacency matrix ++++\n");
    for(int i=0;i<(n);i++)
    {
        for(int j=0;j<(n);j++)
        {
            printf("%3d ",Adjacency[i][j]);

        }
        printf("\n");
    }
    
    return 0;
    
}