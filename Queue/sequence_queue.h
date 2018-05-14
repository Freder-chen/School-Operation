#ifndef __SEQUENCE_QUEUE_H__
#define __SEQUENCE_QUEUE_H__

typedef int Element;

#define QUEUE_INIT_SIZE 10

enum Status
{
	Error = -1
};

struct SequenceQueue {
	Element *data;
	int front; // 头
    int rear;  // 尾
};

// new
struct SequenceQueue *new_queue();
// free
void free_queue(struct SequenceQueue * queue);
// 插入
void push_elem(struct SequenceQueue *queue, Element elem);
// 删除
Element pop_elem(struct SequenceQueue *queue);

#endif
