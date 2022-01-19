#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include<bits/stdc++.h>
using namespace std;
using namespace cv;

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

//creates and returns image of type Mat with marked city_centres and houses
Mat generate_city(int x, int y, int* houses, int cnt_H, int* city_centres, int cnt_CC)
{
	Mat grid(x,y,CV_8UC3,Scalar(255,255,255));
	for (int i=0;i<cnt_H;i++)
	{
		grid.at<Vec3b>(((houses[i]/y)),((houses[i]%x)))[0] = 0;

	}
	for (int i=0;i<cnt_CC;i++)
	{
		grid.at<Vec3b>(city_centres[i]/y,city_centres[i]%x)[1] = 0;
	}
	

	Mat city(5*x,5*y,CV_8UC3,Scalar(255,255,255));
    for (int i=0;i<x;i++)
    {
    	for (int j=0;j<y;j++)
    	{
    		city.at<Vec3b>(5*i+2,5*j+2) = grid.at<Vec3b>(i,j);
    	}
    }

	return city;
}

//utility function to add an edge between two given nodes
void add_edge(int point1_x, int point1_y, int point2_x, int point2_y, Mat* city)
{
	int node1_x = (point1_x*5+2);
	int node1_y = (point1_y*5+2);
	int node2_x = (point2_x*5+2);
	int node2_y = (point2_y*5+2);

	if (node1_x == node2_x)
	{
		if (node1_y < node2_y)
		{
			for (int i=node1_y+1;i<node2_y;i++){
				city->at<Vec3b>(node1_x,i)[2]= 0;
			}
		}
        else{
			for (int i=node2_y+1;i<node1_y;i++){
				city->at<Vec3b>(node1_x,i)[2]= 0;
			}
		}
	}
	if (node1_y == node2_y)
	{
		if (node1_x < node2_x)
		{
			for (int i=node1_x+1;i<node2_x;i++){
				city->at<Vec3b>(i,node1_y)[2]= 0;
			}
		}
        else{
			for (int i=node2_x+1;i<node1_x;i++){
				city->at<Vec3b>(i,node1_y)[2]= 0;
			}
		}
	}
}

int main()
{
	int i,j;
    int x,y;
    printf("Enter x and y:");
    scanf("%d %d",&x,&y);


    int **adjacency = (int**)malloc(((x*y)+1) * sizeof(int *));//1 indexing
    for (i=0; i<=(x*y); i++)
    {
        adjacency[i] = (int *)malloc(((x*y)+1) * sizeof(int));//1 indexing
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

    //printing adjacency matrix
    printf("\n Adjacency matrix:\n");
    for(i=0;i<(x*y);i++)
    {
        for(j=0;j<(x*y);j++)
        {
            printf("%d ",adjacency[i][j]);

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

	Mat city = generate_city(x,y,houses,cnt_houses,city_centres,cnt_city_centres);

    //add_edge(5,6,4,6,&city);
    //add_edge(4,6,4,7,&city);

    add_edge(0,0,0,1,&city);

	namedWindow("WIN",WINDOW_NORMAL);
	imshow("WIN",city);
	waitKey(0);

    
    return 0;
    
}