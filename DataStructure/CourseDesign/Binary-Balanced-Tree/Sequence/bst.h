#ifndef __BST__
#define __BST__

typedef int Element;

struct Tree {
	Element *nodes;
	int len;
	// size = 1 << len - 1
};

struct Tree *create_tree(Element elems[], int len);

void free_tree(struct Tree *tree);

void traverse_tree(struct Tree *tree);

float average_length_of_search(struct Tree *tree);

void del_element(struct Tree *tree, int element);

#endif