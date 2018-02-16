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
	int i;

	for (i = 0; i < 10; i++)
	{
		print_dec(i);
		print("\n");
	}
}