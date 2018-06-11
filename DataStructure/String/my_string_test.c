#include "my_string.h"

int main()
{
	String s = new_str();
	int len = strlen(s);
	int i;
	for (i = 0; insert_char(&s, 'a', i) == Succeed, i < 10; i++)
		printf("i = %d, len = %d\n", i, strlen(s));
	printf("%s\n%d\n", s, len);

	for (i = 0; delete_char(&s, i) == Succeed, i < 10; i++)
		printf("i = %d, len = %d\n", i, strlen(s));
	printf("%s\n%d\n", s, len);

	strcopy(&s, "12345");
	printf("%s\n", s);

	strconcat(&s, "abcde");
	printf("%s\n", s);

	substr(&s, "0123456789", 2, 5);
	printf("%s\n", s);

	enum Status status = strequal("1234", "12345");
	if (status == True) printf("True\n");
	else printf("False\n");

	enum Status status2 = strequal("1234", "1234");
	if (status2 == True) printf("True\n");
	else printf("False\n");

	free_str(s);

	return 0;
}