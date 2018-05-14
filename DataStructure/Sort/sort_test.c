#include "sort.h"
#include <stdio.h>
#include <string.h>

/* Low level handing */

void _print_arr(SortElement *arr, int len, char *tips)
{
	printf("%s:", tips);
	int i;
	for (i = 0; i < len; i++)
	{
		printf(" %d", arr[i]);
	}
	printf("\n");
}

void _sort_test(char *func_name, void (*sort_func)(SortElement *arr, int len))
{
	SortElement arr[] = {2, 1, 4, 3 };
	int len = sizeof(arr) / sizeof(SortElement);

	char tips[100];
	
	_print_arr(arr, len, strcat(strcpy(tips, func_name), " sort test begin"));

	sort_func(arr, len);
	
	_print_arr(arr, len, strcat(strcpy(tips, func_name), " sort test end"));
}

/* Sort test handing */

void bubble_sort_test() { _sort_test("Bubble", bubble_sort); }

void select_sort_test() { _sort_test("Select", select_sort); }

void insert_sort_test() { _sort_test("Insert", insert_sort); }

void hill_sort_test() { _sort_test("Hill", hill_sort); }

void quick_sort_test() { _sort_test("Quick", quick_sort); }

void merge_sort_test() { _sort_test("Merge", merge_sort); }

int main()
{
	bubble_sort_test();
	select_sort_test();
	insert_sort_test();
	hill_sort_test();
	quick_sort_test();
	merge_sort_test();

	return 0;
}