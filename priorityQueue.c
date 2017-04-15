/*
	Created 4/13/2017
	Author: Joe Wong
	Supplement to Dijkstra.c
*/

#include"priorityQueue.h"


MinHeapNode* new_MinHeapNode(char* name, unsigned int weight){
	MinHeapNode* newNode = (MinHeapNode*)malloc(sizeof(MinHeapNode));
	if(!newNode){
		perror("Not enough memory:");
		return NULL;
	}
	strcpy(newNode->name, name);
	newNode->weight = weight;
	newNode->parent = NULL;
	
	return newNode;
}

MinHeap* new_MinHeap(unsigned int capacity){
	MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
	if(!heap){
		perror("Not enough memory");
		return NULL;
	}
	heap->size = 0;
	heap->capacity = capacity;
	heap->pos = (posNode*)calloc(capacity, sizeof(posNode));
	if(!heap->pos){
		free(heap);
		perror("Not enough memory");
		return NULL;
	}
	heap->array = (MinHeapNode**)calloc(capacity, sizeof(MinHeapNode*));
	if(!heap->array){
		free(heap->pos);
		free(heap);
		perror("Not enough memory");
		return NULL;
	}
	
	return heap;
}

void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b){
	MinHeapNode* temp = *a;
	*a = *b;
	*b = temp;
}

unsigned int hashGenerator(MinHeap* heap, char* name){
	unsigned int size = heap->capacity;
	unsigned int hashKey = SDBM_hash(size, name);

	//while a node exists and the key to the node is not equal to name, generate new hash key
	//******IMPROVEMENT NOTE******
	/*This generator can cause potential clustering. Need to improve this design in future */
	while(heap->pos[hashKey].name && strcmp(heap->pos[hashKey].name, name)){
		hashKey = (hashKey + 1) % size;
	}
	//enter the name into empty slot
	strcpy(heap->pos[hashKey].name, name);

	return hashKey;
}

//Utility function to find the hashKey of a node without editing the name
unsigned int hashFinder(MinHeap* heap, char* name){
	unsigned int size = heap->capacity;
	unsigned int hashKey = SDBM_hash(size, name);

	//while a node exists and the key to the node is not equal to name, generate new hash key
	//******IMPROVEMENT NOTE******
	/*This generator can cause potential clustering. Need to improve this design in future */
	while(heap->pos[hashKey].name && strcmp(heap->pos[hashKey].name, name)){
		hashKey = (hashKey + 1) % size;
	}
	if(heap->pos[hashKey].name){
		return hashKey;
	} else {
		return size;
	}
}

void minHeapify(MinHeap* heap, unsigned int idx){
	unsigned int smallest, left, right;
	smallest = idx;
	left = LEFTCHILD(idx);
	right = RIGHTCHILD(idx);

	if(left < heap->size && heap->array[left]->weight < heap->array[smallest]->weight){
		smallest = left;
	}
	if(right < heap->size && heap->array[right]->weight < heap->array[smallest]->weight){
		smallest = right;
	}

	if(smallest != idx){
		//Nodes to be swapped in the heap array
		MinHeapNode* smallestNode = heap->array[smallest];
		MinHeapNode* idxNode = heap->array[idx];

		//swap positions
		heap->pos[hashGenerator(heap, smallestNode->name)].index = idx;
		heap->pos[hashGenerator(heap, idxNode->name)].index = smallest;

		//swap nodes
		swapMinHeapNode(&heap->array[smallest], &heap->array[idx]);

		minHeapify(heap, smallest);
	}
}

int isEmpty(MinHeap* heap){
	return heap->size == 0;
}

MinHeapNode* extractMin(MinHeap* heap){
	if(isEmpty(heap)){
		return NULL;
	}

	MinHeapNode* root = heap->array[0];
	MinHeapNode* lastNode = heap->array[heap->size - 1];

	heap->array[0] = lastNode;
	
	//Update position array
	heap->pos[hashFinder(heap, root->name)].index = heap->size - 1;
	heap->pos[hashFinder(heap, lastNode->name)].index = 0;

	//reduce heap size and heapify root
	heap->size -= 1;
	minHeapify(heap, 0);

	return root;
}

void decreaseKey(MinHeap* heap, char* name, unsigned int weight){
	//get position of node in heap
	unsigned int hashKey = hashFinder(heap, name);
	unsigned int i = heap->pos[hashKey].index;

	//get the node and update weight
	heap->array[i]->weight = weight;

	while(i && heap->array[i]->weight < heap->array[PARENT(i)]->weight){
		//swap this node with its parent
		heap->pos[hashKey].index = PARENT(i);
		//change hashKey to i's parent's index in pos array
		hashKey = hashFinder(heap, heap->array[PARENT(i)]->name);
		heap->pos[hashKey].index = i;

		swapMinHeapNode(&heap->array[i], &heap->array[PARENT(i)]);
	
		//bump i to parent's index
		i = PARENT(i);	
	}
}

bool inMinHeap(MinHeap* heap, char* name){
	unsigned int hashKey = hashFinder(heap, name);
	if(hashKey >= heap->capacity){
		return false;
	}
	if(heap->pos[hashKey].index < heap->size){
		return true;
	}
	return false;
}

void destroy_heap(MinHeap* heap, unsigned int size){
	if(!heap){
		return;
	}

	unsigned int i;
	for(i = 0; i < size; i++){
		free(heap->array[i]);
	}

	free(heap->array);
	free(heap->pos);
	free(heap);
}
