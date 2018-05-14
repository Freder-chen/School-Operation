#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "..\queue\link_queue.h"
#include "sequence_graph.h"

#ifndef INT_MAX
#define INT_MAX 2147483647
#endif

/* Low level handing */
void _print_list(int *list, int len, char *tips)
{
	int i;
	printf("%s:", tips);
	for (i = 0; i < len; i++)
	{
		printf(" %d", list[i]);
	}
	printf("\n");
}

void _print_vertex_list(GraphVertex *vertex_list, int len)
{
	_print_list((int *)vertex_list, len, "The vertex list")
}

struct SequenceGraph *_create_graph1()
{
	GraphVertex vertex_list[] = {1, 2, 3, 4, 5};
	int scope = sizeof(vertex_list) / sizeof(int);

	int edge_list[] = {
		0, 1, 1,
		0, 3, 1,
		1, 2, 1,
		1, 3, 1,
		2, 4, 1,
		3, 2, 1,
		3, 4, 1
	};
	int path_amount = sizeof(edge_list) / sizeof(int) / 3;

	return new_graph_base(scope, path_amount, vertex_list, edge_list);
}

struct SequenceGraph *_create_graph2()
{
	GraphVertex vertex_list[] = {1, 2, 3, 4, 5};
	int scope = sizeof(vertex_list) / sizeof(int);

	int edge_list[] = {
		0, 1, 5,
		0, 2, 10,
		1, 2, 3,
		1, 3, 2,
		1, 4, 9,
		2, 1, 2,
		2, 4, 1,
		3, 0, 7,
		3, 4, 6,
		4, 3, 4
	};
	int path_amount = sizeof(edge_list) / sizeof(int) / 3;

	return new_graph_base(scope, path_amount, vertex_list, edge_list);
}

/* Graph path handing */

GraphVertex *topological_sorting(struct SequenceGraph *graph)
{
	GraphVertex *vertex_list = (GraphVertex *)malloc(sizeof(GraphVertex) * graph->scope);
	int vertex_index = 0;

	// Initialize indegree
	int *indegree = (int *)malloc(sizeof(int) * graph->scope);
	memset(indegree, 0, sizeof(int) * graph->scope);
	int i, j;
	for (i = 0; i < graph->scope; i++)
	{
		for (j = 0; j < graph->scope; j++)
		{
			if (graph->edge[j][i])
			{
				indegree[i]++;
			}
		}
	}

	// Find vertex
	struct LinkedQueue *queue = new_queue();
	for (i = 0; i < graph->scope; i++)
	{
		for (j = 0; j < graph->scope; j++)
		{
			if (indegree[j] == 0)
			{
				push_queue_elem(queue, j);
				indegree[j] = -1;
			}
		}

		// Deal with indegree
		while (!is_empty_queue(queue))
		{
			int vi = pop_queue_elem(queue);
			for (j = 0; j < graph->scope; j++)
			{
				if (graph->edge[vi][j])
				{
					indegree[j]--;
				}
			}

			vertex_list[vertex_index++] = graph->vertex[vi];
		}

	}

	free(indegree);
	free_queue(queue);
	return vertex_list;
}

void dijkstra(struct SequenceGraph *graph, int index)
{
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

	while (!is_empty_queue(queue))
	{
		int vi = pop_queue_elem(queue);

		for (i = 0; i < graph->scope; i++)
		{
			if (graph->edge[vi][i])
			{
				if (distance[i] - graph->edge[vi][i] > distance[vi])
				{
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

int main()
{
	struct SequenceGraph *graph1 = _create_graph1();
	print_graph(graph1);
	GraphVertex *vertex_list = topological_sorting(graph1);
	_print_vertex_list(vertex_list, graph1->scope);
	free_graph(graph1);
	free(vertex_list);

	struct SequenceGraph *graph2 = _create_graph2();
	dijkstra(graph2, 0);
	free_graph(graph2);

	return 0;
}