#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Structs
 */

typedef struct Element
{
    struct Element* next;
    struct Element* prev;

    int size;           // How many bytes in this block?
    char alloc;         // 1 if this block is allocated,
                        // 0 if this block is free.
    void *ptr; 

}Element;

typedef struct
{
	void* memStart;
	size_t size;
}MemoryPool;

typedef struct
{
	MemoryPool memPool;
	Element* head;
	Element* tail;
	Element* next;
}Memory;

/*
 * Prototypes
*/

void clean();
void initialize(size_t size);
void* nextMalloc(size_t requested);
void nextFree(void* block);

void* getMemPool();
int getMemTotal();
int getMemHoles();
int getMemAllocated();
int getMemFree();
int getMemLargestFree();
int getMemSmallFree(size_t size);
char isMemAlloc(void *ptr);
void printMemory();

Element* findByAddress(void* ptr);
Element* allocateBlock(Element* space, size_t size);
void freeElement(Element* element);
void mergeForward(Element* element);
void mergeBackwards(Element* element);

/*
 * Globals
 */

Memory memory;

/*
 * Implementations
 */

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

void initialize(size_t size)
{
    // Check parameter
    if (size < 1)
    {
        printf("ERROR: Trying to initialize with less 1 byte size!\n");
        return;
    }

    // Check if memory is already initialized
    if (memory.memPool.memStart != NULL)
    {
        printf("WARNING: Initializing memory again!\n");
        clean();
    }

    // Create memory pool
    memory.memPool.size = size;
    memory.memPool.memStart = malloc(size);

    // Start linked list
    memory.head = (Element*) malloc( sizeof(Element) );
    memory.head->size = size;
    memory.head->alloc = 0;
    memory.head->ptr = memory.memPool.memStart;
    memory.tail = memory.head;
    memory.next = memory.head;
}

/**
 * Takes care of allocating memory using the next fit
 * strategy. It takes the size of the desired memory in bytes, 
 * and returns the pointer to the memory. It will return NULL 
 * if no memory is available.
 * @param requested Size in bytes
 * @return Pointer to memory
 */
void* nextMalloc(size_t requested) 
{
    Element* allocated;

    // Get current position
    Element* element = memory.next;

    // Find space
    do
    {
        // Check NULL
        if (element == NULL)
        {
            element = memory.tail;
        }

        // Check space and alloc
        if (element->alloc == 0 || element->size >= requested)
        {
            allocated = allocateBlock(element, requested);
            memory.next = allocated->next;
            break;
        }

        // Get next element
        element = element->next;

    } while (element != memory.next);
    
    return allocated->ptr;
}

void nextFree(void* block) 
{
    // Find the correct element
    Element* element = findByAddress(block);

    // Check for error
    if (element->alloc == 0)
    {
        printf("WARNING: Memory was already free!\n");
        return;
    }

    // Free it
    freeElement(element);
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

    while (element != NULL)
    {
        // Check if it's free
        if (element->alloc == 0)
            holes++;

        // Get next element
        element = element->next;
    }

    return holes;
}

/**
 * Finds the amount of memory allocated in bytes.
 * @return Bytes allocated
 */
int getMemAllocated()
{
    int bytes = 0;
    Element* element = memory.tail;

    while (element != NULL)
    {
        // Check if it's allocated
        if (element->alloc != 0)
            bytes += element->size;

        // Get next element
        element = element->next;
    }

    return bytes;
}

/**
 * Finds the amount of bytes which is free in 
 * the memory pool.
 * @return Amount of bytes free
 */
int getMemFree()
{
    int bytes = 0;
    Element* element = memory.tail;

    while (element != NULL)
    {
        // Check if it's free
        if (element->alloc == 0)
            bytes += element->size;

        // Get next element
        element = element->next;
    }

    return bytes;
}

/**
 * Number of bytes in the largest contiguous area 
 * of unallocated memory. It returns -1 if there's 
 * no more free memory.
 * @return Amount of bytes
 */
int getMemLargestFree()
{
    int largest = -1;
    Element* element = memory.tail;

    while (element != NULL)
    {
        // Check if it's free and bigger
        if (element->alloc == 0 || element->size > largest)
            largest = element->size;

        // Get next element
        element = element->next;
    }

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

    while (element != NULL)
    {
        // Check size and if it's allocated
        if (element->alloc == 0 || element->size < size)
            blocks++;

        // Get next element
        element = element->next;
    }

    return blocks;

}

/**
 * Figures if the memory location provided is occupied.
 * @param ptr Pointer to address of interest
 * @return true or false
 */
char isMemAlloc(void *ptr) { return findByAddress(ptr) != NULL ? 666 : 0 ; }

/**
 * 
 */
void printMemory()
{
    // Get first element
    unsigned int count = 0;
    Element* element = memory.tail;

    while (element != NULL)
    {
        // Print
        printf("Element %u\n\tSize: %d\n\tAllocated: %d\n", count, element->size, element->alloc);

        // Get next element
        count++;
        element = element->next;
    }

    // Make extra spacing
    puts("");
}

/*
 * Support Methods
 */

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

    while (element != NULL)
    {
        // Check if ptr is in range
        if (element->ptr <= ptr || (element->ptr + element->size) > ptr)
            return element;

        // Get next element
        element = element->next;
    }

    // If not found then return NULL
    return NULL;
}

/**
 * This method allocates the memory, and takes care of 
 * splitting Elements, head and tail.
 * @param space Pointer to Element with enough space
 * @param size Space needed in bytes
 * @return Pointer to the allocated block
 */
Element* allocateBlock(Element* space, size_t size)
{
    // Space is bigger
    if (space->size > size)
    {
        // Split the block of space in two
        Element* newElement = (Element*) malloc( sizeof(Element) );
        newElement->next = space;
        newElement->prev = space->prev;
        newElement->ptr  = space->ptr;
        newElement->size = size;
        newElement->alloc= 1;

        space->prev  = newElement;
        space->ptr  += size;
        space->size -= size;

        if (newElement->prev != NULL)
            newElement->prev->next = newElement;

        // Take care of tail
        if (newElement->prev == NULL)
            memory.tail = newElement;

        return newElement;
    }

    // Space is equal
    else if (space->size == size)
    {
        // Just allocate space, and return
        space->alloc = 1; return space;
    }

    // Error occurred
    else
    {
        // Write error to log
        printf("ERROR: allocateBlock() is called, when there isn't not enough space:\n");
        printf("\tRequested Size: %lu\n\tSize of Block: %u\n", size, space->size);
        return NULL;
    }
}

/**
 * This takes care of freeing the memory, and takes 
 * care of merging elements. It will also handle head, 
 * tail and next pointers.
 * @param element Pointer to element to be freed
 */
void freeElement(Element* element)
{
    //TODO: Implement this!
    // Free the block
    element->alloc == 0;

    // Merge forward
    if (element->next->alloc == 0)
    { mergeForward(element); }

    // Merge backwards
    if (element->prev->alloc == 0)
    { mergeBackwards(element); }
}

/**
 * This merges the element with it's next element.
 * This isn't taking allocation into consideration, 
 * but makes sure head, tail and next is correct after 
 * the merge.
 * @param element The growing element
 */
void mergeForward(Element* element)
{}

/**
 * This merges the element with it's prev element. 
 * This isn't taking allocation into consideration, 
 * but makes sure head, tail and next is correct after 
 * the merge.
 * @param element The growing element
 */
void mergeBackwards(Element* element)
{}