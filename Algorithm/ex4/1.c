#include <stdio.h>
#include <stdlib.h>

#ifndef INT_MAX
#define INT_MAX 100
#endif

struct Edge {
	int src;
	int dest;
	int weight;
};

struct Graph {
	int *vertex;
	int **edge;
	int scope;
	int path_amount;
};

struct LinkedElem {
	int data;
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
void push_queue_elem(struct LinkedQueue *queue, int elem);
// 删除
int pop_queue_elem(struct LinkedQueue *queue);
int is_empty_queue(struct LinkedQueue *queue);
/* queue func */
struct LinkedElem *_alloc_elem(struct LinkedElem *next) {
	struct LinkedElem *elem = (struct LinkedElem *)malloc(sizeof(struct LinkedElem));
	elem->next = next;
	return elem;
}

struct LinkedQueue *new_queue() {
	struct LinkedQueue *queue = (struct LinkedQueue *)malloc(sizeof(struct LinkedQueue));
	queue->front = _alloc_elem(NULL);
	queue->rear = queue->front;
	return queue;
}

void free_queue(struct LinkedQueue *queue) {
	while (queue->front != queue->rear) {
		pop_queue_elem(queue);
	}
	free(queue);
}

void push_queue_elem(struct LinkedQueue *queue, int elem) {
	struct LinkedElem *element = _alloc_elem(NULL);
	element->data = elem;

	queue->rear->next = element;
	queue->rear = element;
}

int pop_queue_elem(struct LinkedQueue *queue) {
	if (queue->front == queue->rear) {
		printf("Queue is empty, can't pop element.\n");
		return -1;
	}
	struct LinkedElem *element = queue->front->next;
	int e = element->data;

	queue->front->next = element->next;
	if (queue->rear == element) {
		queue->rear = queue->front;
	}
	free(element);

	return e;
}

int is_empty_queue(struct LinkedQueue *queue) {
	return (queue->front == queue->rear) ? 1 : 0;
}

/* base func: new and free graph, queue */
struct Graph *new_graph_base(int scope, int path_amount, int *vertex_list, int *edge_list) {
	int i, j;

	struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
	graph->scope = scope;
	graph->path_amount = path_amount;
	graph->vertex = (int *)malloc(sizeof(int) * graph->scope);;
	graph->edge = (int **)malloc(sizeof(int *) * graph->scope);
	
	// vertex
	for (i = 0; i < graph->scope; i++) {
		graph->vertex[i] = vertex_list[i];
	}
	// edge
	for (i = 0; i < graph->scope; i++) {
		graph->edge[i] = (int *)malloc(sizeof(int) * graph->scope);
		for (j = 0; j < graph->scope; j++) {
			graph->edge[i][j] = 0;
		}
	}
	for (i = 0; i < graph->path_amount * 3; i += 3) {
		graph->edge[edge_list[i]][edge_list[i + 1]] = edge_list[i + 2];
	}

	return graph;
}

void free_graph(struct Graph *graph) {
	if (graph) {
		if (graph->vertex) free(graph->vertex);
		if (graph->edge) {
			int i;
			for (i = 0; i < graph->scope; i++) {
				free(graph->edge[i]);
			}
			free(graph->edge);
		}
		free(graph);
	}
}

/* Low level handing */
void _print_list(int *list, int len, const char *tips) {
	int i;
	printf("%s:", tips);
	for (i = 0; i < len; i++) {
		printf(" %d", list[i]);
	}
	printf("\n");
}

/* Dijkstra */
void dijkstra(struct Graph *graph, int index) {
	int i;

	int *distance = (int *)malloc(sizeof(int) * graph->scope);
	int *path = (int *)malloc(sizeof(int) * graph->scope);
	struct LinkedQueue *queue = new_queue();

	// Init
	for (i = 0; i < graph->scope; i++) { distance[i] = INT_MAX; }
	for (i = 0; i < graph->scope; i++) { path[i] = i; }
	path[index] = 0;
	distance[index] = 0;
	push_queue_elem(queue, index);

	while (!is_empty_queue(queue)) {
		int vi = pop_queue_elem(queue);

		for (i = 0; i < graph->scope; i++) {
			if (graph->edge[vi][i]) {
				if (distance[i] - graph->edge[vi][i] > distance[vi]) {
					path[i] = vi;
					distance[i] = distance[vi] + graph->edge[vi][i];
					push_queue_elem(queue, i);
				}
			}
		}
		
		_print_list(distance, graph->scope, "distance");
		_print_list(path, graph->scope, "path");
	}

	free(distance);
	free(path);
	free_queue(queue);
}

/* Main */
struct Graph *_create_graph1() {
	int vertex_list[] = {1, 2, 3, 4, 5};
	int scope = sizeof(vertex_list) / sizeof(int);

	int edge_list[] = {
		0,3,7,
		1,0,3,
		1,2,4,
		2,4,6,
		3,1,2,
		3,2,5,
		4,3,4
	};
	int path_amount = sizeof(edge_list) / sizeof(int) / 3;

	return new_graph_base(scope, path_amount, vertex_list, edge_list);
}

struct Graph *_create_graph2() {
	int vertex_list[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
	int scope = sizeof(vertex_list) / sizeof(int);

	int edge_list[] = {
		0,1,3, 1,0,3,
		0,2,5, 2,0,5,
		0,3,4, 3,0,4,
		1,4,3, 4,1,3,
		1,5,6, 5,1,6,
		2,3,2, 3,2,3,
		2,6,4, 6,2,4,
		3,4,1, 4,3,1,
		3,7,5, 7,3,5,
		4,5,2, 5,4,2,
		4,8,4, 8,4,4,
		5,9,5, 9,5,5,
		6,7,3, 7,6,3,
		6,10,6, 10,6,6,
		7,8,6, 8,7,6,
		7,10,7, 10,7,7,
		8,9,3, 9,8,3,
		8,11,5, 11,8,5,
		9,11,9, 11,9,9,
		10,11,8, 11,10,8
	};
	int path_amount = sizeof(edge_list) / sizeof(int) / 3;

	return new_graph_base(scope, path_amount, vertex_list, edge_list);
}

int main() {
	struct Graph *graph = _create_graph2();
	dijkstra(graph, 0);
	free_graph(graph);

	return 0;
}