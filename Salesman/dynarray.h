#ifndef DYNARRAY_H
#define DYNARRAY_H

#include <stdio.h>
#include <malloc.h>

/*
* Type definitions for portability.
*/

typedef unsigned long uint32;
typedef long int32;
typedef unsigned char byte;
typedef byte boolean;

/*
* Dynamic Array of pointers, uses dynamic allocation to store in an array of pointers of any type.
* array: the array of pointers
* size: the number of elements within the array
* capacity: memory allocated for the array
*/

struct DynamicArray
{
	void** array;
	uint32 size;
	uint32 capacity;
};

/*
* Declarations for functions.
*/

boolean resize_buffer(struct DynamicArray*);
boolean add_element(struct DynamicArray*, void*);
void release_elements(struct DynamicArray*);

#endif //DYNARRAY_H