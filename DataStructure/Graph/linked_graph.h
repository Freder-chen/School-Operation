#ifndef __LINKED_GRAPH_H__
#define __LINKED_GRAPH_H__

typedef int GraphVertex;

struct EdgeNode
{
	int weight;
	int vex_index;
	struct EdgeNode *next_edge;
};

struct VertexNode
{
	GraphVertex value;
	struct EdgeNode *next_edge;
};

struct LinkedGraph
{
	struct VertexNode *vertex_list;
	int scope;
	int path_amount;
};

struct LinkedGraph *new_graph();

void free_graph(struct LinkedGraph *graph);

void print_graph(struct LinkedGraph *graph);

/* deep-first search recursive module */
void dfs_traverse_graph(struct LinkedGraph *graph);

/* deep-first search non recersive module */
void dfs_traverse_graph2(struct LinkedGraph *graph);

/* breadth-first search module */
void bfs_traverse_graph(struct LinkedGraph *graph);

#endif