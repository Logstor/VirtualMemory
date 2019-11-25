#pragma once

/*
 * Structs
 */

typedef struct Element
{
    struct Element* next;
    struct Element* prev;

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

    size_t bytesFree;
}Memory;