#include "graph.h"
#include <stdio.h>

int main()
{
	// 3 3
	// 1 2 3
	// 0 1 2
	// 1 2 3
	// 2 0 1
	struct Graph *graph = new_graph();
	
	print_graph(graph);

	print_degree(graph);

	dfs_traverse_graph(graph);
	bfs_traverse_graph(graph);
	printf("\n");

	// delete_vertex(graph, 'a');
	printf("%s\n", is_connected_graph(graph) ? "Yes" : "No");

	struct SequenceGraph *sgraph = transform_graph_to_sequence(graph);
	struct SequenceGraph *sg = prim(sgraph, 0);
	// print

	free_sequence_graph(sg);
	free_sequence_graph(sgraph);
	free_graph(graph);
	return 0;
}