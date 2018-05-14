#ifndef __SEQUENCE_LIST_H__
#define __SEQUENCE_LIST_H__

typedef int Element;

struct SequenceList {
	Element *data;
	int len;
	int size;
};

// new
struct SequenceList *new_list();
// free
void free_list(struct SequenceList *list);
// 插入
void insert_elem(struct SequenceList *list, int pos, Element elem);
// 删除
void delete_elem(struct SequenceList *list, int pos);
// 更改
void update_elem(struct SequenceList *list, int pos, Element elem);
// 查找
Element get_elem(struct SequenceList *list, int pos);
int find_elem(struct SequenceList *list, Element elem);

#endif
