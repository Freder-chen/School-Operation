#ifndef __LINK_QUEUE_H__
#define __LINK_QUEUE_H__

typedef int QueueElement;

enum Status
{
	Succeed = 0,
	True = 1,
	False = -1,
	Error = -2,
};

struct LinkedElem {
	QueueElement data;
	struct LinkedElem *next;
};

struct LinkedQueue {
	struct LinkedElem *front; /* 头 */
	struct LinkedElem *rear; /* 尾 */
};

// new
struct LinkedQueue *new_queue();
// free
void free_queue(struct LinkedQueue *queue);
// 插入
void push_queue_elem(struct LinkedQueue *queue, QueueElement elem);
// 删除
QueueElement pop_queue_elem(struct LinkedQueue *queue);

QueueElement get_queue_elem(struct LinkedQueue *queue);

int is_empty_queue(struct LinkedQueue *queue);

#endif