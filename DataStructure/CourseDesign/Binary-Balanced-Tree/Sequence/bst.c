#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define InvalidValue (-100000000)

/* Low Level Handing */

Element *_init_nodes(int len)
{
	int i;

	Element *nodes = (Element *)malloc(sizeof(Element) * len);
	for (i = 0; i < len; i++)
	{
		nodes[i] = InvalidValue;
	}
	
	return nodes;
}

void _free_nodes(Element *nodes)
{
	free(nodes);
}

struct Tree *_create_tree(Element nodes[], int len)
{
	struct Tree *tree = (struct Tree *)malloc(sizeof(struct Tree));
	if (tree != NULL)
	{
		tree->nodes = nodes;
		tree->len = len;
	}
	return tree;
}

void _insert_node(Element nodes[], Element dest, int i)
{
	if (nodes[i] == InvalidValue)
	{
		nodes[i] = dest;
		return;
	}

	if (dest <= nodes[i])
	{
		_insert_node(nodes, dest, 2 * i + 1);
	}
	else
	{
		_insert_node(nodes, dest, 2 * i + 2);
	}
}

int _traverse_tree_func(Element *nodes, int len, int index, int level, int (*func)(Element *, int, int))
{
	int sum = 0;
	if (index < 1 << len - 1 && nodes[index] != InvalidValue)
	{
		sum += _traverse_tree_func(nodes, len, 2 * index + 1, level + 1, func);
		sum += func(nodes, index, level);
		sum += _traverse_tree_func(nodes, len, 2 * index + 2, level + 1, func);
	}
	return sum;
}

int _find_element(Element *nodes, int len, int index, Element element)
{
	if (index < 1 << len - 1 && nodes[index] != InvalidValue)
	{
		if (nodes[index] != element)
		{
			index = element < nodes[index] ? _find_element(nodes, len, 2 * index + 1, element)
			: _find_element(nodes, len, 2 * index + 2, element);
		}
	}
	else
	{
		index = -1;
	}

	return index;
}

void _delete_node(Element *nodes, int len, int index)
{
	if (index >= 1 << len - 1 || nodes[index] == InvalidValue) return;

	int i = index;
	if (2 * i + 1 < 1 << len - 1 && nodes[2 * i + 1] != InvalidValue)
	{
		// Left
		i = 2 * i + 1;
		while (2 * i + 2 < 1 << len - 1 && nodes[2 * i + 2] != InvalidValue)
		{
			i = 2 * i + 2;
		}

		nodes[index] = nodes[i];
		_delete_node(nodes, len, i);
	}
	else if (2 * i + 2 < 1 << len - 1 && nodes[2 * i + 2] != InvalidValue)
	{
		// Right
		i = 2 * i + 2;
		while (2 * i + 1 < 1 << len - 1 && nodes[2 * i + 1] != InvalidValue)
		{
			i = 2 * i + 1;
		}

		nodes[index] = nodes[i];
		_delete_node(nodes, len, i);
	}
	else
	{
		nodes[index] = InvalidValue;
	}
}

/* Binary Sort Tree Handing */

struct Tree *create_tree(Element elems[], int len)
{
	int i;

	Element *nodes = _init_nodes(1 << len - 1);
	for (i = 0; i < len; i++)
	{
		_insert_node(nodes, elems[i], 0);
	}

	return _create_tree(nodes, len);
}

void free_tree(struct Tree *tree)
{
	if (tree == NULL) return;
	_free_nodes(tree->nodes);
	free(tree);
}

// Traverse Tree
int _print_node(Element *nodes, int index, int level) { printf(" %d", nodes[index]); return 0; }

void traverse_tree(struct Tree *tree)
{
	printf("Tree node:");
	_traverse_tree_func(tree->nodes, tree->len, 0, 1, _print_node);
	printf("\n");
}

// Average Length of Search
int _search_length(Element *nodes, int index, int level) { return level; }

float average_length_of_search(struct Tree *tree)
{
	return (float)_traverse_tree_func(tree->nodes, tree->len, 0, 1, _search_length) / tree->len;
}

// Delete Element


void del_element(struct Tree *tree, Element element)
{
	int index = _find_element(tree->nodes, tree->len, 0, element);
	if (index != -1)
	{
		_delete_node(tree->nodes, tree->len, index);
		printf("The element(%d) delete done.\n", element);
	}
	else
	{
		printf("Not find %d, and cannot delete this element.\n", element);
	}
	traverse_tree(tree);
}