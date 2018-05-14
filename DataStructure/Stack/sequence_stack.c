#include "sequence_stack.h"
#include <stdio.h>
#include <stdlib.h>

#define STACK_INIT_SIZE 10

/* Low level handing */

static void _expand_stack_data(struct SequenceStack *stack, int add_size)
{
	stack->size += add_size;
	stack->data = (StackElement *)realloc(stack->data, stack->size);
}

/* Sequence stack handing */

struct SequenceStack *new_stack()
{
	struct SequenceStack *stack = (struct SequenceStack *)malloc(sizeof(struct SequenceStack));
	stack->data = (StackElement *)malloc(sizeof(StackElement) * STACK_INIT_SIZE);
	stack->top = 0;
	stack->size = STACK_INIT_SIZE;
	return stack;
}

void free_stack(struct SequenceStack *stack)
{
	free(stack->data);
	free(stack);
}

void push_elem(struct SequenceStack *stack, StackElement elem)
{
	if (stack->top > stack->size)
	{
		_expand_stack_data(stack, STACK_INIT_SIZE);
	}
	stack->data[stack->top++] = elem;
}

StackElement pop_elem(struct SequenceStack *stack)
{
	if (stack->top <= 0)
	{
		printf("Stack is empty, and can't pop up.\n");
		return ILLGAL;
	}
	return stack->data[--stack->top];
}

StackElement get_elem(struct SequenceStack *stack)
{
	if (stack->top <= 0)
	{
		printf("Stack is empty, and can't get element.\n");
		return ILLGAL;
	}
	return stack->data[(stack->top) - 1];
}

int is_empty_stack(struct SequenceStack *stack)
{
	return stack->top <= 0 ? 1 : 0;
}

int stacklen(struct SequenceStack *stack)
{
	return stack->top;
}

// Test
// int main()
// {
// 	struct SequenceStack *stack = new_stack();
// 	push_elem(stack, 1);
// 	push_elem(stack, 2);
// 	push_elem(stack, 3);
// 	StackElement e;
// 	while ((e = pop_elem(stack)) != ILLGAL)
// 	{
// 		printf("%d\n", e);
// 	}
// 	free_stack(stack);

// 	return 0;
// }