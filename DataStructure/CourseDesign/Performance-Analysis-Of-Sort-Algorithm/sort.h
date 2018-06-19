#ifndef __SORT__
#define __SORT__

#define SortElement int

struct Analysis {
	long long comp_times;
	long long move_times;
};

// 插入排序
struct Analysis insert_sort(SortElement *arr, int len);

// 冒泡排序
struct Analysis bubble_sort(SortElement *arr, int len);

// 快速排序
struct Analysis quick_sort(SortElement *arr, int len);

// 选择排序
struct Analysis select_sort(SortElement *arr, int len);

// 堆排序
struct Analysis heap_sort(SortElement *arr, int len);

// 归并排序
struct Analysis merge_sort(SortElement *array, int len);

#endif