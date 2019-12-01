#include "worst_fit.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>


void *splitElementAndAllocate (Element* elementToSplit, size_t sizeToAllocate);

void *worstFitMalloc(size_t requested) {
    // Size of worstFit (starts off as requested size).
    int worstFitSize = requested;
    // find worst fitted Element
    Element *worstFitElement = NULL;
    Element *currentElement = memory.head;
    // Check that currentElement "obejct" is NOT null, that its size is equal or bigger than requested and it's free.
    while (currentElement != NULL ) {
        if (currentElement->size >= requested && currentElement -> alloc == 0) {
            // Checks if there still isn't found any worstFitElement.
            if  (worstFitElement == NULL) {
                worstFitElement = currentElement;
                worstFitSize = currentElement->size;
            }
                // Checks if the currentElement's size is bigger than worstFitSize
            else if (currentElement->size > worstFitSize) {
                worstFitElement = currentElement;
                worstFitSize = currentElement->size;
            }
        }

        // Sets current to the next Element "object" in the list.
        currentElement = currentElement -> next;
    };

    // Checks if there was found a worstFit
    if  (worstFitElement != NULL){
        // Checks if the found worstFitElement "objects" size is equal to the requested.
        if  (worstFitElement->size == requested) {
            worstFitElement->alloc = ALLOCATED;
            return worstFitElement->ptr;
        }
            // Else the size of the found worstFitElement "Objects" is bigger than the requested.
            // So it needs to be split.
        else {
            return splitElementAndAllocate(worstFitElement,requested);
        }
    } else {
        return NULL;
    }
}

void *splitElementAndAllocate (Element* elementToSplit, size_t sizeToAllocate) {

    // Allocates space for a new Element "Object" and sets its variables.
    Element *requstedElement = (Element*)malloc(sizeof(Element));

    // Sets the values for the requestedElement.
    requstedElement->next = elementToSplit;
    requstedElement->prev = elementToSplit->prev;
    requstedElement->size = sizeToAllocate;
    requstedElement->ptr = elementToSplit->ptr;
    requstedElement->alloc = ALLOCATED;
    // Makes sure that the header is still the first Element.
    if (requstedElement->prev == NULL) {
        memory.head = requstedElement;
    }

    // Requested Element's prev Element's next pointer set to point to the newly created Element.
    if (requstedElement->prev != NULL) {
        requstedElement->prev->next = requstedElement;
    }

    // ElementToSplit's values is set to match the newly created Element ahead of it.
    elementToSplit->prev = requstedElement;
    elementToSplit->size = elementToSplit->size - sizeToAllocate;
    elementToSplit->ptr = elementToSplit->ptr + sizeToAllocate;

    return requstedElement->ptr;
}

/* Allocate a block of memory with the requested size.
 *  If the requested block is not available, mymalloc returns NULL.
 *  Otherwise, it returns a pointer to the newly allocated block.
 *  Restriction: requested >= 1
 */

void freeInclElementsPrev (Element * ElementToFree) {
    // ElementToFree's prev Element is saved for later use.
    Element *prevFromMemoryToFree = ElementToFree->prev;
    // Resets ElementToFree's prev to prev's prev
    ElementToFree->prev = prevFromMemoryToFree->prev;
    // Resets ElementsToFree's size to sum of ToFree og prev's Size.
    ElementToFree->size = ElementToFree->size + prevFromMemoryToFree->size;
    // Checks if prevForMemoryToTree is null
    if (prevFromMemoryToFree->prev != NULL) {
        prevFromMemoryToFree->prev->next = ElementToFree;
    }
    // Free the Element that have been merged into the Element that should be freed.
    free(prevFromMemoryToFree);
}

void freeInclElementsNext (Element * ElementToFree) {
    // ElementToFree's next Element is saved for later use.
    Element *nextForMemoryToFree = ElementToFree->next;
    // Resets ElementToFree's next to next's next
    ElementToFree->next = nextForMemoryToFree->next;
    // Resets ElementsToFree's size to sum of ToFree og prev's Size.
    ElementToFree->size = ElementToFree->size + nextForMemoryToFree->size;
    // Checks if prevForMemoryToTree is null
    if (nextForMemoryToFree->next != NULL) {
        nextForMemoryToFree->next->prev = ElementToFree;
    }
    // Free the Element that have been merged into the Element that should be freed.
    free(nextForMemoryToFree);
}

/* Frees a block of memory previously allocated by mymalloc. */
void worstFree(void* block) {
    // Checks if block is inside the controlled memory
    if (block >= memory.head->ptr && block < memory.head->ptr + memory.memPool.size) {
        // Element to free.
        Element *ElementToFree;
        // Iterates through the list.
        Element *currentElement = memory.head;
        while (currentElement != NULL) {
            // Checks if block inside the currentlyElement
            if (block >= currentElement->ptr && block < (currentElement->ptr + currentElement->size)) {
                ElementToFree = currentElement;
                break;
            }
            currentElement = currentElement->next;
        }

        // Checks if the Element requested to free is already is FREE.
        if (ElementToFree->alloc == FREE) {
            #if DEBUGGER == 1
            printf("ERROR: Element that is wished to free is already free");
            #endif
            return;
        } else {
            ElementToFree->alloc = FREE;
        }

        // Checks if ElementToFree's prev Element isn't NULL and it is FREE.
        if (ElementToFree->prev != NULL) {
            if (ElementToFree->prev->alloc == FREE) {
                freeInclElementsPrev(ElementToFree);
            }
        }
            // Check is ElementToFree's next Element isn't NULL and it is FREE.
        else if (ElementToFree->next != NULL) {
            if ( ElementToFree->next->alloc == FREE) {
                freeInclElementsNext(ElementToFree);
            }
        }
            // There is no FREE Element's on either sides of ElementToFree.
        else {
            #if DEBUGGER == 1
            printf("Element that is wished to free dont needed merge to be freed\n"
                   "Element: %p, Allocated: %d\n", ElementToFree, ElementToFree->alloc);
            #endif
        }

    } else {
        #if DEBUGGER == 1
        printf("ERROR: Element that is wished to free is outside of the controlled memory\n");
        #endif
        return;
    };

}