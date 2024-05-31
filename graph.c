#include <stdio.h>
#include "graph.h"

/*
* Frees memory used by the array of siblings for every node and the nodes themselves.
* nodes - pointer to the array of nodes, the array will be freed as well
* n - number of nodes
*/

boolean dealloc_nodes(struct Node** nodes, uint32 n)
{
    uint32 iterator;
    for (iterator = 0; iterator < n; iterator++) //Loop that frees the memory allocated by the array of edges of every node
        release_elements(&(*nodes)[iterator].edges);
    free(*nodes); //Free the array of nodes itself
    *nodes = NULL; //No more dangling pointer
    return 1;
}

/*
* Generates a complete graph with random weights for edges.
* nodes - the array of nodes
* n - number of nodes
*/

boolean generate_graph(struct Node* nodes, uint32 n)
{
    uint32 iterator, iterator2;
	time_t t;

    srand(time(&t)); //set the seed to the current time
    for (iterator = 0; iterator < n; iterator++) //loop that initializes the nodes within the nodes array
    {
        nodes[iterator].id = iterator;
        nodes[iterator].edges.size = 0;
        nodes[iterator].edges.capacity = n-1; //since there are n nodes, every node has n-1 edges
        nodes[iterator].edges.array = malloc(nodes[iterator].edges.capacity * sizeof(struct Node*)); //array of pointers to nodes
        if (nodes[iterator].edges.array == NULL)
        {
            printf("Failed to allocate memory for edges");
            return 0;
        }
    }
    for (iterator = 0; iterator < n-1; iterator++) //create edges between nodes
    {
        for (iterator2 = iterator+1; iterator2 < n; iterator2++)
        {
            float weight = frand(0.0f, 500.0f);
            struct Edge* newEdge = malloc(sizeof(struct Edge));
            newEdge->sibling = &nodes[iterator2];
            newEdge->weight = weight;
            if (!add_element(&nodes[iterator].edges, newEdge)) //Add the edge to the current node's edges
            {
                printf("Failed to allocate memory for edges");
                return 0;
            }
            struct Edge* newEdge2 = malloc(sizeof(struct Edge));
            newEdge2->sibling = &nodes[iterator];
            newEdge2->weight = weight;
            if (!add_element(&nodes[iterator2].edges, newEdge2)) //add the current edge to the other node's edges
            {
                printf("Failed to allocate memory for edges");
                return 0;
            }
        }
    }
    return 1;
}

/*
* Prints the graph as pairs of nodes with and the weight of the edge between them. No duplicates are shown.
* nodes - the array of nodes
* n - the number of nodes
*/

boolean print_pairs(struct Node* nodes, uint32 n)
{
    uint32 iterator, siblings_iterator;

    if (nodes == NULL)
    {
        printf("No graph has been generated\n");
        return 0;
    }

    printf("Graph edges and cost:\n");
    for (iterator = 0; iterator < n; iterator++) //display graph pairs by showing the current node, the other node it is connected to, and the weight of the edge
    {
        for (siblings_iterator = iterator; siblings_iterator < nodes[iterator].edges.size; siblings_iterator++)
        {
            struct Edge* edge = nodes[iterator].edges.array[siblings_iterator];
            struct Node* sibling = edge->sibling;
            printf("%u %u %f\n", nodes[iterator].id, sibling->id, edge->weight);
        }
    }
    return 1;
}

/*
* Adds the priority functionality to a node and inserts it in a queue.
* node - the node to be inserted in the queue
* cost - the priority of the node
* parent - the next element in the queue
*/

struct PQNode* create_pqnode(struct Node* node, float cost, struct PQNode* parent)
{
    struct PQNode* pqnode = (struct PQNode*)malloc(sizeof(struct PQNode));
    pqnode->node = node;
    pqnode->cost = cost;
    pqnode->parent = parent;
    return pqnode;
}

/*
* Prints the entire priority queue recursively.
* current_pqnode - the node in the priority queue to be printed
*/

void pq_print(struct PQNode* current_pqnode)
{
    if (current_pqnode->parent) {
        pq_print(current_pqnode->parent);
    }
    printf("%u ", current_pqnode->node->id+1); //the printing is done in reverse order as it should be the oldest to most recent
}