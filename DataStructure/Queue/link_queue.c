#include "link_queue.h"
#include <stdio.h>
#include <stdlib.h>

/* Low level handing */

struct LinkedElem *_alloc_elem(struct LinkedElem *next)
{
	struct LinkedElem *elem = (struct LinkedElem *)malloc(sizeof(struct LinkedElem));
	elem->next = next;
	return elem;
}

struct LinkedQueue *_alloc_queue()
{
	struct LinkedQueue *queue = (struct LinkedQueue *)malloc(sizeof(struct LinkedQueue));
	queue->front = _alloc_elem(NULL);
	queue->rear = queue->front;
	return queue;
}

/* Queue handing */

struct LinkedQueue *new_queue()
{
	return _alloc_queue();
}

void free_queue(struct LinkedQueue *queue)
{
	while (queue->front != queue->rear)
	{
		pop_elem(queue);
	}
	free(queue);
}

void push_queue_elem(struct LinkedQueue *queue, QueueElement elem)
{
	struct LinkedElem *element = _alloc_elem(NULL);
	element->data = elem;

	queue->rear->next = element;
	queue->rear = element;
}

QueueElement pop_queue_elem(struct LinkedQueue *queue)
{
	if (queue->front == queue->rear)
	{
		printf("Queue is empty, can't pop element.\n");
		return Error;
	}
	struct LinkedElem *element = queue->front->next;
	QueueElement e = element->data;

	queue->front->next = element->next;
	if (queue->rear == element)
	{
		queue->rear = queue->front;
	}
	free(element);

	return e;
}

QueueElement get_queue_elem(struct LinkedQueue *queue)
{
	if (queue->front == queue->rear)
	{
		printf("Queue is empty, can't pop element.\n");
		return Error;
	}
	struct LinkedElem *element = queue->front->next;
	return element->data;
}

int is_empty_queue(struct LinkedQueue *queue)
{
	return (queue->front == queue->rear) ? 1 : 0;
}

// Test
// int main()
// {
// 	struct LinkedQueue *queue = new_queue();
// 	push_queue_elem(queue, 1);
// 	QueueElement e;
// 	while ((e = pop_queue_elem(queue)) != Error)
// 	{
// 		printf("%d\n", e);
// 	}
// 	free_queue(queue);

// 	return 0;
// }