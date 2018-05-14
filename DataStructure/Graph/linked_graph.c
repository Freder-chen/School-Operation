#include "linked_graph.h"
#include "..\stack\sequence_stack.h"
#include "..\queue\link_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Low level handing */

void _free_vertex_edge_node(struct EdgeNode *node)
{
	while (node != NULL)
	{
		struct EdgeNode *fnode = node;
		node = node->next_edge;
		free(fnode);
	}
}

void _print_vertex_edge_node(struct EdgeNode *node)
{
	while (node != NULL)
	{
		struct EdgeNode *edgenode = node;
		node = node->next_edge;
		printf(" %d", edgenode->vex_index);
	}
	printf("\n");
}

void _print_vertex(GraphVertex vertex, char *format)
{
	printf("%d%s", vertex, format);
}

/* Linked graph handing */

struct LinkedGraph *new_graph()
{
	int i;
	struct LinkedGraph *graph = (struct LinkedGraph *)malloc(sizeof(struct LinkedGraph));

	printf("Input the number of graph scope and path amount: ");
	scanf("%d%d", &graph->scope, &graph->path_amount);
	graph->vertex_list = (struct VertexNode *)malloc(sizeof(struct VertexNode) * graph->scope);
	
	printf("Input the imformation of vertex:\n");
	for (i = 0; i < graph->scope; i++)
	{
		scanf("%d", &graph->vertex_list[i].value);
		graph->vertex_list[i].next_edge = NULL;
	}

	printf("Input the information of edge:\n");
	for (i = 0; i < graph->path_amount; i++)
	{
		int r, c;
		scanf("%d%d", &r, &c);

		struct EdgeNode *edge = (struct EdgeNode *)malloc(sizeof(struct EdgeNode));
		edge->next_edge = graph->vertex_list[r].next_edge;
		edge->vex_index = c;
		edge->weight = 1;
		graph->vertex_list[r].next_edge = edge;
	}
	return graph;
}

void free_graph(struct LinkedGraph *graph)
{
	if (graph != NULL)
	{
		if (graph->vertex_list != NULL)
		{
			int i;
			for (i = 0; i < graph->scope; i++)
			{
				if (graph->vertex_list[i].next_edge != NULL)
				{
					_free_vertex_edge_node(graph->vertex_list[i].next_edge);
				}
			}
			free(graph->vertex_list);
		}
		free(graph);
	}
}

void print_graph(struct LinkedGraph *graph)
{
	int i;
	struct VertexNode *vertexnode = graph->vertex_list;

	for (i = 0; i < graph->scope; i++)
	{
		printf("%d:", vertexnode[i].value);
		_print_vertex_edge_node(vertexnode[i].next_edge);
	}
	printf("\n");
}

/* deep-first search recursive module */
void _dfs_traverse_graph(struct LinkedGraph *graph, int index, int *status)
{
	struct EdgeNode *edge;

	status[index] = 1;
	_print_vertex(graph->vertex_list[index].value, "\t");

	for (edge = graph->vertex_list[index].next_edge; edge; edge = edge->next_edge)
	{
		if (status[edge->vex_index] == 0)
		{
			_dfs_traverse_graph(graph, edge->vex_index, status);
		}
	}
}
void dfs_traverse_graph(struct LinkedGraph *graph)
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
}

/* deep-first search non recersive module */
void _dfs_traverse_graph2(struct LinkedGraph *graph, int index, int *status)
{
	struct EdgeNode *edge;
	struct SequenceStack *stack = new_stack();

	status[index] = 1;
	push_elem(stack, index);
	_print_vertex(graph->vertex_list[index].value, "\t");

	while(!is_empty_stack(stack))
	{
		index = get_elem(stack);
		for (edge = graph->vertex_list[index].next_edge; edge; edge = edge->next_edge)
		{
			if (status[edge->vex_index] == 0)
			{
				status[edge->vex_index] = 1;
				push_elem(stack, edge->vex_index);
				_print_vertex(graph->vertex_list[edge->vex_index].value, "\t");
				break;
			}
		}
		if (!edge)
		{
			pop_elem(stack);
		}
	}
	free_stack(stack);
}
void dfs_traverse_graph2(struct LinkedGraph *graph)
{
	int i;
	int *status = (int *)malloc(sizeof(int) * graph->scope);

	memset(status, 0, sizeof(int) * graph->scope);

	printf("The graph vertex include: ");
	for (i = 0; i < graph->scope; i++)
	{
		if (status[i] == 0)
		{
			_dfs_traverse_graph2(graph, i, status);
		}
	}
	printf("\n");
}

/* breadth-first search module */
void _bfs_traverse_graph(struct LinkedGraph *graph, int index, int *status)
{
	struct EdgeNode *edge;
	struct LinkedQueue *queue = new_queue();

	status[index] = 1;
	_print_vertex(graph->vertex_list[index].value, "\t");
	push_queue_elem(queue, index);

	while (!is_empty_queue(queue))
	{
		index = get_queue_elem(queue);
		for (edge = graph->vertex_list[index].next_edge; edge; edge = edge->next_edge)
		{
			if (status[edge->vex_index] == 0)
			{
				status[edge->vex_index] = 1;
				push_queue_elem(queue, edge->vex_index);
				_print_vertex(graph->vertex_list[edge->vex_index].value, "\t");
			}
		}
		pop_queue_elem(queue);
	}
	free_queue(queue);
}
void bfs_traverse_graph(struct LinkedGraph *graph)
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
}

// Test
// int main()
// {
// 	struct LinkedGraph *graph = new_graph();

// 	print_graph(graph);
// 	dfs_traverse_graph(graph);
// 	dfs_traverse_graph2(graph);
// 	bfs_traverse_graph(graph);

// 	free_graph(graph);
// 	system("pause");
// 	return 0;
// }