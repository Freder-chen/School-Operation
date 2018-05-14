#ifndef __MY_STRING_H__
#define __MY_STRING_H__

#include <stdio.h>

typedef char* String;

enum Status
{
	Succeed = 0,
	True = 1,
	False = -1,
	Error = -2,
};


/* 新建一个字符串实例 */
String new_str();

/* 释放一个字符串实例 */
void free_str(String str);

/* 求字符串长度 */
size_t strlen(String str);

/* 插入字符 */
enum Status insert_char(String *str, char ch, size_t index);

/* 删除字符 */
enum Status delete_char(String *str, size_t index);

// TO-DO
// enum Status replace(String str, String substr);

/* 查找字符 */
enum Status get_char(String *str, size_t index, char *dest_char);

/* 分配 */
enum Status strcopy(String *dest, String src);

/* 连接 */
enum Status strconcat(String *dest, String src);

/* 子串 */
enum Status substr(String *dest, String src, size_t start, size_t len);

/* 比较 */
enum Status strcmp(String str1, String str2);

#endif