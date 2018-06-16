// Begin: 1900  1 1
// End  : 2001 11 4
#include <stdio.h>

enum Mouth {
	CommonFeb = 28,
	LeapFeb = 29,
	Small = 30,
	Large = 31
};

struct Year { enum Mouth mouth[12]; }
CommonYear = {Large, CommonFeb, Large, Small, Large, Small, Large, Large, Small, Large, Small, Large},
LeapYear   = {Large, LeapFeb,   Large, Small, Large, Small, Large, Large, Small, Large, Small, Large};

struct Current {
	int year;
	int mouth;
	int day;
};

void print_year(const struct Year year)
{
	int i;
	for (i = 0; i < 12; i++)
		printf("%d ", year.mouth[i]);
	printf("\n");
}

int main()
{
	print_year(LeapYear);
}