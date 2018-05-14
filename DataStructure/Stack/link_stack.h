#ifndef __LINK_STACK_H__
#define __LINK_STACK_H__

typedef int Element;
#define ILLGAL -1

struct LinkElem {
	Element data;
	struct LinkElem *next;
};

struct LinkStack {
	struct LinkElem *top;
};

// new
struct LinkStack *new_stack();
// free
void free_stack(struct LinkStack *stack);
// 插入
void push_elem(struct LinkStack *stack, Element elem);
// 删除
Element pop_elem(struct LinkStack *stack);

#endif