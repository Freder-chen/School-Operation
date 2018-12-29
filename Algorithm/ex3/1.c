#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/* base func: construct graph and print graph*/
struct Graph *new_graph_base(int scope, int path_amount, int *vertex_list, int *edge_list) {
	int i, j;

	struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
	graph->scope = scope;
	graph->path_amount = path_amount;
	graph->vertex = (int *)malloc(sizeof(int) * graph->scope);
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

void print_graph(struct Graph *graph) {
	if (!graph) {
		printf("The graph is NULL, cannot print.\n");
		return;
	}
	
	int i, j;

	// print graph vertex
	printf("The graph vertex:\n");
	for (i = 0; i < graph->scope; i++)
	{
		printf("%d%s", graph->vertex[i], "\t");
	}
	printf("\n");

	// print graph edge
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

/* Prim and Kruskal */
int _is_in_edge_list(int *edge_list, int list_len, int src, int dest, int weight) {
	int i;
	for (i = 0; i < list_len * 3; i += 3) {
		if (edge_list[i] == src
		&& edge_list[i + 1] == dest
		&& edge_list[i + 2] == weight
		) {
			return 1;
		}
	}
	return 0;
}

struct Edge _find_min_edge(struct Graph *graph, int *visited, int *edge_list, int list_len) {
	int i, j;
	struct Edge edge = {0, 0, 0};

	for (i = 0; i < graph->scope; i++) {
		if (visited[i] == 1) {
			for (j = 0; j < graph->scope; j++) {
				if (graph->edge[i][j] != 0 /* edge is exist */
				&& (edge.weight == 0 || graph->edge[i][j] < edge.weight) /* find min */
				&& visited[j] != 1 /* node is not visited */
				&& !_is_in_edge_list(edge_list, list_len, i, j, graph->edge[i][j])
				) {
					edge.src = i;
					edge.dest = j;
					edge.weight = graph->edge[i][j];
				}
			}
		}
	}

	return edge;
}

void _record_edge_in_list(struct Edge edge, int *edge_list, int index /* edge index*/) {
	int i = 3 * index;
	edge_list[i] = edge.src;
	edge_list[i + 1] = edge.dest;
	edge_list[i + 2] = edge.weight;
}

struct Graph *prim(struct Graph *graph, int index) {
	printf("Prim: ");

	int *visited = (int *)malloc(sizeof(int) * graph->scope);
	memset(visited, 0, sizeof(int) * graph->scope);
	int *edge_list = (int *)malloc(sizeof(int) * 3 * (graph->scope - 1));

	int i;
	for (i = 0; i < graph->scope; i++) {
		printf("%d ", index);
		visited[index] = 1;
		struct Edge min_edge = _find_min_edge(graph, visited, edge_list, i);
		_record_edge_in_list(min_edge, edge_list, i);
		index = min_edge.dest;
	}

	struct Graph *g = new_graph_base(graph->scope, graph->scope - 1, graph->vertex, edge_list);
	free(visited);
	free(edge_list);

	printf("\n");
	return g;
}

void _record_edge(struct Edge *edge, int src, int dest, int weight) {
	edge->src = src;
	edge->dest = dest;
	edge->weight = weight;
}

int _compare_edge(const void *edge1, const void *edge2) {
	return (((struct Edge *)edge1)->weight - ((struct Edge *)edge2)->weight);
}

void __union(int *node_list, int node_len, int x, int y) {
	int i;
	for (i = 0; i < node_len; i++) {
		if (node_list[i] == node_list[y]) {
			node_list[i] = node_list[x];
		}
	}
}

void _union_node_list(int *node_list, int node_len, int x, int y) {
	if (node_list[x] < node_list[y]) {
		__union(node_list, node_len, x, y);
	} else {
		__union(node_list, node_len, y, x);
	}
}

struct Graph *kruskal(struct Graph *graph, int index) {
	printf("Kruscal: ");
	int edge_list_len = graph->path_amount / 2;
	
	// Get edge list
	struct Edge *edge_list = (struct Edge *)malloc(sizeof(struct Edge) * edge_list_len);
	int list_index = 0, i , j;
	for (i = 0; i < graph->scope; i++) {
		for (j = 0; j < i; j++) {
			if (graph->edge[i][j] != 0) {
				_record_edge(&edge_list[list_index++], i, j, graph->edge[i][j]);
			}
		}
	}

	// Sort edge list
	qsort(edge_list, edge_list_len, sizeof(struct Edge), _compare_edge);

	// Select edge
	int *node_list = (int *)malloc(sizeof(int) * graph->scope);
	for (i = 0; i < graph->scope; i++) {
		node_list[i] = i;
	} // respresent their father

	int *elist = (int *)malloc(sizeof(int) * 3 * (graph->scope - 1));
	int eindex = 0;

	for (i = 0; i < edge_list_len; i++) {
		if (node_list[edge_list[i].src] != node_list[edge_list[i].dest]) {
			_union_node_list(node_list, graph->scope, edge_list[i].src, edge_list[i].dest);
			_record_edge_in_list(edge_list[i], elist, eindex++);
		}
	}

	// Create graph
	struct Graph *g = new_graph_base(graph->scope, eindex, graph->vertex, elist);
	free(edge_list); free(node_list); free(elist);
	return g;
}

/* Test func */
struct Graph *create_graph1() {
	int vertex_list[] = {0, 1, 2, 3, 4, 5};
	int scope = sizeof(vertex_list) / sizeof(int);

	int edge_list[] = {
		0,1,3, 1,0,3,
		0,5,5, 5,0,5,
		0,4,6, 4,0,6,
		1,2,1, 2,1,1,
		1,5,4, 5,1,4,
		2,3,6, 3,2,6,
		2,5,4, 5,2,4,
		3,4,8, 4,3,8,
		3,5,5, 5,3,5,
		4,5,2, 5,4,2
	};
	int path_amount = sizeof(edge_list) / sizeof(int) / 3;
	return new_graph_base(scope, path_amount, vertex_list, edge_list);
}

struct Graph *create_graph2() {
	int vertex_list[] = {0, 1, 2, 3, 4};
	int scope = sizeof(vertex_list) / sizeof(int);

	int edge_list[] = {
		0,1,5, 1,0,5,
		0,2,7, 2,0,7,
		0,4,2, 4,0,2,
		1,3,6, 3,1,6,
		1,4,3, 4,1,3,
		2,3,4, 3,2,4,
		2,4,4, 4,2,4,
		3,4,5, 4,3,5
	};
	int path_amount = sizeof(edge_list) / sizeof(int) / 3;
	return new_graph_base(scope, path_amount, vertex_list, edge_list);
}

struct Graph *create_graph3() {
	int vertex_list[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
	int scope = sizeof(vertex_list) / sizeof(int);

	int edge_list[] = {
		0,1,3, 1,0,3,
		0,2,5, 2,0,5,
		0,3,4, 3,0,4,
		1,4,3, 4,1,3,
		1,5,6, 5,1,6,
		2,3,2, 3,2,2,
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
	struct Graph *graph = create_graph2();
	print_graph(graph);
	printf("\n");

	// struct Graph *graph2 = prim(graph, 0);
	// print_graph(graph2);
	// printf("\n");

	struct Graph *graph3 = kruskal(graph, 0);
	print_graph(graph3);
	printf("\n");

	free_graph(graph);
	// free_graph(graph2);
	free_graph(graph3);
	return 0;
}