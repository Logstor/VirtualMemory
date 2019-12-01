#pragma once

#include <stddef.h>

/*
* Prototypes
*/
void firstInit(size_t size);

void* firstMalloc(size_t requested);
void firstFree(void* block);