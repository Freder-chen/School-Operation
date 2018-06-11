#ifndef __BALANCED_TREE__
#define __BALANCED_TREE__

enum TreeStatus
{
	Balanced = 1,
	None = 0,
};

struct Node {
	int data;
	struct Node *lchild;
	struct Node *rchild;
};

struct Tree {
	struct Node *root;
};

struct Tree *creat_binary_sort_tree(int *arr, int len);

struct Tree *create_AVL_tree(int *arr, int len);

void free_tree(struct Tree *tree);

void traverse_tree(struct Tree *tree);

float average_length_of_search(struct Tree *tree);

void del_element(struct Tree *tree, int element);

void transfrom_tree(struct Tree *tree);

#endif