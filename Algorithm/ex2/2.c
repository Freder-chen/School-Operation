#include <stdio.h>
#include <stdlib.h>

void _swap(int *elem1, int *elem2)
{
	int temp = *elem1;
	*elem1 = *elem2;
	*elem2 = temp;
}

void _quick_sort_recursive(int *arr, int start, int end)
{
	if (start >= end) return;

	int mid = start, left = start + 1, right = end;
	while (left < right) {
		while (arr[left] < arr[mid] && left < right) {
			left++;
		}
		while (arr[right] > arr[mid] && left < right) {
			right--;
		}
		_swap(&arr[left], &arr[right]);
	}

	// left == right
	if (arr[mid] > arr[left]) {
		_swap(&arr[mid], &arr[left]);
	}

	_quick_sort_recursive(arr, start, left - 1);
	_quick_sort_recursive(arr, right, end);
}

void quick_sort(int *arr, int len)
{
	_quick_sort_recursive(arr, 0, len - 1);
}

int main()
{
	int arr[] = {0, 1, 4, 3, 7, 5, 6};
	int len = sizeof(arr) / sizeof(int);
	quick_sort(arr, len);

	int i;
	for (i = 0; i < len; i++)
		printf("%d\t", arr[i]);
}