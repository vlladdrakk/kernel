#include "kernel_ken.h"
#include "monitor.h"
#include "common.h"
#include "kheap.h"

extern heap_t* current_heap;

void* alloc(u32int size, u8int page_align)
{
	return kalloc(size, page_align, current_heap);
}

void free(void* ptr)
{
	mfree(ptr, current_heap);
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
