#include "link_list.h"
#include <stdio.h>
#include <stdlib.h>

/* Low level handing */

struct LinkElem *_alloc_elem(struct LinkElem *next) {
	struct LinkElem *elem = (struct LinkElem *)malloc(sizeof(struct LinkElem));
	elem->next = next;
	return elem;
}

struct LinkList *_alloc_list() {
	struct LinkList *list = (struct LinkList *)malloc(sizeof(struct LinkList));
	list->head = _alloc_elem(NULL);
	list->len = 0;
	return list;
}

struct LinkElem *_find_elem(struct LinkList *list, int pos, int len, char *user) {
	if (pos >= len) {
		printf("The list length is %d, can't %s the pos of %d.\n", list->len, user, user == "update" ? pos - 1 : pos);
		return NULL;
	}
	struct LinkElem *elem = list->head;
	while (pos-- > 0) {
		elem = elem->next;
	}
	return elem;
}

/* Sequence list handing */

struct LinkList *new_list() {
	return _alloc_list();
}

void free_list(struct LinkList *list) {
	// Free elem
	struct LinkElem *elem = list->head;
	while (elem != NULL) {
		struct LinkElem *e = elem->next;
		free(elem);
		elem = e;
	}
	// Free list
	free(list);
}

void insert_elem(struct LinkList *list, int pos, Element element) {
	struct LinkElem *elem = _find_elem(list, pos, list->len + 1, "insert");
	if (elem == NULL) return;
	struct LinkElem *e = _alloc_elem(elem->next);
	e->data = element;
	elem->next = e;
	list->len++;
}

void delete_elem(struct LinkList *list, int pos) {
	struct LinkElem *elem = _find_elem(list, pos, list->len, "delete");
	if (elem == NULL) return;
	struct LinkElem *e = elem->next;
	elem->next = e->next;
	free(e);
	list->len--;
}

void update_elem(struct LinkList *list, int pos, Element element) {
	struct LinkElem *elem = _find_elem(list, pos + 1, list->len + 1, "update");
	if (elem == NULL) return;
	elem->data = element;
}

Element get_elem(struct LinkList *list, int pos) {
	return _find_elem(list, pos + 1, list->len + 1, "get");
}

int find_elem(struct LinkList *list, Element element) {
	int pos = 0;
	struct LinkElem *elem = list->head->next;
	while (elem != NULL) {
		if (elem->data == element) {
			return pos;
		}
		elem = elem->next;
		pos++;
	}
	return -1;
}

void print_list(struct LinkList *list) {
	struct LinkElem *elem = list->head;
	while (elem->next != NULL) {
		printf("%d\n", elem->next->data);
		elem = elem->next;
	}
}

// Test
// int main()
// {
// 	struct LinkList *list = new_list();
// 	delete_elem(list, 0);
// 	insert_elem(list, 0, 1);
// 	update_elem(list, 2, 10);
// 	print_list(list);
// 	free_list(list);
// 	return 0;
// }