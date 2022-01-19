#include<stdio.h>
#include<stdlib.h>
#include <math.h>
  

//stores a node and the minimum edge
//weight from this node to MST
typedef struct PQ{
    int edge_weight;
    int node;
}PQ;

typedef struct Heap{
    PQ *Array;
    int capacity;
    int size;
}Heap;

// Prototype of a utility function to swap two numbers
void swap(PQ *x, PQ *y);

//function that allows you to build the heap 
Heap* build_heap(int n)
{
    Heap *heap = (Heap *)malloc(sizeof(Heap));
    heap->Array = (PQ *)malloc(sizeof(PQ)*n);
    heap->capacity = n;
    heap->size = 0;
    return heap;
}

//utility heap functions
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
        return (PQ){__INT16_MAX__, __INT16_MAX__};
    return heap->Array[0];
}

//heapify or propagate down function
void heapify(int i, Heap *heap)
{
    int l = left_child(i);
    int r = right_child(i);
    int min = i;

    if(l < heap->size && heap->Array[l].edge_weight < heap->Array[i].edge_weight)
        min = l;
    if (r < heap->size && heap->Array[r].edge_weight < heap->Array[min].edge_weight)
        min = r;
    
    if (min != i)
    {
        swap(&(heap->Array[i]), &(heap->Array[min]));
        heapify(min, heap);
    }
    return;
}

//insert function 
void heap_insert(Heap *heap, int weight, int node)
{
    if(heap->size == heap->capacity){
        printf("\nOverflow: Could not insertKey\n");
        return;
    }

    heap->size += 1;
    int i = heap->size -1;
    heap->Array[i].edge_weight = weight;
    heap->Array[i].node = node;

    while (i != 0 && heap->Array[i].edge_weight 
            < heap->Array[parent(i)].edge_weight)
    {
        swap(&(heap->Array[i]), &(heap->Array[parent(i)]));
        i = parent(i);
    }
    return;
}

//removes minimum from heap and returns
PQ heap_remove_min(Heap *heap)
{
    if (heap->size <= 0)
        return (PQ){__INT16_MAX__, __INT16_MAX__};

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

//function to update the value of a node, not used in algorithm
void heap_update_val(int node, int new_val, Heap *heap)
{
    int i;
    for(i = 0; i < heap->size; i++)
    {
        if (heap->Array[i].node == node){
            break;
        }
    }
    heap->Array[i].edge_weight = new_val;

    while (i != 0 && heap->Array[i].edge_weight < heap->Array[parent(i)].edge_weight)
    {
        swap(&(heap->Array[i]), &(heap->Array[parent(i)]));
        i = parent(i);
    }
}

//utility function to print heap
void print_heap(Heap *heap)
{
    printf("\n");
    for (int i = 0; i < heap->size; i++)
    {
        printf("Weight: %d\n", heap->Array[i].edge_weight);
        printf("Node1: %d\n", heap->Array[i].node);
    }
    printf("\n");
}

void swap(PQ *x, PQ *y) 
{ 
    PQ temp = *x; 
    *x = *y; 
    *y = temp; 
}

void prims(int** Graph, int n, int start)
{
    Heap *heap = build_heap(n*n);
    
    //tracks whether a node is in the MST
    int *inMST = (int *)malloc(n*sizeof(int));
    for (int i = 0; i < n; i++){
        inMST[i] = 0;
    }
    inMST[start] = 1;


    //tracks parent nodes
    int *parent = (int *)malloc(n*sizeof(int));
    for (int i = 0; i < n; i++){
        parent[i] = start;
    }


    //pushing all vertices into queue (with cost = distance from start)
    for (int i = 0; i < n; i++)
    {
        if(i==start) continue;
        if (Graph[start][i] == 0){
            heap_insert(heap, __INT16_MAX__, i);
            continue;
        }
        
        heap_insert(heap, Graph[start][i], i);
    }

    //maintains current lowest key from node to MST
    int *key = (int *)malloc(n*sizeof(int));
    for (int i = 0; i < n; i++){
        if (Graph[start][i] == 0) {
            key[i] = __INT16_MAX__;
            continue;
        }
        key[i] = Graph[start][i];
    }    

    int total_cost = 0;
    int Vertices_in_MST = 1;

    //until queue not empty
    while(Vertices_in_MST != n)
    {
        PQ min;     //minimum distance vertex from MST
        
        do{
            min = heap_remove_min(heap);
        }while(inMST[min.node] == 1);
        //we check whether this node is already a part of MST
        //duplicates may exist in queue

        total_cost += min.edge_weight;
        inMST[min.node] = 1;
        //added to MST
        Vertices_in_MST += 1;

        for(int i = 0; i<n; i++)
        {
            if (Graph[min.node][i] == 0) continue;
            //we only look at successors - can be made faster using an adjacency list

            if(Graph[min.node][i] < key[i] && inMST[i] == 0)
            {
                //heap_update_val(i, Graph[min.node][i], heap);
                //update val not used as it takes O(n) time, instead duplicate values added
                heap_insert(heap, Graph[min.node][i], i);
                key[i] = Graph[min.node][i];
                parent[i] = min.node;
                //if the node is removed, the parent will be min.node, as it has the shortest link
            }
        }
    }

    printf("\nCost = %d\n", total_cost);
    printf("Node - Parent - Edge Cost\n");
    for(int i = 0; i< n; i++)
    {
        if(i == start) continue;
        printf("%2d\t%2d\t   %2d\n", i+1, parent[i]+1, Graph[i][parent[i]]);
    }
    printf("\n");
}

//functions to generate adjacency matrices randomly
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

//IO function
void automated()
{
    int n;
    printf("Enter Number of Houses = ");
    scanf("%d",&n);

    int **Graph;
    gen_input(n, &Graph);


    //printing adjacency matrix
    printf("\n ++++ Adjacency matrix ++++\n");
    for(int i=0;i<(n);i++)
    {
        for(int j=0;j<(n);j++)
        {
            printf("%3d ",Graph[i][j]);

        }
        printf("\n");
    }

    prims(Graph, n, 0);
}

//IO function
void take_user_input()
{
    int n;
    printf("Enter Number of Houses = ");
    scanf("%d", &n);
    printf("Adjacency Matrix = \n");

    int **Graph = (int **)malloc(n*sizeof(int *));
    for(int i =0; i<n;i++){
        Graph[i] = (int *)malloc(n*sizeof(int));
    }
    
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++) scanf("%d",&Graph[i][j]);
    }

    prims(Graph, n, 0);
}


int main()
{
    int choice;
    printf("Enter 1 to Enter Input [Number of nodes and Adjacency Matrix is required\n");
    printf("Enter 2 to randomly generate a graph of given size\n");
    scanf("%d", &choice);

    if(choice == 1) take_user_input();
    if(choice == 2) automated();

    return 0;
}



