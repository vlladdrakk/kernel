#include "kernel_ken.h"
#include "monitor.h"
#include "common.h"
#include "kheap.h"

void* alloc(u32int size, u8int page_align)
{
	//borked
}

void free(void* ptr)
{
	// kfree(ptr, uheap);
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
