#include "user_main.h"
#include "kernel_ken.h"
#pragma src
void print_arr(int* arr, int size) {
	int i;
	for (i = 0; i < size; i++)
	{
		print_hex(arr[i]);
		print(", ");
	}
	print("\n");
}

void user_main()
{
	print("In user code\n");
	unsigned int ins;
	asm("mov %%esp, %0": "=r"(ins));
	print("user esp: ");
	print_hex(ins);
	print("\n");
}