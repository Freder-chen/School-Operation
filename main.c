#include "tree\linked_tree.h"
#include "stack\sequence_stack.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	struct SequenceStack *stack = new_stack();
	free_stack(stack);
	return 0;
}