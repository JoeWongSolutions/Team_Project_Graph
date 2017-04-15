#include "priorityQueue.h"
#ifndef GRAPH_H
#define GRAPH_H
#include "Team_Project_Graph/graph/main.c"
#endif


void printPath(MinHeapNode* dest);

unsigned int dijkstra(vertex* graph, _vertex* src, _vertex* dest, unsigned int size){
	
	unsigned int* dist = malloc(sizeof(unsigned int)*size);	//An array to hold the shortest distance (weight) values of each node in the graph from src 
	vertex* current = graph;	//Current is a utility pointer for traversing the vertical nodes in the adj. list
	_vertex* pCrawl;		//pCrawl is a utility pointer to traverse each adjacent node (horizontal nodes)
	MinHeapNode* heapCurrent;	//heapCurrent holds the current heap node extracted from the queue
	unsigned int hashValue, destHashValue;
	unsigned int pCrawlHashValue;
	unsigned int result;

	// minHeap will be the set of edges E
	struct MinHeap* minHeap = new_MinHeap(size);

	//Initialize step, fill heap with all vertices and set dist to infinite
	int v = 0;
	
	for (v = 0; v < size; v++){
		dist[v] = INT_MAX;
		minHeap->array[v] = new_MinHeapNode(current->VER.name, dist[v]);
		minHeap->pos[hashGenerator(minHeap, current->VER.name)].index = v;
		current = current->_vertical_list;
	}		

	//Make distance of src node 0 to be extracted first
	decreaseKey(minHeap, src->name, 0);
	dist[hashFinder(minHeap, src->name)] = 0;

	//initialize size of minHeap
	minHeap->size = size;
	//store hash value of dest
	destHashValue = hashFinder(minHeap, dest->name);

	//minHeap will contain all nodes that have not had their weights finalized
	while (!isEmpty(minHeap) && strcmp(dest->name, pCrawl->name)){

		//Extract vertex with minimum value from minHeap
		heapCurrent = extractMin(minHeap);
		hashValue = hashFinder(minHeap, heapCurrent->name);

		current = hashTableSearch(heapCurrent->name); //*****Need to implement this hashtable function correctly in hashtable.c**********
		pCrawl = current->VER._horizontal_next;
		pCrawlHashValue = hashFinder(minHeap, pCrawl->name);
		//For each adjacent vertex, if in the heap and distance value is more than weight plus distance to src, update the vertex with new distance.
		while(pCrawl){
			
			if(inMinHeap(minHeap, pCrawl->name) && dist[hashValue] != INT_MAX && pCrawl->weight + dist[hashValue] < dist[pCrawlHashValue]){
				
				//update the dist array
				dist[pCrawlHashValue] = pCrawl->weight + dist[hashValue];
				//update the heap node
				decreaseKey(minHeap, pCrawl->name, dist[pCrawlHashValue]);
				//update the parent pointer
				minHeap->array[pCrawlHashValue]->parent = heapCurrent;
			
			}
			if(strcmp(dest->name, pCrawl->name) == 0){
				break;
			}
			pCrawl = pCrawl->_horizontal_next;
		}
	}
	//Display info
	printPath(minHeap->array[destHashValue]);
	printf(" => finish\n");
	result = dist[destHashValue];
	
	//destroy allocated space
	free(dist);
	destroy_heap(minHeap, size);

	return result;
}

void printPath(MinHeapNode* dest){
	if(dest->parent == NULL){
		printf("start ");
		return;
	}
	printPath(dest->parent);
	printf("=> %s",dest->name);
}
