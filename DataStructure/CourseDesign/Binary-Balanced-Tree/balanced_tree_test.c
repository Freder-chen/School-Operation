#include "balanced_tree.h"
#include <stdio.h>

int main()
{
	int arr[] = {1, 2, 3, 5, 4, 6, 7, 9, 8};
	// int arr[] = {5, 4, 6, 2, 1, 3, 7, 9, 8};

	struct Tree *tree = creat_binary_sort_tree(arr, sizeof(arr) / sizeof(int));
	traverse_tree(tree);
	printf("Tree's average length of search: %f\n\n", average_length_of_search(tree));

	del_element(tree, 3);
	printf("\n");

	transfrom_tree(tree);
	printf("After transfrom, tree root is %d\n", tree->root->data);
	traverse_tree(tree);
	printf("\n");

	struct Tree *t = create_AVL_tree(arr, sizeof(arr) / sizeof(int));
	traverse_tree(t);
	printf("AVL Tree's average length of search: %f\n\n", average_length_of_search(tree));

	free_tree(tree);
	free_tree(t);

	return 0;
}