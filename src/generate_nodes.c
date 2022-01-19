#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<math.h>

void create_city(int** A, int* CC, int* H, int*cnt_CC, int*cnt_H, int x, int y)
{
	int i,j;
    // A[i][j]=(rand() % (upper – lower + 1)) + lower ----> gives edge weight = a random number between 1 and 100.
    int cnt_nodes= x*y;
    int upper= 100;
    int lower=1;
    for(i=0;i<cnt_nodes;i++)
    {
        if(((i+1)%y)!=0)//right edge from a node
        {
            A[i][i+1]=(rand() % (upper - lower + 1)) + lower;
            A[i+1][i]=A[i][i+1];
        }
        if((i+1-y)>=1)//upper edge from a node
        {
            A[i][i-y]=(rand() % (upper - lower + 1)) + lower;
            A[i-y][i]=A[i][i-y];
        }
        if((i+1+y)<=cnt_nodes)//lower edge from a node
        {
            A[i][i+y]=(rand() % (upper - lower + 1)) + lower;
            A[i+y][i]=A[i][i+y];
        }
        if((i+1-1)%y!=0)//left edge from a node
        {
            A[i][i-1]=(rand() % (upper - lower + 1)) + lower;
            A[i-1][i]=A[i][i-1];
        }

    }

    int num_cc=cnt_nodes/3;
    int num=0; int temp1=0; int temp2=0;
    for(i=0;i<cnt_nodes;i++)
    {
        num = rand() % 2 + 0;//random number 0 or 1
        if(num_cc>0)
        {
            if(num==0)//house
        {
            H[temp1]=i;
            temp1++;
        }
        else if(num==1)//city-centre
        {
            CC[temp2]=i;
            temp2++;
            num_cc--;
        }
        }
        else
        {
            H[temp1]=i;
            temp1++;
        }
        
        
    }
    //temp1=temp1-1;
    //temp2=temp2-1;
    
    *cnt_H=temp1;
    *cnt_CC=temp2;

}

void gen_input(int x, int y, int ***Adj, int **h, int **cc, int *cnt_CC, int *cnt_H)
{
    int i,j;

    int **adjacency = (int**)malloc((x*y) * sizeof(int *));//0 indexing
    for (i=0; i<(x*y); i++)
    {
        adjacency[i] = (int *)malloc((x*y)* sizeof(int));//0 indexing
    }
        
    for(i=0;i<(x*y);i++)
    {
        for(j=0;j<(x*y);j++)
        {
            adjacency[i][j]=0;
        }
    }


    int* city_centres = (int*)malloc((x*y) * sizeof(int));
    int cnt_city_centres;


    int* houses = (int*)malloc((x*y) * sizeof(int));
    int cnt_houses;


    create_city(adjacency, city_centres, houses, &cnt_city_centres, &cnt_houses,x,y);

    *Adj = adjacency;
    *h = houses;
    *cc = city_centres;
    *cnt_CC = cnt_city_centres;
    *cnt_H = cnt_houses;
}

int main()
{
	int i,j;
    int x,y;
    printf("Enter x and y:");
    scanf("%d %d",&x,&y);

    int **adjacency, *houses, *city_centres, cnt_city_centres, cnt_houses;

    gen_input(x, y, &adjacency, &houses, &city_centres, &cnt_city_centres, &cnt_houses);

    //printing adjacency matrix
    printf("\n+++++ Adjacency matrix +++++\n");
    for(i=0;i<(x*y);i++)
    {
        for(j=0;j<(x*y);j++)
        {
            printf("%3d ",adjacency[i][j]);

        }
        printf("\n");
    }
    
    //printing array of city centres (number of city centres = cnt_city_centres)
    printf("\n");
    printf("Number of city centres:%d",cnt_city_centres);
    printf("\n");
    for(i=0;i<cnt_city_centres;i++)
    {
    	printf("%d ",city_centres[i]);
	}
	printf("\n");
    
    //printing array of houses (number of houses = cnt_houses)
    printf("\n");
    printf("Number of houses:%d",cnt_houses);
    printf("\n");
    for(i=0;i<cnt_houses;i++)
    {
    	printf("%d ",houses[i]);
	}
	printf("\n");



    return 0;
    
}