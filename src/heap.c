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
        return (PQ){INT_MAX, INT_MAX, INT_MAX};
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
        return (PQ){INT_MAX, INT_MAX, INT_MAX};

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

void heap_update_val(int node, int new_val, int new_pred, Heap *heap)
{
    int i;
    for(i = 0; i < heap->size; i++)
    {
        if (heap->Array[i].node == node){
            break;
        }
    }
    heap->Array[i].min_dist = new_val;
    heap->Array[i].predecessor = new_pred;

    while (i != 0 && heap->Array[i].min_dist < heap->Array[parent(i)].min_dist)
    {
        swap(&(heap->Array[i]), &(heap->Array[parent(i)]));
        i = parent(i);
    }
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


int main()
{
    int n;
    scanf("%d", &n);
    Heap *ptr = build_heap(n);
    
    while(1){
        printf("1. insert \n2. get min \n3. end\n4. remove min\n5. print\n6.Update Val\n");
        scanf("%d", &n);
        if (n == 1){
            int node;
            int val;
            int pred;
            scanf("%d", &node);
            scanf("%d", &val);
            scanf("%d", &pred);
            heap_insert(ptr, val, node, pred);
        }
        if (n == 2){
            PQ min = heap_getMin(ptr);
            printf("Node: %d\n", min.node);
            printf("Dist: %d\n", min.min_dist);
            printf("Pred_Node: %d\n", min.predecessor);
        }
        if (n==3)
            break;
        if (n==4) {
            PQ min = heap_remove_min(ptr);
            printf("Node: %d\n", min.node);
            printf("Dist: %d\n", min.min_dist);
            printf("Pred_Node: %d\n", min.predecessor);
            //printf("%d", );
        }
        if (n == 5){
            print_heap(ptr);
        }
        if (n==6){
            int node;
            int val;
            int pred;
            scanf("%d", &node);
            scanf("%d", &val);
            scanf("%d", &pred);
            heap_update_val(node, val, pred, ptr);
        }
    }

    return 0;
}
