#ifndef __SEQUENCE_GRAPH_H__ 
#define __SEQUENCE_GRAPH_H__

typedef int GraphVertex;

struct SequenceGraph
{
	GraphVertex *vertex;
	int **edge;
	int scope;
	int path_amount;
};

/* create a sequence graph by code */
struct SequenceGraph *new_graph_base(int scope, int path_amount, GraphVertex *vertex_list, int *edge_list);

/* create a sequence graph */
struct SequenceGraph *new_graph();

/* free a sequence graph */
void free_graph(struct SequenceGraph *graph);

/* print a sequence graph */
void print_graph_vertex(struct SequenceGraph *graph);
void print_graph_edge(struct SequenceGraph *graph);
void print_graph(struct SequenceGraph *graph);

/* deep-first search recursive module */
void dfs_traverse_graph(struct SequenceGraph *graph);

/* deep-first search non recersive module */
void dfs_traverse_graph2(struct SequenceGraph *graph);

/* breadth-first search module */
void bfs_traverse_graph(struct SequenceGraph *graph);

#endif