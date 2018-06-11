#include "my_string.h"
#include <stdlib.h>
#include <malloc.h>

// src  - source
// dest - destination
#define INIT_SIZE 10

/* 新建一个字符串实例 */
String new_str()
{
	String str = (String)malloc(INIT_SIZE * sizeof(char));
	str[0] = '\0';
	return str;
}

/* 释放一个字符串实例 */
void free_str(String str)
{
	// 释放 str
	free(str);
}

/* 求字符串长度 */
size_t strlen(String str)
{
	size_t i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	return i;
}

enum Status insert_char(String *str, char ch, size_t index)
{
	size_t size = _msize(*str);
	size_t len = strlen(*str);

	if (len >= size)
	{
		String s = (String)realloc(*str, len + INIT_SIZE);
		if (s == NULL)
		{
			printf("Realloc failed.\n");
			return Error;
		}
		*str = s;
	}
	if (index > len || index < 0)
	{
		printf("The length of string is %d, cannot insert character in %d.\n", len, index);
		return Error;
	}

	size_t i;
	for (i = len + 1; i > index; i--)
	{
		(*str)[i] = (*str)[i - 1];
	}
	(*str)[index] = ch;
	(*str)[len + 1] = '\0';

	return Succeed;
}

enum Status delete_char(String *str, size_t index)
{
	size_t len = strlen(*str);

	if (index >= len || index < 0)
	{
		printf("The length of string is %d, cannot delete character in %d.\n", len, index);
		return Error;
	}

	size_t i;
	for (i = index; i < len; i++)
	{
		(*str)[i] = (*str)[i + 1];
	}

	return Succeed;
}

enum Status get_char(String *str, size_t index, char *dech)
{
	size_t len = strlen(*str);

	if (index >= len || index < 0)
	{
		printf("The length of string is %d, cannot get character in %d.\n", len, index);
		*dech = 0;
		return Error;
	}

	*dech = (*str)[index];
	return Succeed;
}

/* 分配 */
enum Status strcopy(String *dest, String src)
{
	size_t src_len = strlen(src);
	size_t dest_len = strlen(*dest);
	size_t dest_size = _msize(*dest);

	if (dest_size < src_len)
	{
		String s = (String)realloc(*dest, src_len + INIT_SIZE);
		if (s == NULL)
		{
			printf("Realloc failed.\n");
			return Error;
		}
		*dest = s;
	}

	size_t i;
    for(i = 0; i <= src_len; i++)
    {
    	(*dest)[i] = src[i];
    }

	return Succeed;
}

/* 连接 */
enum Status strconcat(String *dest, String src)
{
	size_t src_len = strlen(src);
	size_t dest_len = strlen(*dest);
	size_t dest_size = _msize(*dest);

	if (dest_size < dest_len + src_len)
	{
		String s = (String)realloc(*dest, dest_len + src_len + INIT_SIZE);
		if (s == NULL)
		{
			printf("Realloc failed.\n");
			return Error;
		}
		*dest = s;
	}

	size_t i;
    for(i = 0; i <= src_len; i++)
    {
    	(*dest)[i + dest_len] = src[i];
    }

	return Succeed;
}

/* 子串 */
enum Status substr(String *dest, String src, size_t start, size_t len)
{
	size_t src_len = strlen(src);
	size_t dest_size = _msize(*dest);

	if (start + len > src_len || start < 0)
	{
		printf("The length of source string is %d, cannot start at %d.(start+len=%d)\n", src_len, start, start + len);
		return Error;
	}
	if (dest_size < len)
	{
		String s = (String)realloc(*dest, len + INIT_SIZE);
		if (s == NULL)
		{
			printf("Realloc failed.\n");
			return Error;
		}
		*dest = s;
	}

	size_t i;
    for(i = 0; i < len; i++)
    {
    	(*dest)[i] = src[i + start];
    }
    (*dest)[len] = '\0';

	return Succeed;
}

/* 比较 */
enum Status strequal(String str1, String str2)
{
	size_t len1 = strlen(str1);
	size_t len2 = strlen(str2);

	if (len1 != len2) return False;

	size_t i;
	for(i = 0; i < len1; i++)
    {
    	if (str1[i] == str2[i])
    	{
    		continue;
    	}
    	return False;
    }
    
	return True;
}