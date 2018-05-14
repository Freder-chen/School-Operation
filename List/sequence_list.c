	#include "sequence_list.h"
#include <stdio.h>
#include <stdlib.h>

#define LIST_INIT_SIZE 10

/* Low level handing */

static void _expand_list_data(struct SequenceList *list, int add_size) {
	list->size += add_size;
	list->data = (Element *)realloc(list->data, list->size);
}

static int _has_error1(struct SequenceList *list) {
	if (!list) {
		printf("List is null.\n");
		return 1;
	}
	return 0;
}

static int _has_error(struct SequenceList *list, int pos) {
	if (_has_error1(list)) {
		return 1;
	}
	if (pos < 0 || pos > list->len) {
		printf("Position is out of range.\n");
		return 1;
	}
	return 0;
}

/* Sequence list handing */

struct SequenceList *new_list() {
	struct SequenceList *list = (struct SequenceList *)malloc(sizeof(struct SequenceList));
	list->data = (Element *)malloc(sizeof(Element) * LIST_INIT_SIZE);
	list->len = 0;
	list->size = LIST_INIT_SIZE;
	return list;
}

void free_list(struct SequenceList * list) {
	free(list->data);
	free(list);
}

void insert_elem(struct SequenceList *list, int pos, Element elem) {
	if (_has_error(list, pos)) {
		return;
	}

	if (list->len >= list->size) {
		_expand_list_data(list, LIST_INIT_SIZE);
	}
	int i;
	for (i = list->len; i > pos; i--) {
		list->data[i] = list->data[i - 1];
	}
	list->data[pos] = elem;
	list->len++;
}

void delete_elem(struct SequenceList *list, int pos) {
	if (_has_error(list, pos)) {
		return;
	}

	int i;
	for (i = pos; i < list->len - 1; i++) {
		list->data[i] = list->data[i + 1];
	}
	list->len--;
}

void update_elem(struct SequenceList *list, int pos, Element elem) {
	if (_has_error(list, pos)) {
		return;
	}
	list->data[pos] = elem;
}

Element get_elem(struct SequenceList *list, int pos) {
	if (_has_error(list, pos)) {
		// There may have some problem, if return this.
		return list->data[0];
	}
	return list->data[pos];
}

/* return:
 * --- normal: Element index in list.
 * --- -1: Not find the elemnt.
 * --- -2: Problems with parameters.
 */
int find_elem(struct SequenceList *list, Element elem) {
	if (_has_error1(list)) {
		return -2;
	}
	int i;
	for (i = 0; i < list->len; i++) {
		if (list->data[i] == elem) {
			return i;
		}
	}
	return -1;
}

// Test
// int main()
// {
// 	struct SequenceList *list = new_list();
// 	int i;
// 	for (i = 0; i < 15; i++) {
// 		insert_elem(list, 0, i);
// 	}
// 	for (i = 0; i < 15; i++) {
// 		printf("%d, ", list->data[i]);
// 	}
// 	printf("\n");
// 	for (i = 0; i < 5; i++) {
// 		delete_elem(list, 0);
// 	}
// 	for (i = 0; i < 15; i++) {
// 		printf("%d, ", list->data[i]);
// 	}
// 	printf("\n");
// 	return 0;
// }