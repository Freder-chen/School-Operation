#include "graph.h"
#include "../../Queue/link_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Edge 
{
	int src;
	int dest;
	int weight;
};

/* Low Level Handing */

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

int _get_vertex_outdegree(struct Graph *graph, int index)
{
	int out = 0;
	struct EdgeNode *edge = graph->vertex_list[index].next_edge;
	while (edge)
	{
		edge = edge->next_edge;
		out++;
	}
	return out;
}

int _get_vertex_indegree(struct Graph *graph, int index)
{
	int i;

	int in = 0;
	for (i = 0; i < graph->scope; i++)
	{
		if (i == index) continue;

		struct EdgeNode *edge = graph->vertex_list[i].next_edge;
		while (edge)
		{
			if (edge->vex_index == index) in++;
			edge = edge->next_edge;
		}
	}
	return in;
}

void _print_vertex(GraphVertex vertex, char *format)
{
	printf("%c%s", vertex, format);
}

int **_get_edge_matrix(struct Graph *graph)
{
	int i, j;

	int ** edge_matrix = (int **)malloc(sizeof(int *) * graph->scope);
	for (i = 0; i < graph->scope; i++)
	{
		edge_matrix[i] = (int *)malloc(sizeof(int) * graph->scope);
		for (j = 0; j < graph->scope; j++)
		{
			edge_matrix[i][j] = 0;
		}

		struct EdgeNode *edge = graph->vertex_list[i].next_edge;
		while (edge)
		{
			edge_matrix[i][edge->vex_index] = edge->weight;
			edge = edge->next_edge;
		}
	}

	return edge_matrix;
}

void _free_edge_matrix(int **edge_matrix, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		free(edge_matrix[i]);
	}
	free(edge_matrix);
}

GraphVertex *_get_vertex_list(struct Graph *graph)
{
	int i;

	GraphVertex *vertex_list = (GraphVertex *)malloc(sizeof(GraphVertex) * graph->scope);
	for (i = 0; i < graph->scope; i++)
	{
		vertex_list[i] = graph->vertex_list[i].value;
	}

	return vertex_list;
}

void _free_vertex_list(GraphVertex *vertex_list)
{
	free(vertex_list);
}

/* Graph Handing */

struct Graph *new_graph()
{
	int i;
	struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));

	printf("Input the number of graph scope and path amount: ");
	scanf("%d%d", &graph->scope, &graph->path_amount);
	graph->vertex_list = (struct VertexNode *)malloc(sizeof(struct VertexNode) * graph->scope);
	
	printf("Input the imformation of vertex:\n");
	for (i = 0; i < graph->scope; i++)
	{
		scanf("%*[ \n]%c", &(graph->vertex_list[i].value));
		graph->vertex_list[i].next_edge = NULL;
	}

	printf("Input the information of edge:\n");
	for (i = 0; i < graph->path_amount; i++)
	{
		int r, c, w;
		scanf("%d%d%d", &r, &c, &w);

		struct EdgeNode *edge = (struct EdgeNode *)malloc(sizeof(struct EdgeNode));
		edge->next_edge = graph->vertex_list[r].next_edge;
		edge->vex_index = c;
		edge->weight = w;
		graph->vertex_list[r].next_edge = edge;
	}

	return graph;
}

void free_graph(struct Graph *graph)
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

void print_graph(struct Graph *graph)
{
	int i;

	struct VertexNode *vertexnode = graph->vertex_list;
	for (i = 0; i < graph->scope; i++)
	{
		printf("%c:", vertexnode[i].value);
		_print_vertex_edge_node(vertexnode[i].next_edge);
	}
	printf("\n");
}

void print_degree(struct Graph *graph)
{
	int i;
	printf("%10s%10s\n", "Indegree", "Outdegree");
	for (i = 0; i < graph->scope; i++)
	{
		printf("%c:%5d%10d\n", graph->vertex_list[i].value, _get_vertex_indegree(graph, i), _get_vertex_outdegree(graph, i));
	}
	printf("\n");
}

/* deep-first search module */
void _dfs_traverse_graph(struct Graph *graph, int index, int *status)
{
	struct EdgeNode *edge;

	status[index] = 1;
	_print_vertex(graph->vertex_list[index].value, " ");

	for (edge = graph->vertex_list[index].next_edge; edge; edge = edge->next_edge)
	{
		if (status[edge->vex_index] == 0)
		{
			_dfs_traverse_graph(graph, edge->vex_index, status);
		}
	}
}
void dfs_traverse_graph(struct Graph *graph)
{
	int i;
	int *status = (int *)malloc(sizeof(int) * graph->scope);

	memset(status, 0, sizeof(int) * graph->scope);

	printf("The graph vertex include(dfs): ");
	for (i = 0; i < graph->scope; i++)
	{
		if (status[i] == 0)
		{
			_dfs_traverse_graph(graph, i, status);
		}
	}
	printf("\n");
}

/* breadth-first search module */
void _bfs_traverse_graph(struct Graph *graph, int index, int *status)
{
	struct EdgeNode *edge;
	struct LinkedQueue *queue = new_queue();

	status[index] = 1;
	_print_vertex(graph->vertex_list[index].value, " ");
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
				_print_vertex(graph->vertex_list[edge->vex_index].value, " ");
			}
		}
		pop_queue_elem(queue);
	}
	free_queue(queue);
}
void bfs_traverse_graph(struct Graph *graph)
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

void _free_edge(struct Graph *graph, int index)
{
	int i;

	for (i = 0; i < graph->scope; i++)
	{
		if (i == index) continue;

		struct EdgeNode *next_edge = graph->vertex_list[i].next_edge;
		if (next_edge && next_edge->vex_index == index)
		{
			graph->vertex_list[i].next_edge = next_edge->next_edge;
			free(next_edge);
			(graph->path_amount)--;
			return;
		}

		struct EdgeNode *edge = next_edge;
		next_edge = next_edge->next_edge;
		while (next_edge)
		{
			if (next_edge->vex_index == index)
			{
				edge->next_edge = next_edge->next_edge;
				free(next_edge);
				(graph->path_amount)--;
				return;
			}
			next_edge = next_edge->next_edge;
		}
	}
}
void _free_node(struct Graph *graph, int index)
{
	int i;

	while (graph->vertex_list[index].next_edge)
	{
		struct EdgeNode *fedge = graph->vertex_list[index].next_edge;
		graph->vertex_list[index].next_edge = graph->vertex_list[index].next_edge->next_edge;
		free(fedge);
		(graph->path_amount)--;
	}

	for (i = index + 1; i < graph->scope; i++)
	{
		graph->vertex_list[i - 1].value = graph->vertex_list[i].value;
		graph->vertex_list[i - 1].next_edge = graph->vertex_list[i].next_edge;
	}
	(graph->scope)--;

	for (i = 0; i < graph->scope; i++)
	{
		struct EdgeNode *edge = graph->vertex_list[i].next_edge;
		while (edge)
		{
			if (edge->vex_index > index)
			{
				(edge->vex_index)--;
			}
			edge = edge->next_edge;
		}
	}

	graph->vertex_list = (struct VertexNode *)realloc(graph->vertex_list, sizeof(struct VertexNode) * graph->scope);
}
void delete_vertex(struct Graph *graph, char element)
{
	int i;

	struct VertexNode *vertex_list = graph->vertex_list;
	for (i = 0; i < graph->scope; i++)
	{
		if (vertex_list[i].value == element)
		{
			// delete the element
			_free_edge(graph, i);
			_free_node(graph, i);
			printf("This element(%c) deleted.\n", element);
			break;
		}
	}
	if (i >= graph->scope)
	{
		// not find the element
		printf("This element(%c) is not find.\n", element);
	}

	dfs_traverse_graph(graph);
}

int is_connected_graph(struct Graph *graph)
{
	int i, j, k;
	
	int status = 1;
	int **edge_matrix = _get_edge_matrix(graph);

	for (i = 0; i < graph->scope; i++)
	{
		for (j = 0; j < graph->scope; j++)
		{
			if (!edge_matrix[i][j]) edge_matrix[i][j] = 1000000;
		}
	}

	for (k = 0; k < graph->scope; k++)
	{
		for (i = 0; i < graph->scope; i++)
		{
			for (j = 0; j < graph->scope; j++)
			{
				if (edge_matrix[i][j] > edge_matrix[i][k] + edge_matrix[k][j])
				{
					edge_matrix[i][j] = edge_matrix[i][k] + edge_matrix[k][j];
				}
			}
		}
	}
	
	for (i = 0; i < graph->scope; i++)
	{
		for (j = 0; j < graph->scope; j++)
		{
			if (!edge_matrix[i][j] && i != j)
			{
				status = 0;
			}
		}
	}

	_free_edge_matrix(edge_matrix, graph->scope);
	return status;
}

struct SequenceGraph *transform_graph_to_sequence(struct Graph *graph)
{
	struct SequenceGraph *sgraph = (struct SequenceGraph *)malloc(sizeof(struct SequenceGraph));
	sgraph->scope = graph->scope;
	sgraph->path_amount = graph->path_amount;
	sgraph->vertex = _get_vertex_list(graph);
	sgraph->edge   = _get_edge_matrix(graph);
	return sgraph;
}

void free_sequence_graph(struct SequenceGraph *graph)
{
	_free_edge_matrix(graph->edge, graph->scope);
	_free_vertex_list(graph->vertex);
	free(graph);
}

int _is_in_edge_list(int *edge_list, int list_len, int src, int dest, int weight)
{
	int i;
	for (i = 0; i < list_len * 3; i += 3)
	{
		if(edge_list[i] == src
		&& edge_list[i + 1] == dest
		&& edge_list[i + 2] == weight
		)
		{
			return 1;
		}
	}
	return 0;
}

struct Edge _find_min_edge(struct SequenceGraph *graph, int *visited, int *edge_list, int list_len)
{
	int i, j;
	struct Edge edge = {0, 0, 0};

	for (i = 0; i < graph->scope; i++)
	{
		if (visited[i] == 1)
		{
			for (j = 0; j < graph->scope; j++)
			{
				if (graph->edge[i][j] != 0 /* edge is exist */
				&& (edge.weight == 0 || graph->edge[i][j] < edge.weight) /* find min */
				&& visited[j] != 1 /* node is not visited */
				&& !_is_in_edge_list(edge_list, list_len, i, j, graph->edge[i][j])
				)
				{
					edge.src = i;
					edge.dest = j;
					edge.weight = graph->edge[i][j];
				}
			}
		}
	}

	return edge;
}

void _record_edge_in_list(struct Edge edge, int *edge_list, int index /* edge index*/)
{
	int i = 3 * index;
	edge_list[i] = edge.src;
	edge_list[i + 1] = edge.dest;
	edge_list[i + 2] = edge.weight;
}

struct SequenceGraph *_new_graph_base(int scope, int path_amount, GraphVertex *vertex_list, int *edge_list)
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

struct SequenceGraph *prim(struct SequenceGraph *graph, int index)
{
	printf("Prim: ");

	int *visited = (int *)malloc(sizeof(int) * graph->scope);
	memset(visited, 0, sizeof(int) * graph->scope);
	// 最小生成树边的个数不会超过节点数
	int *edge_list = (int *)malloc(sizeof(int) * 3 * (graph->scope - 1));
	
	int i;
	for (i = 0; i < graph->scope; i++)
	{
		printf("%d ", index);
		visited[index] = 1;
		struct Edge min_edge = _find_min_edge(graph, visited, edge_list, i);
		_record_edge_in_list(min_edge, edge_list, i);
		index = min_edge.dest;
	}

	struct SequenceGraph *g = _new_graph_base(graph->scope, graph->scope - 1, graph->vertex, edge_list);
	free(visited);
	free(edge_list);

	printf("\n");
	return g;
}
