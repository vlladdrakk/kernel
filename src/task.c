#include "task.h"
#include "kheap.h"
#include "paging.h"
#include "user_main.h"

extern heap_t* kheap;
extern heap_t* current_heap;
extern page_directory_t* kernel_directory;
extern page_directory_t* current_directory;
extern initial_esp;
// Called like: exec_switch(eip, current_directory->physicalAddr, ebp, esp)
extern void exec_switch(u32int, u32int, u32int, u32int);
extern u32int read_eip();

task_t* current_task = 0;
task_t* ready_queue = 0;

u32int next_pid = 1;

void initialize_tasking()
{
	asm volatile("cli");
	// allocate memory for the current task on the kernel heap
	current_task = (task_t*) kmalloc(sizeof(task_t));

	// Initialize task
	current_task->id = 0;
	current_task->next = 0;
	current_task->prev = 0;
	current_task->age = 0;

	// The page directory for this task is the kernel directory
	current_task->directory = clone_directory(kernel_directory);
	current_directory = current_task->directory;
	switch_page_directory(current_directory);

	u32int heap_start = 0x7EFE000 - 0x42000;
	u32int heap_end = heap_start +  0x10000;
	monitor_write("here\n");
	monitor_write_hex(current_directory->physicalAddr);
	monitor_write("\n");

	int i;
	for (i = heap_start; i < heap_end; i += 0x1000)
		alloc_frame(get_page(i, 1, current_directory), 0, 1);


	current_task->heap = create_heap(heap_start, heap_end, heap_end + 0x30000, 0, 0);

		// Set the esp and ebp for the task
	current_task->regs.esp = 0x7EFE000 - 0x2000;
	current_task->regs.ebp = current_task->regs.esp;
	current_task->regs.eip = (u32int)user_main;

	current_heap = current_task->heap;

	exec_switch(current_task->regs.eip, current_directory->physicalAddr, current_task->regs.ebp, current_task->regs.esp);
	asm volatile("sti");
}

/**
   Duplicates the current task, returns 0 to the child and the childs pid to the parent
**/
int fork()
{
	// disable interrupts
	asm volatile("cli");

	// Keep link to parent
	task_t* parent = current_task;
	task_t* child = (task_t*) kmalloc(sizeof(task_t));

	// Copy data from parent task
	child->directory = clone_directory(parent->directory);
	child->regs = parent->regs;
	child->priority = parent->priority;
	child->id = next_pid++;
	child->user_esp = parent->user_esp;
	// child->heap = (heap_t*)create_uheap(child->id, 0, 0);

	// create new stack, set esp and ebp
	u32int stack_phys;
	child->user_esp = (u32int)kmalloc_ap(STACK_SIZE, &stack_phys);
	child->regs.esp = child->regs.ebp = stack_phys;

	// find end of ready_queue
	task_t* end_task = ready_queue;
	while (end_task->next)
		end_task = (task_t*)end_task->next;
	// add new task to queue
	end_task->next = (struct task_t*)child;

	// Entry point for new process
	u32int eip = read_eip();

	if (current_task == parent)
	{
		// Configure childs registers
		u32int esp;
        u32int ebp;

        asm volatile("mov %%esp, %0" : "=r"(esp));
        asm volatile("mov %%ebp, %0" : "=r"(ebp));

        child->regs.esp = esp;
        child->regs.ebp = ebp;
        child->regs.eip = eip;

        // enable interrupts
        asm volatile("sti");

        return child->id;
	}
	else
	{
		// This is the child process
		return 0;
	}
	
}

void switch_task()
{
	// copy the stack pointer and base pointer
	u32int esp, ebp;
	asm volatile("mov %%esp, %0":: "r"(esp));
	asm volatile("mov %%ebp, %0":: "r"(ebp));

	u32int eip = read_eip();

	if (eip == 0x12345)
		return; // task just switched

	// store registers
	current_task->regs.esp = esp;
	current_task->regs.ebp = ebp;
	current_task->regs.eip = eip;

	// get the next task
	current_task = (task_t*)current_task->next;

	if (!current_task)
		current_task = ready_queue;

	// reset register values
	esp = current_task->regs.esp;
	ebp = current_task->regs.ebp;
	eip = current_task->regs.eip;

	// set user heap
	// uheap = current_task->heap;

	// set the new page directory
	current_directory = current_task->directory;
	// switch_page_directory(current_task->directory);

	exec_switch(eip, current_directory->physicalAddr, ebp, esp);
}
