#include "./sort.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void _putchars(char c, int n)
{
	while (n--) putchar(c);
}

int *_get_random(int len)
{
	int *random = (int *)malloc(sizeof(int) * len);
	
	srand(time(0));
	while (len--)
	{
		random[len] = rand();
	}

	return random;
}

void _sort_test(char *func_name, char *func_stability, struct Analysis (*sort_func)(SortElement *arr, int len))
{
	int len = 50000;
	SortElement *arr = _get_random(len);
	
	clock_t start_t = clock();
	struct Analysis analysis = sort_func(arr, len);
	clock_t end_t = clock();
	
	_putchars('-', 62);
	printf("\n|%-10s|%13d|%13d|%10lf|%10s|\n",
		func_name,
		analysis.comp_times,
		analysis.move_times,
		(double)(end_t - start_t) / CLOCKS_PER_SEC,
		func_stability);

	free(arr);
}

int main()
{
	_putchars('-', 62);
	printf("\n|%-10s|%13s|%13s|%10s|%10s|\n", "Name", "Comp Times", "Move Times", "Time", "Stability");
	_sort_test("Insert", "Stable", insert_sort);
	_sort_test("Bubble", "Stable", bubble_sort);
	_sort_test("Quick", "Unstable", quick_sort);
	_sort_test("Select", "Unstable", select_sort);
	_sort_test("Heap", "Unstable", heap_sort);
	_sort_test("Merge", "Stable", merge_sort);
	_putchars('-', 62);

	return 0;
}