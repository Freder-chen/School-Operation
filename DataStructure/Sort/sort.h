#ifndef __SORT__
#define __SORT__

#define SortElement int

// 冒泡排序
void bubble_sort(SortElement *arr, int len);

// 选择排序
void select_sort(SortElement *arr, int len);

// 插入排序
void insert_sort(SortElement *arr, int len);

// 希尔排序
void hill_sort(SortElement *arr, int len);

// 快速排序
void quick_sort(SortElement *arr, int len);

// 堆排序
void heap_sort(SortElement *arr, int len);

// 归并排序
void merge_sort(SortElement *array, int len);

#endif