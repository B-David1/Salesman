#include <stdio.h>
#include "dynarray.h"

/*
* Resize the array if it goes past the allocated memory, otherwise do nothing.
* darray - the dynamic array struct to be received
*/

boolean resize_buffer(struct DynamicArray* darray)
{
    if (darray->capacity <= darray->size) //malloc size is lower than the number of elements
    {
        while (darray->capacity <= darray->size) //increase malloc size
            darray->capacity <<= 1;
        void* new_alloc = realloc(darray->array, darray->capacity * sizeof(*darray->array)); //resize
        if (new_alloc == NULL) return 0;
        darray->array = new_alloc;
    }
    return 1;
}

/*
* Add an element to the array and resize if necessary.
* darray - the dynamic array struct to be received
* element - the element to be added to the dynamic array
*/

boolean add_element(struct DynamicArray* darray, void* element)
{
    if (!resize_buffer(darray)) return 0; //check if it has enough memory
    darray->array[darray->size++] = element; //add element to dynamic array
    return 1;
}

void release_elements(struct DynamicArray* darray)
{
    uint32 iterator;

    for (iterator = 0; iterator < darray->size; iterator++)
    {
        if (darray->array[iterator]) free(darray->array[iterator]);
        darray->array[iterator] = NULL; //no more dangling pointer
    }
}