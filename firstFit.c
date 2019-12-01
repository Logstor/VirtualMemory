#include "firstFit.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "utility.h"

void firstInit(size_t size)
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
        printf("\nWARNING: Initializing memory again!\n");
        clean();
    }

    // Create memory pool
    memory.memPool.size = size;
    memory.memPool.memStart = malloc(size);

    // Start linked list
    memory.head = (Element*) calloc( 1, sizeof(Element) );
    memory.head->size = size;
    memory.head->alloc = 0;
    memory.head->ptr = memory.memPool.memStart;
    memory.bytesFree  = size;
    memory.tail = memory.head;
    memory.next = memory.head;
}

void* firstMalloc(size_t requested)
{
    // Check if there's space
    if (memory.bytesFree < requested)
        return NULL;

    // Get current position
    Element* element = memory.tail;

    // Find space
    do 
    {
        // Check space and alloc
        if (element->alloc == 0 && element->size >= requested)
        {
            Element* allocated = allocateBlock(element, requested);
            memory.bytesFree -= allocated->size;
            return allocated->ptr;
        }

        // Get next element
        element = element->next;
    } while (element != memory.tail && element != NULL);

    return NULL;
}