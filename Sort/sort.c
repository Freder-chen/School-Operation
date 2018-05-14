#include "sort.h"
#include <stdlib.h>

/* Low level handing */

void _swap(SortElement *elem1, SortElement *elem2)
{
	SortElement temp = *elem1;
	*elem1 = *elem2;
	*elem2 = temp;
}

/* Brute force */

void bubble_sort(SortElement *arr, int len)
{
	int i, j;

	for (i = 0; i < len - 1; i++)
	{
		for (j = 0; j < len - i - 1; j++)
		{
			if (arr[j] > arr[j + 1])
			{
				_swap(&arr[j], &arr[j + 1]);
			}
		}
	}
}

void select_sort(SortElement *arr, int len)
{
	int i, j;
	for (i = 0; i < len; i++)
	{
		int temp = i;
		for (j = i + 1; j < len; j++)
		{
			if (arr[temp] > arr[j])
			{
				temp = j;
			}
		}
		_swap(&arr[i], &arr[temp]);
	}
}

/* Insert sort is used in hill sort */
void _insert_sort(SortElement *arr, int len, int gap)
{
	SortElement temp;
	int i, j;
	for (i = gap; i < len; i++)
	{
		temp = arr[i];
		for (j = i - gap; j >= 0 && arr[j] > temp; j -= gap)
		{
			arr[j + gap] = arr[j];
		}
		arr[j + gap] = temp;
	}
}

void insert_sort(SortElement *arr, int len)
{
	_insert_sort(arr, len, 1);
	// int i, j;
	// for (i = 1; i < len; i++)
	// {
	// 	SortElement temp = arr[i];
	// 	for (j = i; j > 0; j--)
	// 	{
	// 		if (temp < arr[j - 1])
	// 		{
	// 			arr[j] = arr[j - 1];
	// 		}
	// 		else break;
	// 	}
	// 	arr[j] = temp;
	// }
}

void hill_sort(SortElement *arr, int len)
{
	int gap;
	for (gap = len / 2; gap > 0; gap /= 2)
	{
		_insert_sort(arr, len, gap);
	}
}

/* Dividing and treating */

void _quick_sort_recursive(SortElement *arr, int start, int end)
{
	if (start >= end) return;

	int mid = start, left = start + 1, right = end;

	while (left < right)
	{
		while (arr[left] < arr[mid] && left < right)
		{
			left++;
		}
		while (arr[right] > arr[mid] && left < right)
		{
			right--;
		}
		_swap(&arr[left], &arr[right]);
	}

	// left == right
	if (arr[mid] > arr[left])
	{
		_swap(&arr[mid], &arr[left]);
	}

	_quick_sort_recursive(arr, start, left - 1);
	_quick_sort_recursive(arr, right, end);
}

void quick_sort(SortElement *arr, int len)
{
	_quick_sort_recursive(arr, 0, len - 1);
}

void _merge(SortElement *array, int start, int middle, int end)
{
	int i;
	SortElement *arr = (SortElement *)malloc(sizeof(SortElement) * (end - start + 1));
	int ap = 0, lp = start, rp = middle + 1;

	// Sort
	while (lp <= middle && rp <= end)
	{
		if (array[lp] > array[rp])
		{
			arr[ap++] = array[lp++];
		}
		else
		{
			arr[ap++] = array[rp++];
		}
	}
	for (i = lp; i <= middle; i++)
	{
		arr[ap++] = array[lp++];
	}
	for (i = rp; i <= end; i++)
	{
		arr[ap++] = array[rp++];
	}

	// Copy
	for (i = start; i <= end; i++)
	{
		array[i] = arr[i - start];
	}

	free(arr);
}

void _merge_sort(SortElement *arr, int start, int end)
{
	if (start < end)
	{
		int middle = (start + end) / 2;
		_merge_sort(arr, start, middle);
		_merge_sort(arr, middle + 1, end);
		_merge(arr, start, middle, end);
	}
}

void merge_sort(SortElement *arr, int len)
{
	_merge_sort(arr, 0, len - 1);
}