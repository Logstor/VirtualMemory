#pragma once

#include "memoryStructure.h"

/*
*Prototypes
*/
void clean();

void* getMemPool();
int getMemTotal();
int getMemHoles();
int getMemAllocated();
int getMemFree();
int getMemLargestFree();
int getMemSmallFree(size_t size);
char isMemAlloc(void *ptr);

Element* findByAddress(void* ptr);
Element* allocateBlock(Element* space, size_t size);