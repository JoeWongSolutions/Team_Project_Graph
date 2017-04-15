

#ifndef STDLIBS
#define STDLIBS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<limits.h>
#endif

#include"hashtable.h"

#ifndef MAX_NAME
#define MAX_NAME 20
#endif

#define PARENT(i) (i-1)/2
#define LEFTCHILD(i) 2*i+1
#define RIGHTCHILD(i) 2*i+2



typedef struct MinHeapNode {
	char name[MAX_NAME];
	unsigned int weight;
	struct MinHeapNode* parent;
}MinHeapNode;

typedef struct posNode { //Position node used for pos array
	char name[MAX_NAME];
	unsigned int index;
}posNode;

typedef struct MinHeap {
	unsigned int size; //number of nodes currently in heap
	unsigned int capacity;
	posNode *pos;  //a pointer to an array holding positions of each node
	MinHeapNode** array; //a pointer to the array used as a heap 
}MinHeap;

MinHeapNode* new_MinHeapNode(char* name, unsigned int weight);
MinHeap* new_MinHeap(unsigned int capacity);
void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b);
unsigned int hashGenerator(MinHeap* heap, char* name);
unsigned int hashFinder(MinHeap* heap, char* name);
void minHeapify(MinHeap* heap, unsigned int idx);
int isEmpty(MinHeap* heap);
MinHeapNode* extractMin(MinHeap* heap);
void decreaseKey(MinHeap* heap, char* name, unsigned int weight);
bool inMinHeap(MinHeap* heap, char* name);
void destroy_heap(MinHeap* heap, unsigned int size);
