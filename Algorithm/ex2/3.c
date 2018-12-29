#include <stdio.h>

void swap(int *x,int *y)
{
     int temp;
     temp = *x;
     *x = *y;
     *y = temp;

}
int select(int *arr, int left, int right, int k)
{
    if (left >= right) return arr[left];
    int i = left;
    int j = right + 1;
    int pivot = arr[left];
    while (true) 
    {
        do {
            i = i + 1;
        } while (arr[i] < pivot);
        do {
            j = j - 1;
        } while (arr[j] > pivot);
        if (i >= j) break;
        swap(&arr[i], &arr[j]);
    }
    if (j - left + 1 == k)
        return pivot;
    arr[left] = arr[j];
    arr[j] = pivot;
    if (j - left + 1 < k)
        return select(arr, j+1, right, k-j+left-1);
    else
        return select(arr, left, j-1, k);
}

int main()
{
    int arr[] = {0, 1, 4, 3, 7, 5, 6};
    int len = sizeof(arr) / sizeof(int);
    int k = 5;
    int min = select(arr, 0, len - 1, k);
    if (min != -1) printf("min(%d) =%d\n", k, min);

    return 0;
}

