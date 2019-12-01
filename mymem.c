#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "mymem.h"
#include <time.h>
#include "nextFit.h"
#include "firstFit.h"
#include "best_fit.h"
#include "worst_fit.h"


struct memoryList* findFirstElement();

/* The main structure for implementing memory allocation.
 * You may change this to fit your implementation.
 */

struct memoryList
{
  // doubly-linked list
  struct memoryList *last;
  struct memoryList *next;

  int size;            // How many bytes in this block?
  char alloc;          // 1 if this block is allocated,
                       // 0 if this block is free.
  void *ptr;           // location of block in memory pool.
};


strategies myStrategy = NotSet;    // Current strategy


size_t mySize;
void *myMemory = NULL;

static struct memoryList *head;
static struct memoryList *next;


/* initmem must be called prior to mymalloc and myfree.

   initmem may be called more than once in a given exeuction;
   when this occurs, all memory you previously malloc'ed  *must* be freed,
   including any existing bookkeeping data.

   strategy must be one of the following:
		- "best" (best-fit)
		- "worst" (worst-fit)
		- "first" (first-fit)
		- "next" (next-fit)
   sz specifies the number of bytes that will be available, in total, for all mymalloc requests.
*/

/**
 * Frees all memory used for bookkeeping.
 */ 
void freeAllBlocks()
{
	// Find first element 
	struct memoryList* element = findFirstElement();

	// Loop through every element and free
	struct memoryList* nextElement;
	while (element != NULL)
	{
		// Get next element
		nextElement = element->next;

		// Free current element
		free(element);

		// Rotate elements
		element = nextElement;
	}

	// Free memory pool
	free(myMemory);
}

/**
 * 
 * @param strategy the strategy to use
 * @param sz the size in bytes
 */
void initmem(strategies strategy, size_t sz)
{
	// Check parameters
	if (strategy == NotSet || sz < 1)
		return;

	myStrategy = strategy;

	switch (myStrategy)
	{
	case Next:
		nextinit(sz);
		break;
	case First:
		firstInit(sz);
		break;
	case Worst:
		firstInit(sz);
		break;
	case Best:
		firstInit(sz);
		break;
	
	default:
		/* all implementations will need an actual block of memory to use */
		mySize = sz;

		if (myMemory != NULL) free(myMemory); /* in case this is not the first time initmem2 is called */

		/* release any other memory you were using for bookkeeping when doing a re-initialization! */
		freeAllBlocks();

		// Allocate memory
		myMemory = malloc(sz);
		
		// Initialize memory management structure
		struct memoryList *memBlock = (struct memoryList*)malloc(sizeof(struct memoryList));
		memBlock->ptr = myMemory;
		memBlock->size = sz;
		memBlock->alloc = 0;
		next = memBlock;
		break;
	}
	
}

/**
 * Allocates a memoryList element on the Heap.
 * @return memoryList*
 */
struct memoryList* makeElement()
{
	//TODO: Implement error handling
	return (struct memoryList*) malloc( sizeof(struct memoryList) );
}

int mergeIntoList(struct memoryList* element)
{
	//TODO: Implement me!
}

void* nextFitMalloc(size_t request)
{
	// Create the new list element
	struct memoryList* element = makeElement();
	
	// Merge element into list
	

	// Update Next pointer
}

/* Allocate a block of memory with the requested size.
 *  If the requested block is not available, mymalloc returns NULL.
 *  Otherwise, it returns a pointer to the newly allocated block.
 *  Restriction: requested >= 1 
 */
void *mymalloc(size_t requested)
{
	assert((int)myStrategy > 0);
	
	switch (myStrategy)
	  {
	  case NotSet: 
	            return NULL;
	  case First:
	            return firstMalloc(requested);
	  case Best:
	            return bestFitMalloc(requested);
	  case Worst:
	            return worstFitMalloc(requested);
	  case Next:
	            return nextMalloc(requested);
	  }
	return NULL;
}

struct memoryList* findElement(void* address)
{
	// Find first element
	struct memoryList* element = findFirstElement();

	// Loop through elements
	while (1)
	{
		//TODO: Doesn't account for next element could be null
		if (address >= element->ptr && address < element->next->ptr)
			return element;
		
		element = element->next;
	}
	
}

/**
 * Check whether the next block of memory is free.
 * @return true if next isn't allocated
 */
int isNextFree(struct memoryList* block)
{
	// Check if next is null
	if (block->next != NULL)
	{
		// Check if next is free
		if (block->next->alloc == 0)
			return 666;
	}

	// Otherwise return false
	return 0;
}

/**
 * Check whether the previous element is free.
 * @return true if previous isn't allocated
 */
int isLastFree(struct memoryList* block)
{
	// Check if previous is null
	if (block->last != NULL)
	{
		// Check if last is free
		if (block->last->alloc == 0)
			return 666;
	}

	// Otherwise return false
	return 0;
}

/**
 * This function merges the block recursively until it's 
 * surrounded with allocated blocks.
 */
void mergeBlock(struct memoryList* block)
{
}

/* Frees a block of memory previously allocated by mymalloc. */
void myfree(void* block)
{
	switch (myStrategy)
	{
	case Next:
		nextFree(block);
		break;
	case First:
		nextFree(block);
		break;
	case Best:
		nextFree(block);
		break;
	case Worst:
		nextFree(block);
		break;
	
	default:
		{
			// Find element
			struct memoryList* element = findElement(block);
			if (element == NULL)
			{
				//TODO: Validate this
				printf("ERROR: Memory doesn't exist!\n");
				assert(0);
				return;
			}

			// Set alloc
			element->alloc = 0;

			// Merge with free blocks
			mergeBlock(element);
			break;
		}
	}
}

/****** Memory status/property functions ******
 * Implement these functions.
 * Note that when refered to "memory" here, it is meant that the 
 * memory pool this module manages via initmem/mymalloc/myfree. 
 */

/**
 * Searches through the elements until it hits the first.
 * @return The first memoryList*
 */
struct memoryList* findFirstElement()
{
	struct memoryList* element = next;
	
	// Loop backwards through elements until it hits first element
	while (1)
	{
		if (element->last == NULL)
			return element;
		else
			element = element->last;
	}
}

/* Get the number of contiguous areas of free space in memory. */
int mem_holes()
{
	return getMemHoles();
}

/* Get the number of bytes allocated */
int mem_allocated()
{
	return getMemAllocated();
}

/* Number of non-allocated bytes */
int mem_free()
{
	return getMemFree();
}

/* Number of bytes in the largest contiguous area of unallocated memory */
int mem_largest_free()
{
	return getMemLargestFree();
}

/* Number of free blocks smaller than "size" bytes. */
int mem_small_free(int size)
{
	return getMemSmallFree(size);
}       

/**
 * Decides whether the memory is allocated or not.
 * @return true or false
 */
char mem_is_alloc(void *ptr) 
{
	return isMemAlloc(ptr);
}

/* 
 * Feel free to use these functions, but do not modify them.  
 * The test code uses them, but you may find them useful.
 */


//Returns a pointer to the memory pool.
void *mem_pool()
{
	return getMemPool();
}

// Returns the total number of bytes in the memory pool. */
int mem_total()
{
	return getMemTotal();
}


// Get string name for a strategy. 
char *strategy_name(strategies strategy)
{
	switch (strategy)
	{
		case Best:
			return "best";
		case Worst:
			return "worst";
		case First:
			return "first";
		case Next:
			return "next";
		default:
			return "unknown";
	}
}

// Get strategy from name.
strategies strategyFromString(char * strategy)
{
	if (!strcmp(strategy,"best"))
	{
		return Best;
	}
	else if (!strcmp(strategy,"worst"))
	{
		return Worst;
	}
	else if (!strcmp(strategy,"first"))
	{
		return First;
	}
	else if (!strcmp(strategy,"next"))
	{
		return Next;
	}
	else
	{
		return 0;
	}
}


/* 
 * These functions are for you to modify however you see fit.  These will not
 * be used in tests, but you may find them useful for debugging.
 */

/* Use this function to print out the current contents of memory. */
void print_memory()
{
	print_memory();
}

/* Use this function to track memory allocation performance.  
 * This function does not depend on your implementation, 
 * but on the functions you wrote above.
 */ 
void print_memory_status()
{
	printf("%d out of %d bytes allocated.\n",getMemAllocated(),getMemTotal());
	printf("%d bytes are free in %d holes; maximum allocatable block is %d bytes.\n",getMemFree(),getMemHoles(),getMemLargestFree());
	printf("Average hole size is %f.\n\n",((float)getMemFree())/getMemHoles());
}

/* Use this function to see what happens when your malloc and free
 * implementations are called.  Run "mem -try <args>" to call this function.
 * We have given you a simple example to start.
 */
void try_mymem(int argc, char **argv) {
        strategies strat;
	void *a, *b, *c, *d, *e;
	if(argc > 1)
	  strat = strategyFromString(argv[1]);
	else
	  strat = First;
	
	
	/* A simple example.  
	   Each algorithm should produce a different layout. */
	/*
	initmem(strat,500);
	
	
	a = mymalloc(100);
	b = mymalloc(100);
	c = mymalloc(100);
	myfree(b);
	d = mymalloc(50);
	myfree(a);
	e = mymalloc(25);
	*/

	initmem(strat, 100);
	a = mymalloc(10);
	b= mymalloc(1);
	myfree(a);
	c = mymalloc(1);
	
	print_memory();
	print_memory_status();
	
}
