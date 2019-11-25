#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "utility.h"
#include "memoryStructure.h"

/*
* Prototypes
*/
void firstInit(size_t size);

void* firstMalloc(size_t requested);
void firstFree(void* block);

/*
* Implementations
*/

