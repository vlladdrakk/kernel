#include "user_main.h"
#include "kernel_ken.h"

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
	// test reading and writing from allocated array
	int i;
	int* arr = (int*) alloc(sizeof(int)*10, 1);
	print("arr location: ");
	print_hex((unsigned int) arr);

	for (i = 0; i < 10; i++)
		arr[i] = i*2;

	print("\nNew allocated array:\n");
	print_arr(arr, 10);

	// test freeing memory
	free(arr);
	print("After freeing:\n");
	print_arr(arr, 10);

	print("Allocating a new int (val=33)\n");
	int* val = (int*) alloc(sizeof(int), 1);
	*val = 33;

	print_arr(arr, 10);
}