#include "common.h"
#include "kheap.h"
#include "ordered_array.h"
#include "user_heap.h"

/**
   This is the array that will map pids to heap locations
**/
heap_map_t* uheap_map = 0;

/**
   Pull the kernel heap from kheap.c
**/
extern heap_t* kheap;

/**
   Create and initialize the data structures for the heaps
**/
void initialize_uheaps(u32int num_of_heaps)
{
	/**
	   Store the user heap map in the kernel heap
	**/
	uheap_map = (heap_map_t*) kalloc(sizeof(heap_map_t), 0, kheap);
	uheap_map->index = (int*) kalloc(sizeof(int) * num_of_heaps, 0, kheap);
	uheap_map->locations = (u32int*) kalloc(sizeof(u32int) * num_of_heaps, 0, kheap);

	/**
	   Initialize the index to all empty heaps,
	   0 is a free heap because pid 0 (init) uses the kernel heap
	**/
	memset(uheap_map->index, 0x0, num_of_heaps);
	// Initialize the heap locations
	memset(uheap_map->locations, 0x0, num_of_heaps);

	uheap_map->size = num_of_heaps;
}

/**
   Find a unused heap and return it
**/
u32int get_free_uheap()
{
	u32int i;
	for (i = 0; i < uheap_map->size; i++)
		if (uheap_map->index[i] == 0)
			break;

	if (i == uheap_map->size)
	{
		PANIC("No available heaps\n"); // All heaps are being used
	}
	else
	{
		return i;
	}
}

/**
   Return the heap pointer given the map index
**/
heap_t* get_uheap_pointer(u32int idx)
{
	if (uheap_map->locations[idx] == 0x0)
	{
		PANIC("Attempting to retrieve uninitialized heap\n");
	}
	else
	{
		return (heap_t*) uheap_map->locations[idx];
	}
}

/**
   Sets a heap to be free
**/
void free_uheap(u32int idx)
{
	// Mark the heap as unused
	uheap_map->index[idx] = 0;
}

static s8int header_t_less_than(void*a, void *b)
{
   return (((header_t*)a)->size < ((header_t*)b)->size)?1:0;
}

/**
   Creates a new heap for a process
**/
heap_t* create_uheap(u32int pid, u8int supervisor, u8int readonly)
{
	// Init process cannot create a user heap
	ASSERT(pid > 0);

	asm volatile("cli");
	// Acquire a free heap
	u32int idx = get_free_uheap();
	monitor_write("index: ");
	monitor_write_hex(idx);
	monitor_write("\n");
	// Claim the heap
	uheap_map->index[idx] = pid;

	// check if the heap is initialized
	if (uheap_map->locations[idx] == 0x0)
	{ // heap was not initialized
		u32int start_addr = (idx * UHEAP_MAX) + 0x1000; // Calculate the index of the heap (addresses for heaps start at 0x1000)
		u32int end_addr = start_addr + UHEAP_INITIAL;
		u32int max_addr = start_addr + UHEAP_MAX;

		// Create the heap and store the address
		heap_t *heap = (heap_t*)kmalloc(sizeof(heap_t));

		// All our assumptions are made on startAddress and endAddress being page-aligned.
		ASSERT(start_addr%0x1000 == 0);
		ASSERT(end_addr%0x1000 == 0);

		// Initialise the index.
		heap->index = place_ordered_array( (void*)start_addr, UHEAP_INDEX_SIZE, &header_t_less_than);

		// Shift the start address forward to resemble where we can start putting data.
		start_addr += UHEAP_INDEX_SIZE;

		// Make sure the start address is page-aligned.
		if (start_addr & 0xFFFFF000 != 0)
		{
		   start_addr &= 0xFFFFF000;
		   start_addr += 0x1000;
		}
		// Write the start, end and max addresses into the heap structure.
		heap->start_address = start_addr;
		heap->end_address = end_addr;
		heap->max_address = max_addr;
		heap->supervisor = supervisor;
		heap->readonly = readonly;

		// We start off with one large hole in the index.
		header_t *hole = (header_t *)start_addr;
		hole->size = end_addr-start_addr;
		hole->magic = HEAP_MAGIC;
		hole->is_hole = 1;

		// Store the address of the heap
		uheap_map->locations[idx] = (u32int)heap;
	}

	return (heap_t*) uheap_map->locations[idx];

	asm volatile("sti");
}
