#include "sequence_graph.h"
#include "..\stack\sequence_stack.h"
#include "..\queue\link_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Low level handing */

void _print_vertex(GraphVertex vertex, char *format)
{
	printf("%d%s", vertex, format);
}

/* Sequence graph handing */

struct SequenceGraph *new_graph_base(int scope, int path_amount, GraphVertex *vertex_list, int *edge_list)
{
	int i, j;

	struct SequenceGraph *graph = (struct SequenceGraph *)malloc(sizeof(struct SequenceGraph));
	graph->scope = scope;
	graph->path_amount = path_amount;
	graph->vertex = (GraphVertex *)malloc(sizeof(GraphVertex) * graph->scope);
	graph->edge   = (int **)malloc(sizeof(int *) * graph->scope);

	for (i = 0; i < graph->scope; i++)
	{
		graph->edge[i] = (int *)malloc(sizeof(int) * graph->scope);
		for (j = 0; j < graph->scope; j++)
		{
			graph->edge[i][j] = 0;
		}
	}

	for (i = 0; i < graph->scope; i++)
	{
		graph->vertex[i] = vertex_list[i];
	}

	for (i = 0; i < graph->path_amount * 3; i += 3)
	{
		graph->edge[edge_list[i]][edge_list[i + 1]] = edge_list[i + 2];
	}

	return graph;
}

struct SequenceGraph *new_graph()
{
	int scope, path_amount;
	printf("Input the number of graph scope and path amount: ");
	scanf("%d%d", &scope, &path_amount);

	int i;
	GraphVertex *vertex_list = (GraphVertex *)malloc(sizeof(GraphVertex) * scope);
	printf("Input the imformation of vertex:\n");
	for (i = 0; i < scope; i++)
	{
		scanf("%d", &vertex_list[i]);
	}

	int *edge_list = (int *)malloc(sizeof(int) * 3 * path_amount);
	printf("Input the information of edge:\n");
	for (i = 0; i < path_amount; i++)
	{
		scanf("%d%d", &edge_list[i], &edge_list[i + 1]);
		edge_list[i + 2] = 1;
	}

	struct SequenceGraph *graph = new_graph_base(scope, path_amount, vertex_list, edge_list);
	free(vertex_list);
	free(edge_list);
	return graph;
}

void free_graph(struct SequenceGraph *graph)
{
	if (graph != NULL)
	{
		if (graph->vertex != NULL) free(graph->vertex);
		if (graph->edge != NULL)
		{
			int i;
			for (i = 0; i < graph->scope; i++)
			{
				free(graph->edge[i]);
			}
			free(graph->edge);
		}
		free(graph);
	}
}

void print_grapg_vertex(struct SequenceGraph *graph)
{
	int i;
	printf("The graph vertex:\n");
	for (i = 0; i < graph->scope; i++)
	{
		_print_vertex(graph->vertex[i], "\t");
	}
	printf("\n");
}

void print_graph_edge(struct SequenceGraph *graph)
{
	int i, j;
	printf("The graph edge:\n");
	for (i = 0; i < graph->scope; i++)
	{
		for (j = 0; j < graph->scope; j++)
		{
			printf("%d\t", graph->edge[i][j]);
		}
		printf("\n");
	}
}

void print_graph(struct SequenceGraph *graph)
{
	if (graph == NULL)
	{
		printf("The graph is NULL, cannot print.\n");
		return;
	}

	print_grapg_vertex(graph);
	print_graph_edge(graph);
}

/* recursive deep-first search module */
void _dfs_traverse_graph(struct SequenceGraph *graph, int index, int *status)
{
	status[index] = 1;
	_print_vertex(graph->vertex[index], "\t");

	int i;
	for (i = 0; i < graph->scope; i++)
	{
		if (graph->edge[index][i] == 1 && status[i] == 0)
		{
			_dfs_traverse_graph(graph, i, status);
		}
	}
}
void dfs_traverse_graph(struct SequenceGraph *graph)
{
	int i;
	int *status = (int *)malloc(sizeof(int) * graph->scope);
	memset(status, 0, sizeof(int) * graph->scope);
	printf("The graph vertex include: ");
	for (i = 0; i < graph->scope; i++)
	{
		if (status[i] == 0)
		{
			_dfs_traverse_graph(graph, i, status);
		}
	}
	printf("\n");
	free(status);
}

/* non recersive deep-first search module */
void _dfs_traverse_graph2(struct SequenceGraph *graph, int index, int *status)
{
	struct SequenceStack *stack = new_stack();
	status[index] = 0;
	push_elem(stack, index);
	_print_vertex(graph->vertex[index], "\t");
	while(!is_empty_stack(stack))
	{
		index = get_elem(stack);

		int i;
		for (i = status[index]; i < graph->scope; i++, status[index]++)
		{
			if (graph->edge[index][i] == 1 && status[i] == -1)
			{
				status[i] = 0;
				push_elem(stack, i);
				_print_vertex(graph->vertex[i], "\t");
				break;
			}		
		}
		if (i >= graph->scope)
		{
			pop_elem(stack);
		}
	}
	free_stack(stack);
}
void dfs_traverse_graph2(struct SequenceGraph *graph)
{
	int i;
	int *status = (int *)malloc(sizeof(int) * graph->scope);
	memset(status, -1, sizeof(int) * graph->scope);
	printf("The graph vertex include: ");
	for (i = 0; i < graph->scope; i++)
	{
		if (status[i] == -1)
		{
			_dfs_traverse_graph2(graph, i, status);
		}
	}
	printf("\n");
	free(status);
}

/* breadth-first search module */
void _bfs_traverse_graph(struct SequenceGraph *graph, int index, int *status)
{
	int i, j;
	struct LinkedQueue *queue = new_queue();
	status[index] = 1;
	_print_vertex(graph->vertex[index], "\t");
	push_queue_elem(queue, index);
	while (!is_empty_queue(queue))
	{
		index = get_queue_elem(queue);
		for (i = 0; i < graph->scope; i++)
		{
			if (graph->edge[index][i] == 1 && status[i] == 0)
			{
				status[i] = 1;
				_print_vertex(graph->vertex[i], "\t");
				push_queue_elem(queue, i);
			}
		}
		pop_queue_elem(queue);
	}
	free_queue(queue);
}
void bfs_traverse_graph(struct SequenceGraph *graph)
{
	int i;
	int *status = (int *)malloc(sizeof(int) * graph->scope);
	memset(status, 0, sizeof(int) * graph->scope);
	printf("The graph vertex include(bfs): ");
	for (i = 0; i < graph->scope; i++)
	{
		if (status[i] == 0)
		{
			_bfs_traverse_graph(graph, i, status);
		}
	}
	printf("\n");
	free(status);
}



// Test
// int main()
// {
// 	struct SequenceGraph *graph = new_graph();
// 	print_graph(graph);
// 	dfs_traverse_graph(graph);
// 	dfs_traverse_graph2(graph);
// 	bfs_traverse_graph(graph);
// 	free(graph);

// 	system("pause");
// 	return 0;
// }