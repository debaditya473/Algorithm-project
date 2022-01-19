#include<stdio.h>
#include<stdlib.h>
#include <math.h>

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
void print_path(int* Array, int n, int start, int end)
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
        printf(" %d ->", print[i]);
    }
    printf(" %d\n", print[x-1]);
}

//dijkstra's algorithm including path and without update_val function
int dijkstra(int** Graph, int n, int start, int end, int print)
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

        //we go throught the successors of the node being processed 
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
        print_path(pred_array,n,start,end);
        printf("Distance: %d\n\n", cost_array[end]);
    }
    return cost_array[end];
}

int main()
{
    int n;
    scanf("%d",&n);
    int** Graph = (int**)malloc(n*sizeof(int*));
    for (int i=0;i<n;i++){
        Graph[i] = (int*)malloc(n*sizeof(int));
    }

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++) scanf("%d",&Graph[i][j]);
    }
	
    int distance = dijkstra(Graph,n,0,4, 1);
    
    printf("\n");
     
}
