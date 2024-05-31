#include <stdio.h>
#include <malloc.h>
#include <windows.h>
#include "graph.h"

int main() {
    struct Node* nodes = NULL;
    uint32 c, n;
    byte result;

    LARGE_INTEGER frequency;        // ticks per second
    LARGE_INTEGER t1, t2;           // ticks
    double elapsedTime;

    QueryPerformanceFrequency(&frequency);

    printf("Give the number of nodes:");
    scanf("%u", &n);
    startid = 0;
    endid = n - 1;
    printf("Options:\n1 : Generate graph with random distances\n2 : Depth-First Search\n3 : Uniform-Cost Search\n4 : A* with custom heuristic\n");
    printf("5 : Print edges and cost\n6 : Set the starting node and the ending node\n");
    while (1)
    {
        printf("Choose an option:");
        scanf("%u", &c);
        switch (c)
        {
        case 1:
            if (n == 0) break; //No nodes, we generate nothing
            if (nodes) dealloc_nodes(&nodes, n); //Deallocate the current graph, so we can generate a new one and avoid memory leaks
            nodes = malloc(n * sizeof(struct Node)); //Allocate a new array of nodes
            if (!generate_graph(nodes, n)) //Generate random graph
            {
                printf("An error occured, execution halted.");
                return 0;
            }
            break;
        case 2:
            if (n == 0) break; //No nodes, no search
            if (!nodes) break;
            QueryPerformanceCounter(&t1);
            DFS_main(&nodes[startid], endid, n); //Depth-First Search algorithm
            QueryPerformanceCounter(&t2);
            elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart; // compute the elapsed time in milliseconds
            printf("%f ms.\n", elapsedTime);
            break;
        case 3:
            if (n == 0) break; //No nodes, no search
            if (!nodes) break;
            QueryPerformanceCounter(&t1);
            UCS_main(&nodes[startid], endid, n); //Uniform-Cost Search algorithm
            QueryPerformanceCounter(&t2);
            elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart; // compute the elapsed time in milliseconds
            printf("%f ms.\n", elapsedTime);
            break;
        case 4:
            if (n == 0) break; //No nodes, no search
            if (!nodes) break;
            QueryPerformanceCounter(&t1);
            A_Star_main(&nodes[startid], endid, n); //A-star with custom heuristic algorithm
            QueryPerformanceCounter(&t2);
            elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart; // compute the elapsed time in milliseconds
            printf("%f ms.\n", elapsedTime);
            break;
        case 5:
            print_pairs(nodes, n);
            break;
        case 6:
            if (n == 0) break; //No nodes
            retrystart:
            printf("Start node's id (1-%d): ", n);
            scanf("%u", &startid); //starting point
            if (startid > n || startid < 1)
                goto retrystart;
            retryend:
            printf("End node's id (1-%d): ", n);
            scanf("%u", &endid); //ending point
            if (endid > n || endid < 1)
                goto retryend;
            startid--; //indexing starts from 0
            endid--; //indexing start from 0
            break;
        default:
            if (nodes) dealloc_nodes(&nodes, n); //Deallocate the graph when exiting the program
            return 0;
        }
    }
    return 0;
}
