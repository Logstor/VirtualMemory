#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "mymem.h"
#include <time.h>


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

// static struct memoryList *head;
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
}

void* nextFitMalloc(size_t request)
{

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
	            return NULL;
	  case Best:
	            return NULL;
	  case Worst:
	            return NULL;
	  case Next:
	            return nextFitMalloc(requested);
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
 * Merges the current block with the next no 
 * matter if it's free or not.
 */
void mergeForward(struct memoryList* block)
{
	struct memoryList* nextBlock;

	// Check for null
	if (block == NULL)
	{
		printf("WARNING: Trying to forward merge a NULL-block - mergeForward()\n");
	}
	else if (block->next == NULL)
	{
		printf("WARNING: Next block is NULL - mergeForward()\n");
	}
	// When nothing is null then merge
	else
	{
		// Merge
		nextBlock = block->next;
		block->next = nextBlock->next;
		block->size += nextBlock->size;

		// Free memory from consumed block
		free(nextBlock);
	}
}

/**
 * Merges the current block with the previous no 
 * matter if it's free or not.
 */
void mergeBackwards(struct memoryList* block)
{
	struct memoryList* lastBlock;

	// Check for null
	if (block == NULL)
	{
		printf("WARNING: Trying to backwards merge a NULL-block - mergeBackwards()\n");
	}
	else if (block->next == NULL)
	{
		printf("WARNING: Last block is NULL - mergeBackwards()\n");
	}
	// When nothing is null then merge
	else
	{
		// Merge
		lastBlock = block->last;
		block->last = lastBlock->last;
		block->size += lastBlock->size;
		block->ptr = lastBlock->ptr;

		// Free memory
		free(lastBlock);
	}
}

/**
 * This function merges the block recursively until it's 
 * surrounded with allocated blocks.
 */
void mergeBlock(struct memoryList* block)
{
	// Check if block is null
	if (block == NULL)
	{
		printf("WARNING: Block is NULL in mergeBlock()\n");
		return;
	}

	// Check if block is free
	if (block->alloc != 0)
	{
		printf("WARNING: Block is allocated - Can't free - mergeBlock()\n");
		return;
	}

	// Check if next element is free
	if (isNextFree(block))
	{
		// Merge forward
		mergeForward(block);
		// Recursive call
		mergeBlock(block);
	}

	// Check if previous element is free
	else if (isLastFree(block))
	{
		// Merge backwards
		mergeBackwards(block);
		// Recursive call
		mergeBlock(block);
	}

	// Stop
	return;
}

/* Frees a block of memory previously allocated by mymalloc. */
void myfree(void* block)
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
	
	return;
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
	int holes = 0;
	
	// Get first element
	struct memoryList* element = findFirstElement();

	// Go forward and count holes
	while (element != NULL)
	{
		if (element->alloc == 0)
			holes++;
		
		element = element->next;
	}

	// Return the amount of holes
	return holes;
}

/* Get the number of bytes allocated */
int mem_allocated()
{
	int allocMem = 0;

	// Find first element
	struct memoryList* element = findFirstElement();

	// Count allocated memory
	while (element != NULL)
	{
		if (element->alloc != 0)
			allocMem += element->size;

		element = element->next;
	}

	// Return amount of allocated memory
	return allocMem;
}

/* Number of non-allocated bytes */
int mem_free()
{
	int nonAlloc = 0;

	// Find first element
	struct memoryList* element = findFirstElement();

	// Loop through all elements and count nonAlloc
	while (element != NULL)
	{
		if (element->alloc == 0)
			nonAlloc += element->size;
		
		element = element->next;
	}
	
	// Return number of non-Allocated bytes
	return nonAlloc;
}

/* Number of bytes in the largest contiguous area of unallocated memory */
int mem_largest_free()
{
	int large = -1;

	// Find first element
	struct memoryList* element = findFirstElement();

	// Loop through all elements
	while (element != NULL)
	{
		if (element->alloc == 0 || element->size > large)
			large = element->size;

		element = element->next;
	}

	// Return the largest free space
	return large;
}

/* Number of free blocks smaller than "size" bytes. */
int mem_small_free(int size)
{
	int num = 0;

	// Find first element
	struct memoryList* element = findFirstElement();

	// Loop through all elements
	while (element != NULL)
	{
		if (element->alloc == 0 || element->size < size)
			num++;

		element = element->next;
	}

	// Return number of elements found
	return num;
}       

/**
 * Decides whether the memory is allocated or not.
 * @return true or false
 */
char mem_is_alloc(void *ptr) { return findElement(ptr)->alloc; }

/* 
 * Feel free to use these functions, but do not modify them.  
 * The test code uses them, but you may find them useful.
 */


//Returns a pointer to the memory pool.
void *mem_pool()
{
	return myMemory;
}

// Returns the total number of bytes in the memory pool. */
int mem_total()
{
	return mySize;
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
	return;
}

/* Use this function to track memory allocation performance.  
 * This function does not depend on your implementation, 
 * but on the functions you wrote above.
 */ 
void print_memory_status()
{
	printf("%d out of %d bytes allocated.\n",mem_allocated(),mem_total());
	printf("%d bytes are free in %d holes; maximum allocatable block is %d bytes.\n",mem_free(),mem_holes(),mem_largest_free());
	printf("Average hole size is %f.\n\n",((float)mem_free())/mem_holes());
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
	
	initmem(strat,500);
	
	a = mymalloc(100);
	b = mymalloc(100);
	c = mymalloc(100);
	myfree(b);
	d = mymalloc(50);
	myfree(a);
	e = mymalloc(25);
	
	print_memory();
	print_memory_status();
	
}
