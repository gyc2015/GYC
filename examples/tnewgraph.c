#include "graph.h"
#include <stdio.h>
#include <assert.h>

int main()
{
	graph_t a;
	new_graph_empty(&a, 1, 1);
	assert(1 == graph_vertices_count(&a));
	assert(0 == graph_edges_count(&a));
	assert(1 == graph_is_directed(&a));

	graph_add_vertices(&a, 9, 0);
	assert(10 == graph_vertices_count(&a));
	assert(0 == graph_edges_count(&a));
	assert(1 == graph_is_directed(&a));

	vector_int v1;
	vector_int_init_value(&v1, 8, 4,5,2,3,7,1,6,1);
	graph_add_edges(&a, &v1, 0);
	assert(10 == graph_vertices_count(&a));
	assert(4 == graph_edges_count(&a));
	assert(1 == graph_is_directed(&a));
	print_graph_ct(&a, GRAPH_OUT, stdout);
	print_vector_int(&(a.from),stdout);
	print_vector_int(&(a.to),stdout);
	print_vector_int(&(a.oi),stdout);
	print_vector_int(&(a.ii),stdout);
	print_vector_int(&(a.os),stdout);
	print_vector_int(&(a.is),stdout);
	printf("ec=%d\n",graph_edges_count(&a));
	printf("vc=%d\n",graph_vertices_count(&a));

	printf("delete edge 1,3\n");
	vector_int eids;
	vector_int_init_value(&eids, 2, 1,3);
	graph_del_edges(&a, graph_ess_vector(&eids));
	assert(10 == graph_vertices_count(&a));
	assert(2 == graph_edges_count(&a));
	print_graph_ct(&a, GRAPH_OUT, stdout);
	vector_int_destroy(&eids);

	graph_destroy(&a);

	new_graph(&a, &v1, 10, 1);
	print_vector_int(&(a.from),stdout);
	print_vector_int(&(a.to),stdout);
	print_vector_int(&(a.oi),stdout);
	print_vector_int(&(a.ii),stdout);
	print_vector_int(&(a.os),stdout);
	print_vector_int(&(a.is),stdout);
	vector_int_destroy(&v1);

	printf(">>>reverse\n");
	graph_reverse(&a);
	print_vector_int(&(a.from),stdout);
	print_vector_int(&(a.to),stdout);
	print_vector_int(&(a.oi),stdout);
	print_vector_int(&(a.ii),stdout);
	print_vector_int(&(a.os),stdout);
	print_vector_int(&(a.is),stdout);

	return 0;
}
