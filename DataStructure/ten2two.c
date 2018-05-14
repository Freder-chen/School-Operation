#include <stdio.h>

void ten2two(char *dest, unsigned long long src)
{
	const char zero = '0';
	int digit = 0, i = 0;
	while (src / (1 << digit) != 0)
	{
		digit++;
	}
	while (--digit >= 0)
	{
		dest[i++] = src / (1 << digit) + zero;
		src -= src / (1 << digit) * (1 << digit);
	}
	dest[i] = '\0';
}


int main()
{
	char a[100];
	ten2two(a, 9);
	printf("%s\n", a);
	return 0;
}