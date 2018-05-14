#include "link_stack.h"
#include <stdio.h>
#include <stdlib.h>

// struct LinkStack {
// 	struct LinkElem *top;
// };

// struct LinkElem {
// 	Element data;
// 	struct LinkElem *next;
// };

/* Low level Handing */

struct LinkElem *_alloc_elem(struct LinkElem *next)
{
	struct LinkElem *elem = (struct LinkElem *)malloc(sizeof(struct LinkElem));
	elem->next = next;
	return elem;
}

struct LinkStack *_alloc_stack()
{
	struct LinkStack *stack = (struct LinkStack *)malloc(sizeof(struct LinkStack));
	stack->top = NULL;
	return stack;
}

/* Link stack handing */

void push_elem(struct LinkStack *stack, Element elem)
{
	struct LinkElem *e = _alloc_elem(stack->top);
	e->data = elem;
	stack->top = e;
}

Element pop_elem(struct LinkStack *stack)
{
	if (stack->top == NULL)
	{
		printf("Stack is empty, and can't pop up.\n");
		return ILLGAL;
	}
	struct LinkElem *elem = stack->top;
	int element = elem->data;
	stack->top = elem->next;
	free(elem);
	return element;
}

struct LinkStack *new_stack()
{
	return _alloc_stack();
}


void free_stack(struct LinkStack *stack)
{
	while (stack->top != NULL)
	{
		pop_elem(stack);
	}
	free(stack);
}

// Test
// int main()
// {
// 	struct LinkStack *stack = new_stack();
// 	push_elem(stack, 1);
// 	Element e;
// 	while ((e = pop_elem(stack)) != ILLGAL)
// 	{
// 		printf("%d\n", e);
// 	}
// 	free_stack(stack);

// 	return 0;
// }