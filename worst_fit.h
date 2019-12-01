#pragma once

#include "memoryStructure.h"

#define ALLOCATED 1
#define FREE  0

#define DEBUGGER 0

// Prototypes of methods.
void *worstFitMalloc(size_t requested);
void worstFree(void* block);
