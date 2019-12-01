#include "utility.h"

#include <stdlib.h>

/**
 * Frees all memory allocated in this structure.
 */
void clean()
{
    // Free memory pool
    free(memory.memPool.memStart);

    // Free all elements in list
    Element* element = memory.tail;
    Element* temp;
    while (element != NULL)
    {
        // Get new element
        temp = element->next;

        // Free memory
        free(element);

        // Rotate
        element = temp;
    }
}

/**
 * Gets access to the actual memory pool.
 * @return pointer to pool
 */
void* getMemPool() { return memory.memPool.memStart; }

/**
 * Gets the amount of bytes in memory pool.
 * @return Amount of bytes
 */
int getMemTotal() { return memory.memPool.size; }

/**
 * Finds the amount of holes in the memory pool.
 * @return Amount of holes
 */
int getMemHoles()
{
    int holes = 0; 
    Element* element = memory.tail;

    do
    {
        // Check if it's free
        if (element->alloc == 0)
            holes++;

        // Get next element
        element = element->next;
    }
    while (element != NULL && element != memory.tail);

    return holes;
}

/**
 * Finds the amount of memory allocated in bytes.
 * @return Bytes allocated
 */
int getMemAllocated()
{ return memory.memPool.size - memory.bytesFree; }

/**
 * Finds the amount of bytes which is free in 
 * the memory pool.
 * @return Amount of bytes free
 */
int getMemFree()
{ return memory.bytesFree; }

/**
 * Number of bytes in the largest contiguous area 
 * of unallocated memory. It returns 0 if there's 
 * no more free memory.
 * @return Amount of bytes
 */
int getMemLargestFree()
{
    int largest = 0;
    Element* element = memory.tail;

    do
    {
        // Check if it's free and bigger
        if (element->alloc == 0 && element->size > largest)
            largest = element->size;

        // Get next element
        element = element->next;
    }
    while (element != NULL && element != memory.tail);

    return largest;
    
}

/**
 * Sums amount of free blocks smaller than "size" bytes.
 * @param size Bytes
 * @return Amount of blocks
 */
int getMemSmallFree(size_t size)
{
    int blocks = 0;
    Element* element = memory.tail;

    do
    {
        // Check size and if it's allocated
        if (element->alloc == 0 && element->size < size)
            blocks++;

        // Get next element
        element = element->next;
    }
    while (element != NULL && element != memory.tail);

    return blocks;

}

/**
 * Figures if the memory location provided is occupied.
 * @param ptr Pointer to address of interest
 * @return true or false
 */
char isMemAlloc(void *ptr) { return findByAddress(ptr) != NULL ? 666 : 0 ; }

/**
 * Finds the element in the linked list, which 
 * holds this address. If it isn't possible 
 * to find the element, then it returns NULL.
 * @param ptr The address or NULL if not found
 * @return Element*
 */
Element* findByAddress(void* ptr)
{
    // Start from the tail
    Element* element = memory.tail;

    // Sure the pointer is even in the pool
    if ( ptr >= memory.memPool.memStart && ptr < (memory.memPool.memStart + memory.memPool.size) )
    {
        do
        {
            // Check if ptr is in range
            if (ptr >= element->ptr && ptr < (element->ptr + element->size))
                return element;

            // Get next element
            element = element->next;
        }
        while (element != NULL && element != memory.tail);
    }

    // If not found then return NULL
    return NULL;
}