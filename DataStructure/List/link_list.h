#ifndef __LINK_LIST_H__
#define __LINK_LIST_H__

typedef int Element;

struct LinkElem {
	Element data;
	struct LinkElem *next;
};

struct LinkList {
	struct LinkElem *head;
	int len;
};

// new
struct LinkList *new_list();

// free
void free_list(struct LinkList *list);

/* 插入
 * pos: [0, n]
 */
void insert_elem(struct LinkList *list, int pos, Element elem);

/* 删除
 * pos: [0, n]
 */
void delete_elem(struct LinkList *list, int pos);

/* 更改
 */
void update_elem(struct LinkList *list, int pos, Element elem);
/* 查找
 */
Element get_elem(struct LinkList *list, int pos);
int find_elem(struct LinkList *list, Element elem);

#endif