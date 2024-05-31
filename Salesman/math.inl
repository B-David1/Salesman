
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Generate a random float number between a and b

inline float frand(float a, float b)
{
    return ((b - a) * ((float)rand() / RAND_MAX)) + a;
}