#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <chrono>
#include <thread>
using namespace std;
using namespace cv;

typedef struct PQ{
    int node;
    int min_dist;
    int predecessor;
}PQ;

typedef struct Heap{
    PQ *Array;
    int capacity;
    int size;
}Heap;

// Prototype of a utility function to swap two numbers
void swap(PQ *x, PQ *y);

//Prototypes for visualisation functions
Mat generate_city(int x, int y, int* houses, int cnt_H, int* city_centres, int cnt_CC);
void add_edge(int point1_x, int point1_y, int point2_x, int point2_y, Mat* city);
void highlight_node(int x, int y, int node, Mat* city);

Heap* build_heap(int n)
{
    Heap *heap = (Heap *)malloc(sizeof(Heap));
    heap->Array = (PQ *)malloc(sizeof(PQ)*n);
    heap->capacity = n;
    heap->size = 0;
    return heap;
}

int left_child(int i){
    return 2*i+1;
}

int right_child(int i){
    return 2*i+2;
}

int parent(int i){
    return (i-1)/2;
}

PQ heap_getMin(Heap *heap){
    if (heap->size <= 0)
        return (PQ){__INT16_MAX__, __INT16_MAX__, __INT16_MAX__};
    return heap->Array[0];
}

void heapify(int i, Heap *heap)
{
    int l = left_child(i);
    int r = right_child(i);
    int min = i;

    if(l < heap->size && heap->Array[l].min_dist < heap->Array[i].min_dist)
        min = l;
    if (r < heap->size && heap->Array[r].min_dist < heap->Array[min].min_dist)
        min = r;
    
    if (min != i)
    {
        swap(&(heap->Array[i]), &(heap->Array[min]));
        heapify(min, heap);
    }
    return;
}

void heap_insert(Heap *heap, int dist, int node, int pred)
{
    if(heap->size == heap->capacity){
        printf("\nOverflow: Could not insertKey\n");
        return;
    }

    heap->size += 1;
    int i = heap->size -1;
    heap->Array[i].min_dist = dist;
    heap->Array[i].node = node;
    heap->Array[i].predecessor = pred;

    while (i != 0 && heap->Array[i].min_dist < heap->Array[parent(i)].min_dist)
    {
        swap(&(heap->Array[i]), &(heap->Array[parent(i)]));
        i = parent(i);
    }
    return;
}

PQ heap_remove_min(Heap *heap)
{
    if (heap->size <= 0)
        return (PQ){__INT16_MAX__, __INT16_MAX__, __INT16_MAX__};

    if (heap->size == 1)
    {
        heap->size = 0;
        return heap->Array[0];
    }
    
    PQ root = heap->Array[0];
    heap->Array[0] = heap->Array[heap->size-1];
    heap->size -= 1;

    heapify(0, heap);

    return root;
}

void print_heap(Heap *heap)
{
    printf("\n");
    for (int i = 0; i < heap->size; i++)
    {
        printf("Node: %d\n", heap->Array[i].node);
        printf("Dist: %d\n", heap->Array[i].min_dist);
        printf("Pred_Node: %d\n", heap->Array[i].predecessor);
    }
    printf("\n");
}

void swap(PQ *x, PQ *y) 
{ 
    PQ temp = *x; 
    *x = *y; 
    *y = temp; 
}

//function to print the dijkstra's shortest path
void print_path(int* Array, int n, int start, int end, Mat* city)
{
    printf("\nPath => ");
    
    int x = 1;
    int temp = end;
    while(1)
    {
        if (temp == start) break;
        x += 1;
        temp = Array[temp];
    }
    int *print = (int *)malloc(x*sizeof(int));

    temp = end;
    for(int i = 0; i < x; i++)
    {
        print[x-1-i] = temp;
        temp = Array[temp];
    }

    for(int i = 0; i < x-1; i++)
    {
        //printf(" %d ->", print[i]);
        add_edge(print[i]/10, print[i]%10, print[i+1]/10, print[i+1]%10, city);
    }
}

//dijkstra's algorithm including path
int dijkstra(int** Graph, int n, int start, int end, int print, Mat* city)
{
    if (end == start)
    {
        printf("No path exixts if start and goal nodes are the same\n");
    }

    Heap *heap = build_heap((n*n));

    int* visited = (int*)malloc(n*sizeof(int));
    for (int i=0;i<n;i++)
    {
        visited[i] = -1;
    }

    int* cost_array = (int*)malloc(n*sizeof(int));
    for (int i=0;i<n;i++)
    {
        cost_array[i] = __INT16_MAX__;
    }

    int* pred_array = (int*)malloc(n*sizeof(int));
    for (int i=0;i<n;i++)
    {
        pred_array[i] = -1;
    }

    for (int i=0;i<n;i++)
    {
        if (i!=start) heap_insert(heap,__INT16_MAX__,i,-1);
    }
    heap_insert(heap,0,start,-1);

    while (1)
    {
        PQ min;
        //minimum distance node is removed
        do{
            min = heap_remove_min(heap);
        }while(visited[min.node] == 1);
        //we check whether this node has already been processed, this is possible due to duplicates

        if (min.node == end)
        {
            break;
        }
        //if node being removed is the goal node, we have our solution

        //we go through the successors of the node being processed 
        for (int i=0;i<n;i++)
        {
            if (Graph[min.node][i] != 0 && i!=min.node && visited[i] != 1)
            {
                if (min.min_dist + Graph[min.node][i] < cost_array[i])
                {
                    heap_insert(heap,Graph[min.node][i]+min.min_dist,i,min.node);
                    visited[i]= 0;
                    cost_array[i] = Graph[min.node][i]+min.min_dist;
                    pred_array[i] = min.node;
                }
            }
        }
        visited[min.node] = 1;
        //this node has been processed
    }

    //print function if needed
    if (print == 1){
        print_path(pred_array,n,start,end, city);
        printf("Distance: %d\n\n", cost_array[end]);
    }
    return cost_array[end];
}

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

void findGoalNode(int **Graph, int *H, int *CC, int cnt_H, int cnt_CC, int n, Mat* city)
{
    int min_node;
    int min_distance = __INT32_MAX__;
    int furthest_house = H[0];
    int furthest_house_dist = 0;
    int total_distance;

    for (int i = 0; i< cnt_CC; i++)
    {
        total_distance = 0;
        for(int j = 0; j < cnt_H; j++)
        {
            int distance = dijkstra(Graph, n, CC[i], H[j], 0, city);
            total_distance += distance;
            if(distance > furthest_house_dist){
                furthest_house_dist = distance;
                furthest_house = H[j];
            } 
        }
        if (total_distance < min_distance){
            min_distance = total_distance;
            min_node = CC[i];
        }
    }

    printf("---------------------\n");

    printf("\nSelected City Centre: %d\n", min_node);

    namedWindow("WIN0",WINDOW_NORMAL);
    imshow("WIN0",*city);
    waitKey(1000);

    //std::this_thread::sleep_for(2000ms);
    highlight_node(10,10,min_node,city);

    printf("Total Distance to all Houses from this Centre: %d\n", total_distance);

    //print the path to the furthest house
    printf("Furthest House: %d\n", furthest_house);

    namedWindow("WIN2",WINDOW_NORMAL);
    imshow("WIN2",*city);
    waitKey(1000);

    //std::this_thread::sleep_for(2000ms);
    int distance = dijkstra(Graph, n, min_node, furthest_house, 1, city);
}

void automated()
{
    int i,j;
    int x,y;
    printf("Enter x and y: ");
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
    printf("Number of City Centres: %d",cnt_city_centres);
    printf("\n");
    for(i=0;i<cnt_city_centres;i++)
    {
        printf("%d ",city_centres[i]);
    }
    printf("\n");
    
    //printing array of houses (number of houses = cnt_houses)
    printf("\n");
    printf("Number of Houses: %d",cnt_houses);
    printf("\n");
    for(i=0;i<cnt_houses;i++)
    {
        printf("%d ",houses[i]);
    }
    printf("\n");

    Mat city = generate_city(x,y,houses,cnt_houses,city_centres,cnt_city_centres);

    findGoalNode(adjacency, houses, city_centres, cnt_houses, cnt_city_centres, x*y, &city);
    //runs Djikstras

    namedWindow("WIN",WINDOW_NORMAL);
    imshow("WIN",city);
    waitKey(0);

}

//creates and returns image of type Mat with marked city_centres and houses
Mat generate_city(int x, int y, int* houses, int cnt_H, int* city_centres, int cnt_CC)
{
    Mat grid(x,y,CV_8UC3,Scalar(255,255,255));
    for (int i=0;i<cnt_H;i++)
    {
        grid.at<Vec3b>(((houses[i]/y)),((houses[i]%x)))[0] = 44;
        grid.at<Vec3b>(((houses[i]/y)),((houses[i]%x)))[1] = 197;
        grid.at<Vec3b>(((houses[i]/y)),((houses[i]%x)))[2] = 245;

    }
    for (int i=0;i<cnt_CC;i++)
    {
        grid.at<Vec3b>(city_centres[i]/y,city_centres[i]%x)[0] = 0;
        grid.at<Vec3b>(city_centres[i]/y,city_centres[i]%x)[1] = 0;
        grid.at<Vec3b>(city_centres[i]/y,city_centres[i]%x)[2] = 255;
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

//utility function to highlight the goal node
void highlight_node(int x, int y, int node, Mat* city)
{
    city->at<Vec3b>(5*(node/y)+2,5*(node%x)+2)[0] = 0;
    city->at<Vec3b>(5*(node/y)+2,5*(node%x)+2)[1] = 255;
    city->at<Vec3b>(5*(node/y)+2,5*(node%x)+2)[2] = 0;
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
                city->at<Vec3b>(node1_x,i)[0]= 155;
                city->at<Vec3b>(node1_x,i)[1]= 0;
                city->at<Vec3b>(node1_x,i)[2]= 0;
            }
        }
        else{
            for (int i=node2_y+1;i<node1_y;i++){
                city->at<Vec3b>(node1_x,i)[0]= 155;
                city->at<Vec3b>(node1_x,i)[1]= 0;
                city->at<Vec3b>(node1_x,i)[2]= 0;

            }
        }
    }
    if (node1_y == node2_y)
    {
        if (node1_x < node2_x)
        {
            for (int i=node1_x+1;i<node2_x;i++){
                city->at<Vec3b>(i,node1_y)[0]= 155;
                city->at<Vec3b>(i,node1_y)[1]= 0;
                city->at<Vec3b>(i,node1_y)[2]= 0;
            }
        }
        else{
            for (int i=node2_x+1;i<node1_x;i++){
                city->at<Vec3b>(i,node1_y)[0]= 155;
                city->at<Vec3b>(i,node1_y)[1]= 0;
                city->at<Vec3b>(i,node1_y)[2]= 0;
            }
        }
    }
}

int main()
{
    int wait;
    printf("Enter Any Letter/Number to begin\n");
    scanf("%d", &wait);
    automated();
}