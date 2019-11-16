#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Structs
 */

typedef struct 
{
    Element* next;
    Element* prev;

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
int getMemTotal();
int getMemLargestFree();
int getMemSmallFree(int size);
char isMemAlloc(void *ptr);

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

void* nextMalloc(size_t requested) 
{

}

void nextFree(void* block) 
{

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
}