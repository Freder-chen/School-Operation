#include <stdio.h>
#include <stdlib.h>

void _merge(int *array, int start, int middle, int end)
{
	int i;
	int *arr = (int *)malloc(sizeof(int) * (end - start + 1));
	int ap = 0, lp = start, rp = middle + 1;

	// Sort
	while (lp <= middle && rp <= end) {
		if (array[lp] > array[rp]) {
			arr[ap++] = array[lp++];
		} else {
			arr[ap++] = array[rp++];
		}
	}
	for (i = lp; i <= middle; i++) {
		arr[ap++] = array[lp++];
	}
	for (i = rp; i <= end; i++) {
		arr[ap++] = array[rp++];
	}

	// Copy
	for (i = start; i <= end; i++) {
		array[i] = arr[i - start];
	}

	free(arr);
}

void _merge_sort(int *arr, int start, int end)
{
	if (start < end) {
		int middle = (start + end) / 2;
		_merge_sort(arr, start, middle);
		_merge_sort(arr, middle + 1, end);
		_merge(arr, start, middle, end);
	}
}

void merge_sort(int *arr, int len)
{
	_merge_sort(arr, 0, len - 1);
}

int main()
{
	int arr[] = {0, 1, 4, 3, 7, 5, 6};
	int len = sizeof(arr) / sizeof(int);
	merge_sort(arr, len);
	
	int i;
	for (i = 0; i < len; i++)
		printf("%d\t", arr[i]);

	return 0;
}