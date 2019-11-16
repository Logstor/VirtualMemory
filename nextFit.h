#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
	unsigned int size;
}MemoryPool;

typedef struct
{
	MemoryPool memPool;
	struct memoryList* head;
	struct memoryList* tail;
	struct memoryList* next;
}Memory;

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
