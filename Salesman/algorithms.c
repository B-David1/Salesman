
#include "algorithms.h"

/*
* The main DFS function of the DFS algorithm, used for memory management and calling the DFS algorithm
* start_node - the node which represents the starting point
* end_node - the index of the node which represents the ending point
* n - number of nodes
*/

void DFS_main(struct Node* start_node, uint32 end_node, uint32 n)
{
    float min_cost = FLT_MAX;
    byte* visited = calloc(n, sizeof(byte) * n);

    struct DynamicArray current_path;
    current_path.array = malloc(sizeof(byte) * n);
    current_path.size = 0;
    current_path.capacity = n;
    struct DynamicArray min_path;
    min_path.array = malloc(sizeof(byte) * n);
    min_path.size = 0;
    min_path.capacity = n;

    DFS(start_node, end_node, visited, 0, &min_cost, &current_path, &min_path, 0);

    printf("DFS Minimum cost path with cost %f: ", min_cost);
    for (uint32 i = 0; i < min_path.size; ++i) {
        struct Node* n = min_path.array[i];
        printf("%u ", n->id+1);
    }
    printf("\n");

    //   release_elements(&current_path);
    //   release_elements(&min_path);
    free(visited);
}

/*
* The recursive DFS algorithm
* node - the node which represents the starting point
* end_node - the index of the node which represents the ending point
* visited - an array which displays which nodes were visited
* current_cost - the current cost of the current path when the function is called
* min_cost - the overall minimum cost of what has been computed so far
* current_path - an array which contains the current path being processed
* min_path - an array which contains the path that has the minimum cost so far
* returning - signals that the algorithm has found the end point and now is returning to the start point
*/

void DFS(struct Node* node, uint32 end_node, byte* visited, float current_cost, float* min_cost, struct DynamicArray* current_path, struct DynamicArray* min_path, boolean returning)
{
    if (!node || visited[node->id]) {
        return;
    }

    // Mark the current node as visited
    visited[node->id] = 1;
    add_element(current_path, node);

    // Check if the end node is reached and we are not yet returning
    if (node->id == end_node && !returning) {
        // Continue DFS to return to the start node
        visited[startid] = 0;
        for (uint32 i = 0; i < node->edges.size; ++i) {
            struct Edge* edge = (struct Edge*)node->edges.array[i];
            DFS(edge->sibling, startid, visited, current_cost + edge->weight, min_cost, current_path, min_path, 1);
        }
        visited[startid] = 1;
    }
    // Check if the start node is reached and we are returning
    else if (node->id == end_node && returning) {
        if (current_cost < *min_cost) {
            *min_cost = current_cost;

            // Update the minimum path
            min_path->size = 0;
            for (uint32 i = 0; i < current_path->size; ++i) {
                add_element(min_path, current_path->array[i]);
            }
        }
    }
    else {
        // Visit all the adjacent nodes
        for (uint32 i = 0; i < node->edges.size; ++i) {
            struct Edge* edge = (struct Edge*)node->edges.array[i];
            DFS(edge->sibling, end_node, visited, current_cost + edge->weight, min_cost, current_path, min_path, returning);
        }
    }

    // Backtrack
    visited[node->id] = 0;
    current_path->size--;
}

/*
* The main UCS function of the UCS algorithm, used for memory management and calling the UCS algorithm. The algorithm is split into two parts: 
the part which finds the end point and the part which returns to the starting point.
* start_node - the node which represents the starting point
* end_node - the index of the node which represents the ending point
* n - number of nodes
*/

void UCS_main(struct Node* start_node, uint32 end_node, uint32 n)
{
    boolean* visited = calloc(n, sizeof(byte) * n);

    float cost = UCS(start_node, end_node, visited, n);

    visited[start_node->id] = 0;

    cost += UCS_reverse(&(start_node[end_node]), start_node->id, visited, n);

    printf("with a Total Cost of %f\n", cost);

    free(visited);
}

/*
* The first part of the UCS algorithm that computes the minimum cost distance and the path to the end point.
* start_node - the node which represents the starting point
* end_node - the index of the node which represents the ending point
* visited - an array which displays which nodes were visited
* n - number of nodes
* 
* return - returns the cost of the path to the end point
*/

float UCS(struct Node* start_node, uint32 end_node, boolean* visited, uint32 n)
{
    // Initialize priority queue with the start node
    struct DynamicArray pq;
    pq.array = malloc(sizeof(struct PQNode) * n);
    pq.size = 0;
    pq.capacity = n;
    add_element(&pq, create_pqnode(start_node, 0, NULL));

    while (pq.size > 0) {
        // Find the node with the minimum cost
        uint32 min_index = 0;
        for (uint32 i = 1; i < pq.size; ++i) {
            struct PQNode* pqnode1 = pq.array[min_index];
            struct PQNode* pqnode2 = pq.array[i];
            if (pqnode2->cost < pqnode1->cost) {
                min_index = i;
            }
        }

        // Pop the node with the minimum cost
        struct PQNode* current_pqnode = pq.array[min_index];
        for (uint32 i = min_index; i < pq.size - 1; ++i) {
            pq.array[i] = pq.array[i + 1];
        }
        pq.size--;

        struct Node* current_node = current_pqnode->node;

        // If we reached back to the start node from the end node
        if (current_node->id == end_node) {
            printf("UCS minimum cost path: ");
            pq_print(current_pqnode->parent);

            memset(visited, 0, n);
            struct PQNode* pqnode = current_pqnode->parent;
            if (!pqnode->parent) return;
            do
            {
                visited[pqnode->node->id] = 1;
                pqnode = pqnode->parent;
            } while (pqnode->parent);

            return current_pqnode->cost;
        }

        // Mark the current node as visited
        visited[current_node->id] = 1;

        // Add all unvisited adjacent nodes to the priority queue
        for (uint32 i = 0; i < current_node->edges.size; ++i) {
            struct Edge* edge = (struct Edge*)current_node->edges.array[i];
            if (!visited[edge->sibling->id]) {
                add_element(&pq, create_pqnode(edge->sibling, current_pqnode->cost + edge->weight, current_pqnode));
            }
        }
    }

    printf("No path found between the nodes.\n");

    return 0.0f;
}

/*
* The second part of the UCS algorithm that computes the minimum cost distance and the path to the start point.
* start_node - the node which represents the starting point
* end_node - the index of the node which represents the ending point
* visited - an array which displays which nodes were visited
* n - number of nodes
*
* return - returns the cost of the path to the starting point
*/

float UCS_reverse(struct Node* start_node, uint32 end_node, boolean* visited, uint32 n)
{
    // Initialize priority queue with the start node
    struct DynamicArray pq;
    pq.array = malloc(sizeof(struct PQNode) * n);
    pq.size = 0;
    pq.capacity = n;
    add_element(&pq, create_pqnode(start_node, 0, NULL));

    while (pq.size > 0) {
        // Find the node with the minimum cost
        uint32 min_index = 0;
        for (uint32 i = 1; i < pq.size; ++i) {
            struct PQNode* pqnode1 = pq.array[min_index];
            struct PQNode* pqnode2 = pq.array[i];
            if (pqnode2->cost < pqnode1->cost) {
                min_index = i;
            }
        }

        // Pop the node with the minimum cost
        struct PQNode* current_pqnode = pq.array[min_index];
        for (uint32 i = min_index; i < pq.size - 1; ++i) {
            pq.array[i] = pq.array[i + 1];
        }
        pq.size--;

        struct Node* current_node = current_pqnode->node;

        // If we reached back to the start node from the end node
        if (current_node->id == end_node) {
            pq_print(current_pqnode);

            return current_pqnode->cost;
        }

        visited[current_node->id] = 1;

        // Add all unvisited adjacent nodes to the priority queue
        for (uint32 i = 0; i < current_node->edges.size; ++i) {
            struct Edge* edge = current_node->edges.array[i];
            if (!visited[edge->sibling->id]) {
                add_element(&pq, create_pqnode(edge->sibling, current_pqnode->cost + edge->weight, current_pqnode));
            }
        }
    }

    printf("No path found between the nodes.\n");

    return 0.0f;
}

/*
* The main A* function of the A* algorithm, used for memory management and calling the A* algorithm. The algorithm is split into two parts:
the part which finds the end point and the part which returns to the starting point.
* start_node - the node which represents the starting point
* end_node - the index of the node which represents the ending point
* n - number of nodes
*/

void A_Star_main(struct Node* start_node, uint32 end_node, uint32 n)
{
    boolean* visited = calloc(n, sizeof(byte) * n);

    float cost = A_Star(start_node, end_node, visited, n);

    visited[start_node->id] = 0;

    cost += A_Star_reverse(&(start_node[end_node]), start_node->id, visited, n);

    printf("with a Total Cost of %f\n", cost);

    free(visited);
}

/*
* The first part of the A* algorithm that computes the minimum cost distance and the path to the end point.
* start_node - the node which represents the starting point
* end_node - the index of the node which represents the ending point
* visited - an array which displays which nodes were visited
* n - number of nodes
*
* return - returns the cost of the path to the end point
*/

float A_Star(struct Node* start_node, uint32 end_node, boolean* visited, uint32 n) {
    // Initialize priority queue with the start node
    struct DynamicArray pq;
    pq.array = malloc(sizeof(struct PQNode) * n);
    pq.size = 0;
    pq.capacity = n;

    float heuristic = custom_heuristic(start_node, (&start_node)[end_node]);

    add_element(&pq, create_pqnode(start_node, 0, NULL));

    while (pq.size > 0) {
        // Find the node with the minimum cost
        uint32 min_index = 0;
        for (uint32 i = 1; i < pq.size; ++i) {
            struct PQNode* pqnode1 = pq.array[min_index];
            struct PQNode* pqnode2 = pq.array[i];
            if (pqnode2->cost + heuristic < pqnode1->cost + heuristic) {
                min_index = i;
            }
        }

        // Pop the node with the minimum cost
        struct PQNode* current_pqnode = pq.array[min_index];
        for (uint32 i = min_index; i < pq.size - 1; ++i) {
            pq.array[i] = pq.array[i + 1];
        }
        pq.size--;

        struct Node* current_node = current_pqnode->node;

        // If we reached back to the start node from the end node
        if (current_node->id == end_node) {
            printf("A* minimum cost path: ", current_pqnode->cost);
            pq_print(current_pqnode->parent);

            memset(visited, 0, n);
            struct PQNode* pqnode = current_pqnode->parent;
            if (!pqnode->parent) return;
            do
            {
                visited[pqnode->node->id] = 1;
                pqnode = pqnode->parent;
            } while (pqnode->parent);

            return current_pqnode->cost;
        }

        // Mark the current node as visited
        visited[current_node->id] = 1;

        // Add all unvisited adjacent nodes to the priority queue
        for (uint32 i = 0; i < current_node->edges.size; ++i) {
            struct Edge* edge = current_node->edges.array[i];
            if (!visited[edge->sibling->id]) {
                add_element(&pq, create_pqnode(edge->sibling, current_pqnode->cost + edge->weight, current_pqnode));
            }
        }
    }

    printf("No path found between the nodes using A*.\n");

    return 0.0f;
}

/*
* The second part of the A* algorithm that computes the minimum cost distance and the path to the start point.
* start_node - the node which represents the starting point
* end_node - the index of the node which represents the ending point
* visited - an array which displays which nodes were visited
* n - number of nodes
*
* return - returns the cost of the path to the starting point
*/


float A_Star_reverse(struct Node* start_node, uint32 end_node, boolean* visited, uint32 n) {
    // Initialize priority queue with the start node
    struct DynamicArray pq;
    pq.array = malloc(sizeof(struct PQNode) * n);
    pq.size = 0;
    pq.capacity = n;

    float heuristic = custom_heuristic(start_node, (&start_node)[end_node]);

    add_element(&pq, create_pqnode(start_node, 0, NULL));

    while (pq.size > 0) {
        // Find the node with the minimum cost
        uint32 min_index = 0;
        for (uint32 i = 1; i < pq.size; ++i) {
            struct PQNode* pqnode1 = pq.array[min_index];
            struct PQNode* pqnode2 = pq.array[i];
            if (pqnode2->cost + heuristic < pqnode1->cost + heuristic) {
                min_index = i;
            }
        }

        // Pop the node with the minimum cost
        struct PQNode* current_pqnode = pq.array[min_index];
        for (uint32 i = min_index; i < pq.size - 1; ++i) {
            pq.array[i] = pq.array[i + 1];
        }
        pq.size--;

        struct Node* current_node = current_pqnode->node;

        // If we reached back to the start node from the end node
        if (current_node->id == end_node) {
            pq_print(current_pqnode);

            return current_pqnode->cost;
        }

        // Mark the current node as visited
        visited[current_node->id] = 1;

        // Add all unvisited adjacent nodes to the priority queue
        for (uint32 i = 0; i < current_node->edges.size; ++i) {
            struct Edge* edge = current_node->edges.array[i];
            if (!visited[edge->sibling->id]) {
                add_element(&pq, create_pqnode(edge->sibling, current_pqnode->cost + edge->weight, current_pqnode));
            }
        }
    }

    printf("No path found between the nodes using A*.\n");

    return 0.0f;
}