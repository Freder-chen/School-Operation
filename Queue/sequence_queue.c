#include "sequence_queue.h"
#include <stdio.h>

// struct SequenceQueue {
// 	Element *data;
// 	int front; // 头
//     int rear;  // 尾
// };

/* Low level handing */
struct SequenceQueue *_alloc_queue()
{
	struct SequenceQueue *queue = (struct SequenceQueue *)malloc(sizeof(struct SequenceQueue));
	queue->data = (Element *)malloc(sizeof(Element) * QUEUE_INIT_SIZE);
	queue->front = 0;
	queue->rear = 0;
	return queue;
}

/* Queue handing */

struct SequenceQueue *new_queue()
{
	return _alloc_queue();
}

void free_queue(struct SequenceQueue *queue)
{
	free(queue->data);
	free(queue);
}

void push_elem(struct SequenceQueue *queue, Element elem)
{
	if ((queue->rear + 1) % QUEUE_INIT_SIZE == queue->front)
    {
    	printf("Queue is full, can't push element.\n");
    	return;
    }
	queue->data[queue->rear] = elem;
	queue->rear = (queue->rear + 1) % QUEUE_INIT_SIZE;
}

Element pop_elem(struct SequenceQueue *queue)
{
	if (queue->front == queue->rear)
	{
		printf("Queue is empty, can't pop element.\n");
		return Error;
	}
	int f = queue->front;
	queue->front = (queue->front + 1) % QUEUE_INIT_SIZE;
	return queue->data[f];
}

int main()
{
	struct SequenceQueue *queue = new_queue();
	push_elem(queue, 1);
	Element e;
	while ((e = pop_elem(queue)) != Error)
	{
		printf("%d\n", e);
	}
	free_queue(queue);
	return 0;
}