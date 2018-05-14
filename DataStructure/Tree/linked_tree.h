#ifndef __LINKED_TREE_H__ 
#define __LINKED_TREE_H__ 

typedef int Element;

struct LinkedTreeNode
{
	Element data;
	struct LinkedTreeNode *lchild;
	struct LinkedTreeNode *rchild;
};

// 递归创建树
struct LinkedTreeNode *recursive_new_tree(Element *elist, Element end_tag);
// 非递归创建树
struct LinkedTreeNode *non_recursive_new_tree(Element *elist, Element end_tag);

// free
// void free_tree(struct LinkedTreeNode *root_node);

// 遍历
void recursive_traverse_tree(struct LinkedTreeNode *node);
void non_recursive_traverse_tree(struct LinkedTreeNode *node);

#endif