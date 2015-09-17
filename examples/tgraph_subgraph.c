#include "graph.h"
#include "graph_topological_properties.h"
#include <assert.h>

int main()
{
	graph_t a;
	vector_int v1;
	vector_int_init_value_end(&v1, -1, 4,5, 2,3, 2,1, 3,1, 4,2, 4,3, 5,4, 6,4, 5,6, -1);
	new_graph(&a, &v1, 0, GRAPH_DIRECTED);

	graph_t b;
	graph_neighbor_subgraph(&a, &b, &v1, 5, GRAPH_ALL_NOBIDIR);
	print_graph_vectors(&b, stdout);
	printf("neis:\n");
	print_vector_int(&v1,stdout);
	graph_destroy(&b);

	graph_neighbors(&a, &v1, 5, GRAPH_ALL_NOBIDIR);
	printf("neis:\n");
	print_vector_int(&v1,stdout);

	graph_subgraph(&a, &b, graph_vss_vector(&v1));
	print_graph_vectors(&b, stdout);

	vector_int_destroy(&v1);
	graph_destroy(&a);
	return 0;
}
