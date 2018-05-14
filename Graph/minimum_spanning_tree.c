#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "..\stack\sequence_stack.h"
#include "sequence_graph.h"

struct Edge 
{
	int src;
	int dest;
	int weight;
};

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

	struct SequenceGraph *g = new_graph_base(graph->scope, graph->scope - 1, graph->vertex, edge_list);
	free(visited);
	free(edge_list);

	printf("\n");
	return g;
}


void _record_edge(struct Edge *edge, int src, int dest, int weight)
{
	edge->src = src;
	edge->dest = dest;
	edge->weight = weight;
}

int _compare_edge(const void *edge1, const void *edge2)
{
	return (((struct Edge *)edge1)->weight - ((struct Edge *)edge2)->weight);
}

void __union(int *node_list, int node_len, int x, int y)
{
	int i;
	for (i = 0; i < node_len; i++)
	{
		if (node_list[i] == node_list[y])
		{
			node_list[i] = node_list[x];
		}
	}
}

void _union_node_list(int *node_list, int node_len, int x, int y)
{
	if (node_list[x] < node_list[y])
	{
		__union(node_list, node_len, x, y);
	}
	else
	{
		__union(node_list, node_len, y, x);
	}
}

struct SequenceGraph *kruscal(struct SequenceGraph *graph, int index)
{
	// printf("Kruscal: ");
	int edge_list_len = graph->path_amount / 2;

	// Get edge list
	struct Edge *edge_list = (struct Edge *)malloc(sizeof(struct Edge) * edge_list_len);
	int list_index = 0, i , j;
	for (i = 0; i < graph->scope; i++)
	{
		for (j = 0; j < i; j++)
		{
			if (graph->edge[i][j] != 0)
			{
				_record_edge(&edge_list[list_index++], i, j, graph->edge[i][j]);
			}
		}
	}

	// Sort edge list
	qsort(edge_list, edge_list_len, sizeof(struct Edge), _compare_edge);

	// Select edge
	int *node_list = (int *)malloc(sizeof(int) * graph->scope);
	for (i = 0; i < graph->scope; i++)
	{
		node_list[i] = i;
	} // respresent their father

	int *elist = (int *)malloc(sizeof(int) * 3 * (graph->scope - 1));
	int eindex = 0;

	for (i = 0; i < edge_list_len; i++)
	{
		if (node_list[edge_list[i].src] != node_list[edge_list[i].dest])
		{
			_union_node_list(node_list, graph->scope, edge_list[i].src, edge_list[i].dest);
			_record_edge_in_list(edge_list[i], elist, eindex++);
		}
	}

	// for (i = 0; i < eindex * 3; i +=3)
	// {
	// 	printf("%d: %d,%d,%d\n", i, elist[i], elist[i + 1], elist[i + 2]);
	// }

	// Create graph
	struct SequenceGraph *g = new_graph_base(graph->scope, eindex, graph->vertex, elist);
	free(edge_list);
	free(node_list);
	free(elist);
	return g;
}

struct SequenceGraph *create_graph()
{
	GraphVertex vertex_list[] = {0, 1, 2, 3, 4, 5};
	int scope = sizeof(vertex_list) / sizeof(int);

	int edge_list[] = {
		0,1,6, 1,0,6,
		0,2,1, 2,0,1,
		0,3,5, 3,0,5,
		1,2,5, 2,1,5,
		1,4,3, 4,1,3,
		2,3,7, 3,2,7,
		2,4,5, 4,2,5,
		2,5,4, 5,2,4,
		3,5,2, 5,3,2,
		4,5,6, 5,4,6
	};
	int path_amount = sizeof(edge_list) / sizeof(int) / 3;

	return new_graph_base(scope, path_amount, vertex_list, edge_list);
}

// Test
int main()
{	
	struct SequenceGraph *graph = create_graph();
	print_graph(graph);

	struct SequenceGraph *graph2 = prim(graph, 0);
	print_graph(graph2);

	struct SequenceGraph *graph3 = kruscal(graph, 0);
	print_graph(graph3);

	free_graph(graph);
	free_graph(graph2);
	free_graph(graph3);
	return 0;
}