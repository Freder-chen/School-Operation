/* 找图的强连通分量 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "..\stack\sequence_stack.h"
#include "sequence_graph.h"

int min(int a, int b)  
{  
    return (a < b ? a : b);  
}  

void tarjin(struct SequenceGraph *graph, int index, int *status, int *low, struct SequenceStack *stack)
{
	status[index] += 2; // 被访问过，且在栈中。
	low[index] = index; // 自己连通自己。
	push_elem(stack, index);

	int i;
	for (i = 0; i < graph->scope; i++)
	{
		if (graph->edge[index][i] == 1)
		{
			if (status[i] == 0)
			{
				tarjin(graph, i, status, low, stack);
				low[index] = min(low[index], low[i]);
			}
			if (status[i] == 2)
			{
				low[index] = min(low[index], low[i]);
			}
		}
	}
	if (i >= graph->scope)
	{
		pop_elem(stack);
		status[index] -= 1;
	}
}

void find_strongly_connected_components_of_graph(struct SequenceGraph *graph)
{
	int i, j;
	struct SequenceStack *stack = new_stack();
	int *status = (int *)malloc(sizeof(int) * graph->scope);
	int *low = (int *)malloc(sizeof(int) * graph->scope);
	memset(status, 0, sizeof(int) * graph->scope);

	tarjin(graph, 0, status, low, stack);

	memset(status, 0, sizeof(int) * graph->scope);
	for (i = 0; i < graph->scope; i++)
	{
		status[low[i]]++;
	}
	for (i = 0; i < graph->scope; i++)
	{
		if (status[i] > 0)
		{
			printf("[");
			for (j = 0; j < graph->scope; j++)
			{
				if (low[j] == i)
				{
					printf(" %d", graph->vertex[j]);
				}
			}
			printf(" ]\n");
		}
	}

	free_stack(stack);
	free(status);
	free(low);
}

int main()
{
	struct SequenceGraph *graph = new_graph();

	find_strongly_connected_components_of_graph(graph);

	free_graph(graph);
	return 0;
}