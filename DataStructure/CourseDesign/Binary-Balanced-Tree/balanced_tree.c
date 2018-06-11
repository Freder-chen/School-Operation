#include "balanced_tree.h"
#include <stdio.h>
#include <stdlib.h>

struct NodeImformationOfSearch {
	struct Node *node;
	int level;
	struct Node *parent;
};

/* Low Level Handing */

struct Node *_create_node(int data, struct Node *lchild, struct Node *rchild)
{
	struct Node *node = (struct Node *)malloc(sizeof(struct Node));
	if (node !=NULL)
	{
		node->data = data;
		node->lchild = lchild;
		node->rchild = rchild;
	}
	return node;
}

struct Node *_create_leaf_node(int data)
{
	_create_node(data, NULL, NULL);
}

void _free_node(struct Node *node)
{
	free(node);
}

struct Tree *_create_tree_node(struct Node *root)
{
	struct Tree *tree = (struct Tree *)malloc(sizeof(struct Tree));
	if (tree != NULL)
	{
		tree->root = root;
	}
	return tree;
}

void _free_tree_node(struct Node *node)
{
	if (node->lchild != NULL) _free_tree_node(node->lchild);
	if (node->rchild != NULL) _free_tree_node(node->rchild);
	_free_node(node);
}

int _traverse_tree_func(struct Node *node, int level, int (*func)(struct Node *, int))
{
	int sum = 0;
	if (node)
	{
		sum += _traverse_tree_func(node->lchild, level + 1, func);
		sum += func(node, level);
		sum += _traverse_tree_func(node->rchild, level + 1, func);
	}
	return sum;
}

void _insert_node(struct Node *root, struct Node *dest)
{
	struct Node *node = root;
	while (1)
	{
		if (dest->data > node->data)
		{
			if (node->rchild != NULL)
			{
				node = node->rchild;
			}
			else
			{
				node->rchild = dest;
				break;
			}
		}
		else
		{
			if (node->lchild != NULL)
			{
				node = node->lchild;
			}
			else
			{
				node->lchild = dest;
				break;
			}
		}
	}
}

void _delete_node(struct Node *node, struct Node *parent)
{
	if (node->lchild != NULL)
	{
		struct Node *n = node->lchild;
		struct Node *p = node;
		while (n->rchild != NULL)
		{
			p = n;
			n = n->rchild;
		}

		node->data = n->data;
		if (p == node) p->lchild = n->lchild;
		else p->rchild = n->lchild;
		free(n);
	}
	else if (node->rchild != NULL)
	{
		struct Node *n = node->rchild;
		struct Node *p = node;
		while (n->lchild != NULL)
		{
			p = n;
			n = n->lchild;
		}

		node->data = n->data;
		if (p == node) p->rchild = n->rchild;
		else p->lchild = n->rchild;
		free(n);
	}
	else
	{
		if (parent->lchild == node) parent->lchild = NULL;
		else parent->rchild = NULL;
		free(node);
	}
}

struct Node *_find_element(struct Node *root, int element, struct Node **parent)
{
	struct Node *rn;
	if (root != NULL && root->data != element)
	{
		rn = element < root->data ? _find_element(root->lchild, element, parent) : _find_element(root->rchild, element, parent);
		if (!*parent) *parent = root;
	}
	else
	{
		rn = root;
	}

	return rn;
}

/* Binary Tree Handing */

// Create Binary Sort Tree
struct Tree *creat_binary_sort_tree(int *arr, int len)
{
	int i;

	struct Node *root = _create_leaf_node(arr[0]);
	for (i = 1; i < len; i++)
	{
		_insert_node(root, _create_leaf_node(arr[i]));
	}

	return _create_tree_node(root);
}

void free_tree(struct Tree *tree)
{
	_free_tree_node(tree->root);
	free(tree);
}

// Traverse Tree
int _print_tree(struct Node *node, int level) { printf("%d ", node->data); return 0; }

void traverse_tree(struct Tree *tree)
{
	printf("%s: ", "Tree node");
	_traverse_tree_func(tree->root, 1, _print_tree);
	printf("\n");
}

// Average Length of Search
int _search_length(struct Node *node, int level) { return level; }
int _tree_size(struct Node *node, int level) { return 1; }

float average_length_of_search(struct Tree *tree)
{
	return (float)_traverse_tree_func(tree->root, 1, _search_length) / _traverse_tree_func(tree->root, 1, _tree_size);
}

// Delete Element
void del_element(struct Tree *tree, int element)
{
	struct Node *parent = NULL;
	struct Node *node = _find_element(tree->root, element, &parent);
	if (node)
	{
		_delete_node(node, parent);
		printf("The element(%d) delete done.\n", element);
	}
	else
	{
		printf("Not find %d, and cannot delete this element.\n", element);
	}
	traverse_tree(tree);
}

// Transfrom Tree
int _height(struct Node *node)
{
	if (node == NULL) return 0;
	int l = _height(node->lchild);
	int r = _height(node->rchild);
	return (l >= r ? l : r) + 1;
}

int _balance(struct Node *node)
{
	return _height(node->lchild) - _height(node->rchild);
}

struct Node *_rotate_left(struct Node *node)
{
	struct Node *nr = NULL;
	if (_balance(node->lchild) <= -1)
	{
		nr = node->lchild->rchild;
		node->lchild->rchild = nr->lchild;
		nr->lchild = node->lchild;
		node->lchild = nr;
	}
	else
	{
		nr = node->lchild;
	}
	node->lchild = nr->rchild;
	nr->rchild = node;
	return nr;
}

struct Node *_rotate_right(struct Node *node)
{
	struct Node *nr = NULL;
	if (_balance(node->rchild) >= 1)
	{
		nr = node->rchild->lchild;
		node->rchild->lchild = nr->rchild;
		nr->rchild = node->rchild;
		node->rchild = nr;
	}
	else
	{
		nr = node->rchild;
	}
	node->rchild = nr->lchild;
	nr->lchild = node;
	return nr;
}

struct Node *_transfrom_tree(struct Node *node)
{
	struct Node *nn = node;
	if (node->lchild != NULL) nn->lchild = _transfrom_tree(node->lchild);
	if (node->rchild != NULL) nn->rchild = _transfrom_tree(node->rchild);

	int balance = _balance(node);
	if (balance >= 2)
	{
		nn = _rotate_left(node);
	}
	else if (balance <= -2)
	{
		nn = _rotate_right(node);
	}

	return nn;
}

void transfrom_tree(struct Tree *tree)
{
	struct Node *root = _transfrom_tree(tree->root);
	if (root != tree->root)
	{
		tree->root = root;
	}
}

struct Tree *create_AVL_tree(int *arr, int len)
{
	int i;

	struct Node *root = _create_leaf_node(arr[0]);
	for (i = 1; i < len; i++)
	{
		_insert_node(root, _create_leaf_node(arr[i]));
		root = _transfrom_tree(root);
	}

	return _create_tree_node(root);
}