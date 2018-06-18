#include "bst.h"
#include <stdio.h>

int main()
{
	Element arr[] = {1, 2, 3, 5, 4, 6, 7, 9, 8};
	// Element arr[] = {2, 1, 3};

	struct Tree *tree = create_tree(arr, sizeof(arr) / sizeof(Element));
	traverse_tree(tree);
	printf("Tree's average length of search: %f\n\n", average_length_of_search(tree));

	del_element(tree, 1);

	free_tree(tree);
	return 0;
}