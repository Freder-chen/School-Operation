#include <stdio.h>
#include <time.h>

int get_mcd_BF(int a, int b)
{
    int temp;
    for (temp = a > b ? b : a; temp; temp--)
    	if (a % temp == 0 && b % temp == 0)
        	break;
    return temp;
}

int get_mcd_GCD(int a, int b)
{
	if (b == 0) return a;

	return get_mcd_GCD(b, a % b);
}

int get_mcd_count(int a, int b, int *count)
{
	if (b == 0) return a;

	(*count)++;
	
	return get_mcd_count(b, a % b, count);
}

int get_mcd_GCD2(int a, int b)
{
	if (b == 0) return a;

	return get_mcd_GCD2(b, a - b);
}

int f(int n, int *count){  
    if(n == 1 || n == 2) return 1;

    (*count)++;
    
    return f(n-1, count) + f(n-2, count);
}


int main()
{
	int count = 0;
	clock_t start, finish;

	int i, time_list[40], count_list[40];
	for (i = 0; i < 40; i++)
	{
		start = clock();
		int num = f(i + 1, &count);
		finish = clock();
		double time = (double)(finish - start) / CLOCKS_PER_SEC;
		printf("%d\t%lf\n", num, time);
	}
	return 0;
}