#ifndef __SEQUENCE_STACK_H__
#define __SEQUENCE_STACK_H__

#define ILLGAL 0

// typedef struct LinkedTreeNode* StackElement;
typedef int StackElement;

struct SequenceStack
{
	StackElement *data;
	int top;
	int size;
};

// new
struct SequenceStack *new_stack();
// free
void free_stack(struct SequenceStack *stack);
// 插入
void push_elem(struct SequenceStack *stack, StackElement elem);
// 删除
StackElement pop_elem(struct SequenceStack *stack);

StackElement get_elem(struct SequenceStack *stack);

int is_empty_stack(struct SequenceStack *stack);
int stacklen(struct SequenceStack *stack);

#endif
