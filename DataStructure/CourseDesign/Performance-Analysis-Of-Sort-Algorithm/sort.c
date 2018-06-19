#include "sort.h"
#include <stdlib.h>
#include <stdio.h>

/* Low level handing */

void _swap(SortElement *elem1, SortElement *elem2)
{
	SortElement temp = *elem1;
	*elem1 = *elem2;
	*elem2 = temp;
}

void _add_times(struct Analysis *analysis, long long comp_times, long long move_times)
{
	analysis->comp_times += comp_times;
	analysis->move_times += move_times;
}

/* Brute force */

// 插入排序
struct Analysis insert_sort(SortElement *arr, int len)
{
	struct Analysis analysis = {0, 0};

	int i, j;
	for (i = 1; i < len; i++)
	{
		_add_times(&analysis, 0, 2);
		SortElement temp = arr[i];
		for (j = i; j > 0; j--)
		{
			_add_times(&analysis, 1, 0);
			if (temp < arr[j - 1])
			{
				_add_times(&analysis, 0, 1);
				arr[j] = arr[j - 1];
			}
			else break;
		}
		arr[j] = temp;
	}

	return analysis;
}

// 冒泡排序
struct Analysis bubble_sort(SortElement *arr, int len)
{
	struct Analysis analysis = {0, 0};

	int i, j;
	for (i = 0; i < len - 1; i++)
	{
		for (j = 0; j < len - i - 1; j++)
		{
			if (arr[j] > arr[j + 1])
			{
				_swap(&arr[j], &arr[j + 1]);
				_add_times(&analysis, 0, 3);
			}
			_add_times(&analysis, 1, 0);
		}
	}

	return analysis;
}

// 快速排序
void _quick_sort_recursive(SortElement *arr, int start, int end, struct Analysis *analysis)
{
	if (start >= end) return;

	int mid = end, left = start, right = end - 1;

	while (left < right)
	{
		while (arr[left] < arr[mid] && left < right)
		{
			left++;
			_add_times(analysis, 1, 0);
		}
		while (arr[right] >= arr[mid] && left < right)
		{
			right--;
			_add_times(analysis, 1, 0);
		}
		_swap(&arr[left], &arr[right]);
		_add_times(analysis, 2, 3);
	}
	if (arr[left] >= arr[end])
	{
		_swap(&arr[left], &arr[end]);
		_add_times(analysis, 0, 3);
	}
	else
	{
		left++;
	}
	_add_times(analysis, 1, 0);

	if (left) _quick_sort_recursive(arr, start, left - 1, analysis);
	_quick_sort_recursive(arr, left + 1, end, analysis);
}
struct Analysis quick_sort(SortElement *arr, int len)
{
	struct Analysis analysis = {0, 0};
	_quick_sort_recursive(arr, 0, len - 1, &analysis);
	return analysis;
}

// 选择排序
struct Analysis select_sort(SortElement *arr, int len)
{
	struct Analysis analysis = {0, 0};

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
			_add_times(&analysis, 1, 0);
		}
		_swap(&arr[i], &arr[temp]);
		_add_times(&analysis, 0, 3);
	}

	return analysis;
}

// 堆排序
void _max_heapify(int arr[], int start, int end, struct Analysis *analysis)
{
	int dad = start;
	int son = dad * 2 + 1;
	while (son <= end)
	{
		_add_times(analysis, 2, 0);

		if (son + 1 <= end && arr[son] < arr[son + 1])
		{
			son++;
		}

		if (arr[dad] > arr[son])
		{
			return;
		}
		else
		{
			_swap(&arr[dad], &arr[son]);
			dad = son;
			son = dad * 2 + 1;
			_add_times(analysis, 0, 3);
		}
	}
}
struct Analysis heap_sort(SortElement *arr, int len)
{
	struct Analysis analysis = {0, 0};

	int i;
	for (i = len / 2 - 1; i >= 0; i--)
	{
		_max_heapify(arr, i, len - 1, &analysis);
	}
	for (i = len - 1; i > 0; i--)
	{
		_swap(&arr[0], &arr[i]);
		_max_heapify(arr, 0, i - 1, &analysis);
		_add_times(&analysis, 0, 3);
	}

	return analysis;
}

// 归并排序
void _merge(SortElement *array, int start, int middle, int end, struct Analysis *analysis)
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
		_add_times(analysis, 1, 1);
	}
	for (i = lp; i <= middle; i++)
	{
		arr[ap++] = array[lp++];
		_add_times(analysis, 0, 1);
	}
	for (i = rp; i <= end; i++)
	{
		arr[ap++] = array[rp++];
		_add_times(analysis, 0, 1);
	}

	// Copy
	for (i = start; i <= end; i++)
	{
		array[i] = arr[i - start];
		_add_times(analysis, 0, 1);
	}

	free(arr);
}
void _merge_sort(SortElement *arr, int start, int end, struct Analysis *analysis)
{
	if (start < end)
	{
		int middle = (start + end) / 2;
		_merge_sort(arr, start, middle, analysis);
		_merge_sort(arr, middle + 1, end, analysis);
		_merge(arr, start, middle, end, analysis);
	}
}
struct Analysis merge_sort(SortElement *array, int len)
{
	struct Analysis analysis = {0, 0};
	_merge_sort(array, 0, len - 1, &analysis);
	return analysis;
}
