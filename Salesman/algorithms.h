#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "graph.h"
#include <float.h>

/*
* Declarations for functions.
*/

void DFS_main(struct Node* nodes, uint32 n);
void DFS(struct Node* node, uint32 end_node, byte* visited, float current_cost, float* min_cost, struct DynamicArray* current_path, struct DynamicArray* min_path);
void UCS_main(struct Node* start_node, uint32 end_node, uint32 n);
float UCS(struct Node* start_node, uint32 end_node, boolean* visited, uint32 n);
float UCS_reverse(struct Node* start_node, uint32 end_node, boolean* visited, uint32 n);
void A_Star_main(struct Node* start_node, uint32 end_node, uint32 n);
float A_Star(struct Node* start_node, uint32 end_node, boolean* visited, uint32 n);
float A_Star_reverse(struct Node* start_node, uint32 end_node, boolean* visited, uint32 n);

// custom heuristic function (a better heuristic function for the travelling salesman is the 2-opt)
float custom_heuristic(struct Node* node, struct Node* end_node)
{
    return 1.0f; // Estimate the heuristic value as a fraction of the maximum possible steps
}

#endif //ALGORITHMS_H