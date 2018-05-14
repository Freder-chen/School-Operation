#include "linked_tree.h"
#include "..\stack\sequence_stack.h"
#include <stdio.h>
#include <stdlib.h>

struct LinkedTreeNode *_alloc_elem(Element data)
{
	struct LinkedTreeNode *node = (struct LinkedTreeNode *)malloc(sizeof(struct LinkedTreeNode));
	node->data = data;
	node->lchild = NULL;
	node->rchild = NULL;
	return node;
}

/* 递归创建树 */
struct LinkedTreeNode *_recursive_new_tree(Element *elist, int *i, Element end_tag)
{
	struct LinkedTreeNode *node = NULL;
	// printf("In node %d.\n", *i);
	if (elist[*i] == end_tag) (*i)++;
	else
	{
		// printf("Alloced list[%d]:%d.\n", *i, elist[*i]);
		node = _alloc_elem(elist[(*i)++]);
		node->lchild = _recursive_new_tree(elist, i, end_tag);
		node->rchild = _recursive_new_tree(elist, i, end_tag);
	}
	return node;
}
struct LinkedTreeNode *recursive_new_tree(Element *elist, Element end_tag)
{
	int i = 0;
	return _recursive_new_tree(elist, &i, end_tag);
}

/* 非递归创建树 */
struct LinkedTreeNode *non_recursive_new_tree(Element *elist, Element end_tag)
{
	int i = 0;
	struct LinkedTreeNode *root = NULL, *node = NULL;
	struct SequenceStack *stack = new_stack();

	if (elist[i++] == end_tag)
	{
		return NULL;
	}
	else
	{
		root = node = _alloc_elem(elist[i - 1]);
		push_elem(stack, node);
		// printf("root:Alloced list[%d]:%d.\n", i - 1, elist[i - 1]);
	}
	do {
		while (elist[i++] != end_tag)
		{
			node->lchild = _alloc_elem(elist[i - 1]);
			node = node->lchild;
			push_elem(stack, node);
			// printf("left:Alloced list[%d]:%d.\n", i - 1, elist[i - 1]);
		}
		while (!is_empty_stack(stack))
		{
			node = pop_elem(stack);
			// printf("pop %d.\n", node->data);
			if (elist[i++] != end_tag)
			{
				node->rchild = _alloc_elem(elist[i - 1]);
				node = node->rchild;
				push_elem(stack, node);
				// printf("right:Alloced list[%d]:%d.\n", i - 1, elist[i - 1]);
				break;
			}
		}
	} while (!is_empty_stack(stack));
	free_stack(stack);
	return root;
}

/* 递归遍历 */
void recursive_traverse_tree(struct LinkedTreeNode *node)
{
	if (node)
	{
		printf("%d\n", node->data);
		recursive_traverse_tree(node->lchild);
		recursive_traverse_tree(node->rchild);
	}
}

/* 非递归遍历 */
void non_recursive_traverse_tree(struct LinkedTreeNode *node)
{
	/* 中序遍历 */
	struct SequenceStack *stack = new_stack();
	if (node == NULL)return;	
	do {
		while (node != NULL)
		{
			push_elem(stack, node);
			// printf("in --- %d ", node->data);
			node = node->lchild;
			// printf("--- to left.\n");
		}
		node = pop_elem(stack);
		printf("%d\n", node->data);
		// printf("out--- %d ", node->data);
		node = node->rchild;
		// printf("--- to right.\n");
	} while (!is_empty_stack(stack) || node != NULL);
	free_stack(stack);
}

// Test
// int main()
// {

// 	Element elist[] = { 1, 2, 0, 0, 3, 4, 0, 0, 0 };
// 	struct LinkedTreeNode *root_node = recursive_new_tree(elist, 0);
// 	recursive_traverse_tree(root_node);

// 	printf("\n------------------------------\n");

// 	Element elist2[] = { 1, 2, 0, 0, 3, 4, 0, 0, 0, 0 };
// 	struct LinkedTreeNode *root_node2 = non_recursive_new_tree(elist2, 0); 
// 	non_recursive_traverse_tree(root_node2);
	
// 	system("pause");

// 	return 0;
// }