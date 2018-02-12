// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
//           Made for JamesM's tutorials <www.jamesmolloy.co.uk>

#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "paging.h"
#include "kheap.h"
#include "user_main.h"
#include "multiboot.h"
#include "user_heap.h"
#include "task.h"

u32int initial_esp;

int main(struct multiboot *mboot_ptr, u32int initial_stack)
{
    initial_esp = initial_stack;
    // Initialise all the ISRs and segmentation
    init_descriptor_tables();
    // Initialise the screen (by clearing it)
    monitor_clear();

    initialise_paging();

    initialize_uheaps(16);

    // initialize_tasking();

    monitor_write("Welcome!\n\n");
    
    /**
       Run the user code
    **/
    // user_main();

    return 0;
}
