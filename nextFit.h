#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "logger.h"
#include "memoryStructure.h"
#include "utility.h"

/*
 * Prototypes
*/
void nextinit(size_t size);

void* nextMalloc(size_t requested);
void nextFree(void* block);
void printMemory();
Element* allocateBlock(Element* space, size_t size);
void freeElement(Element* element);
void mergeForward(Element* element);
void mergeBackwards(Element* element);

/*
 * Implementations
 */
void nextinit(size_t size)
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

    /*
    //TODO: Remove
    char buffer[100];
    sprintf(buffer, "Initializing %lu bytes\n", size);
    writeLog(buffer);
    */
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
    //TODO: Remove
    char buffer[100];
    sprintf(buffer, "Trying to allocate %lu bytes\n", requested);
    writeLog(buffer);

    // Check if there's space
    if (memory.bytesFree < requested)
        return NULL;

    //TODO: Remove
    sprintf(buffer, "\tSpace check done\n");
    writeLog(buffer);

    Element* allocated;

    // Get current position
    Element* element = memory.next;

    //TODO: Remove
    sprintf(buffer, "\tGotten current position\n");
    writeLog(buffer);

    // Find space
    do
    {
        //TODO: Remove
        sprintf(buffer, "\tChecking for NULL\n");
        writeLog(buffer);

        // Check NULL
        if (element == NULL)
        {
            element = memory.tail;
        }

        //TODO: Remove
        sprintf(buffer, "\tChecking for space and size\n");
        writeLog(buffer);

        // Check space and alloc
        if (element->alloc == 0 && element->size >= requested)
        {
            //TODO: Remove
            sprintf(buffer, "\tFound space\n");
            writeLog(buffer);

            allocated = allocateBlock(element, requested);
            memory.next = allocated->next;
            memory.bytesFree -= allocated->size;
            break;
        }

        //TODO: Remove
        sprintf(buffer, "\tGetting element->next\n");
        writeLog(buffer);

        // Get next element
        element = element->next;

    } while (element != memory.next);

    
    //TODO: Remove
    if (allocated != NULL)
    {
        char buffer[100];
        sprintf(buffer, "Allocating %lu bytes\n\n", requested);
        writeLog(buffer);
    }
    else
    {
        char buffer[100];
        sprintf(buffer, "Couldn't allocate %lu bytes\n\n", requested);
        writeLog(buffer);
    }
    
    
    return allocated->ptr;
}

void nextFree(void* block) 
{
    //TODO: Remove
    char buffer[100];
    sprintf(buffer, "Freeing %p\n", block);
    writeLog(buffer);
    
    // Find the correct element
    Element* element = findByAddress(block);
    if(element == NULL)
        return;

    // Check for error
    if (element->alloc == 0)
    {
        //TODO: Remove
        sprintf(buffer, "\tWARNING: Memory was already free!\n");
        writeLog(buffer);

        printf("WARNING: Memory was already free!\n");
        return;
    }

    // Free it
    freeElement(element);
}

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
        printf("Element %u\n\tSize: %d\n\tAllocated: %d\n\tPointer: %p\n\tSelf: %p\n\tPrev: %p\n\tNext: %p\n", 
        count, element->size, element->alloc, element->ptr, element, element->prev, element->next);

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
 * This method allocates the memory, and takes care of 
 * splitting Elements, head and tail.
 * @param space Pointer to Element with enough space
 * @param size Space needed in bytes
 * @return Pointer to the allocated block
 */
Element* allocateBlock(Element* space, size_t size)
{
    //TODO: Remove
    char buffer[100];
    sprintf(buffer, "Allocating %lu byte block in element with size %d\n", size, space->size);
    writeLog(buffer);

    // Space is bigger
    if (space->size > size)
    {
        //TODO: Remove
        sprintf(buffer, "\tSpace is bigger\n");
        writeLog(buffer);

        // Split the block of space in two
        Element* newElement = (Element*) malloc( sizeof(Element) );
        if (newElement == NULL)
        {
            //TODO: Remove!
            sprintf(buffer, "\tMalloc failed!\n");
            writeLog(buffer);
        }
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
        //TODO: Remove
        sprintf(buffer, "\tSpace is equal\n");
        writeLog(buffer);

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
    // Free the block
    element->alloc = 0;
    memory.bytesFree += element->size;

    // Merge forward
    if (element->next != NULL)
    {
        if (element->next->alloc == 0)
        { mergeForward(element); }
    }

    // Merge backwards
    if (element->prev != NULL)
    {
        if (element->prev->alloc == 0)
        { mergeBackwards(element); }
    }
}

/**
 * This merges the element with it's next element.
 * This isn't taking allocation into consideration, 
 * but makes sure head, tail and next is correct after 
 * the merge.
 * @param element The growing element
 */
void mergeForward(Element* element)
{
    //TODO: Remove
    char buffer[100];
    sprintf(buffer, "Merging element %p forward\n", element);
    writeLog(buffer);

    Element* next = element->next;

    // Set next and prev ptrs
    element->next       = next->next;
    if (next->next != NULL)
        next->next->prev = element;

    // Correct size
    element->size += next->size;

    // Check head
    if (element->next == NULL)
        memory.head = element;

    // Check next
    if (memory.next == next)
        memory.next = element;

    //TODO: Remove
    sprintf(buffer, "\tRunning free() on %p\n", next);
    writeLog(buffer);

    // Free memory
    free(next);
}

/**
 * This merges the element with it's prev element. 
 * This isn't taking allocation into consideration, 
 * but makes sure head, tail and next is correct after 
 * the merge.
 * @param element The growing element
 */
void mergeBackwards(Element* element)
{
    //TODO: Remove
    char buffer[100];
    sprintf(buffer, "Merging element %p backwards\n", element);
    writeLog(buffer);

    Element* previous = element->prev;

    // Set next and prev pointers
    element->prev = previous->prev;
    if (element->prev != NULL)
        element->prev->next = element;

    // Memory pool pointer
    element->ptr = previous->ptr;

    // Correct size
    element->size += previous->size;

    // Check tail
    if (element->prev == NULL)
        memory.tail = element;

    // Free memory
    free(previous);
}