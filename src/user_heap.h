#ifndef USER_HEAP_H
#define USER_HEAP_H

#include "kheap.h"

/**
   This is for the creation, management, and placement of user heaps
**/

#define UHEAP_INITIAL 0x20000 // 8K randomly selected size
#define UHEAP_MAX 0x30000 // 16K Double that ^ 'cause why not?
#define UHEAP_INDEX_SIZE 0x4000

typedef struct {
	/**
	   the index maps heap ids to process ids.
	   Ex: if index[0] == 1, then process 1 owns heap 0
	**/
	u32int* index;
	
	/**
	   Locations maps the heap id to the memory location of the heap.
	**/
	u32int* locations;

	/**
	   Size records the max number of heaps allowed.
	**/
	u32int size;
} heap_map_t;

/**
   Create and initialize the data structures for the heaps
**/
void initialize_uheaps(u32int num_of_heaps);

/**
   Find a unused heap and return it
**/
u32int get_free_uheap();

/**
   Return the heap pointer given the map index
**/
heap_t* get_uheap_pointer(u32int idx);

/**
   Sets a heap to be free
**/
void free_uheap(u32int idx);

/**
   Creates a new heap for a process
**/
heap_t* create_uheap(u32int pid, u8int supervisor, u8int readonly);

/**
   Copies src heap into destination heap
**/
void clone_heap(heap_t* src, heap_t* dst);

#endif