#pragma once

#include "memoryStructure.h"

#define ALLOCATED 1
#define FREE  0

#define DEBUGGER 0

// Prototypes
void *bestFitMalloc(size_t requested);
void bestFree(void* block);