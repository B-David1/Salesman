#ifndef GRAPH_H
#define GRAPH_H

#include "dynarray.h"
#include "math.inl"

/*
* Node structure, contains information about a node from the graph.
* stores id, color and a resizable array of pointers to nodes that are connected to it.
*/

struct Node
{
	uint32 id;
	struct DynamicArray edges;
};

/*
* Edge structure, contains information about the edge between two nodes.
* stores the weight and the other node it is connected to.
*/

struct Edge
{
	float weight;
	struct Node* sibling;
};

/*
* Priority queue for nodes structure, contains the priority of a node and the next element in queue.
* stores pointer to a node, the priority, and a pointer to the next element.
*/

struct PQNode {
	struct Node* node;
	float cost;
	struct PQNode* parent;
};

/*
* Declarations for functions.
*/

boolean dealloc_nodes(struct Node**, uint32);
boolean generate_graph(struct Node*, uint32);
boolean print_pairs(struct Node*, uint32);
struct PQNode* create_pqnode(struct Node* node, float cost, struct PQNode* parent);
void pq_print(struct PQNode* current_pqnode);

/*
* Variables that symbolize the starting point and the ending point of the search.
*/

static uint32 startid, endid;

#endif //GRAPH_H