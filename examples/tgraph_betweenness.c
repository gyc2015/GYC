#include "graph_topological_properties.h"
#include "graph_centrality.h"
#include <assert.h>

int main()
{
	graph_t a;
	new_graph_empty(&a, 1, 1);
	graph_add_vertices(&a, 8, 0);
	vector_int v1;
	vector_int_init_value(&v1, 20, 1,5, 2,5, 3,5, 4,5, 5,6, 5,7, 5,8, 6,0, 7,0, 8,0);
	graph_add_edges(&a, &v1, 0);
	print_vector_int(&(a.from),stdout);
	print_vector_int(&(a.to),stdout);
	print_vector_int(&(a.oi),stdout);
	print_vector_int(&(a.ii),stdout);
	print_vector_int(&(a.os),stdout);
	print_vector_int(&(a.is),stdout);
	printf("ec=%d\t",graph_edges_count(&a));
	printf("vc=%d\n",graph_vertices_count(&a));


	vector_double res;
	vector_double_init(&res, 0);
	graph_betweenness(&a, &res, graph_vss_all(), GRAPH_UNDIRECTED);
	printf("betweenness directed:"); print_vector_double(&(res),stdout);

	graph_destroy(&a);
	return 0;
}
