#ifndef TASK_H
#define TASK_H

#include "common.h"
#include "paging.h"
#include "kheap.h"
/**
   Create and manage task operations
**/

#define STACK_SIZE 0x800

typedef struct {
	u32int eip;
	u32int esp;
	u32int ebp;
} reg_t;

typedef struct {
	u8int age;
	u32int id;
	u8int priority;
	u32int user_esp;
	reg_t regs;
	page_directory_t* directory;
	heap_t* heap;
	struct task_t* next;
	struct task_t* prev;
} task_t;

/**
   Setup everything for new tasks
**/
void initialize_tasking();

/**
   Copies values from current task to create the new task
**/

task_t* create_task();

#endif