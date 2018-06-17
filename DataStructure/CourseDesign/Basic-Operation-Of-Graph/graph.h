#ifndef __GRAPH__
#define __GRAPH__

typedef char GraphVertex;

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

struct Graph
{
	struct VertexNode *vertex_list;
	int scope;
	int path_amount;
};

struct SequenceGraph
{
	GraphVertex *vertex;
	int **edge;
	int scope;
	int path_amount;
};

struct Graph *new_graph();

void free_graph(struct Graph *graph);

void print_graph(struct Graph *graph);
void print_degree(struct Graph *graph);

void dfs_traverse_graph(struct Graph *graph);
void bfs_traverse_graph(struct Graph *graph);

void delete_vertex(struct Graph *graph, char element);

int is_connected_graph(struct Graph *graph);

struct SequenceGraph *transform_graph_to_sequence(struct Graph *graph);
void free_sequence_graph(struct SequenceGraph *graph);

struct SequenceGraph *prim(struct SequenceGraph *graph, int index);

#endif