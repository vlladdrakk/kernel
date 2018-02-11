#include "kernel_ken.h"
#include "monitor.h"
#include "common.h"
#include "user_heap.h"
#include "kheap.h"

heap_t* uheap = 0;

void* alloc(u32int size, u8int page_align)
{
	if (uheap == 0) // create the user heap if it doesn't already exist
	{
		uheap = create_uheap(0x1, 0, 0);
	}

	if (page_align == 1)
	{
		return (void*)kalloc(size, 1, uheap);
	}
	else
	{
		return (void*)kalloc(size, 0, uheap);
	}
}

void free(void* ptr)
{
	kfree(ptr, uheap);
}

// Output a null-terminated ASCII string to the monitor
void print(const char *c)
{
	monitor_write(c);
}

// Print an unsigned integer to the monitor in base 16
void print_hex(unsigned int n)
{
	monitor_write_hex(n);
}

// Print an unsigned integer to the monitor in base 10
void print_dec(unsigned int n)
{
	monitor_write_dec(n);
}
