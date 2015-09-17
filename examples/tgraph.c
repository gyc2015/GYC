#include "graph.h"
#include "graph_components.h"
#include <assert.h>

int main()
{
	graph_t a;
	vector_int v1;
	vector_int_init_value_end(&v1, -1, 1,5, 2,5, 1,2, 1,2, 0,3, 0,2, 3,4, 3,6, 4,6, 6,4, -1,7, -1);
	new_graph(&a, &v1, 0, GRAPH_DIRECTED);
	print_vector_int(&(a.from),stdout);
	print_vector_int(&(a.to),stdout);
	print_vector_int(&(a.oi),stdout);
	print_vector_int(&(a.ii),stdout);
	print_vector_int(&(a.os),stdout);
	print_vector_int(&(a.is),stdout);
	printf("ec=%d\t",graph_edges_count(&a));
	printf("vc=%d\n",graph_vertices_count(&a));
	assert(graph_edge_contains(&a, 1,5,GRAPH_OUT));
	assert(graph_edge_contains(&a, 5,1,GRAPH_IN));
	assert(graph_edge_contains(&a, 5,1,GRAPH_ALL));
	assert(!graph_edge_contains(&a, 2,1,GRAPH_OUT));
	assert(!graph_edge_contains(&a, 7,1,GRAPH_OUT));

	printf("remove multi-edges\n");
	graph_remove_multi_edges(&a);
	print_vector_int(&(a.from),stdout);
	print_vector_int(&(a.to),stdout);
	print_vector_int(&(a.oi),stdout);
	print_vector_int(&(a.ii),stdout);
	print_vector_int(&(a.os),stdout);
	print_vector_int(&(a.is),stdout);
	printf("ec=%d\t",graph_edges_count(&a));
	printf("vc=%d\n",graph_vertices_count(&a));
	vector_int_destroy(&v1);
	graph_destroy(&a);

	printf(">>>combine vertices\n");
	vector_int_init_value_end(&v1, -1, 0,1, 2,0, 1,2, 1,3, 2,4, 4,5, 6,7, 6,8, 7,10, 8,9, 11,12, 11,13, 12,11, -1);
	new_graph(&a, &v1, 0, GRAPH_DIRECTED);
	int vc = graph_vertices_count(&a);
	print_vector_int(&(a.from),stdout);
	print_vector_int(&(a.to),stdout);
	print_vector_int(&(a.oi),stdout);
	print_vector_int(&(a.ii),stdout);
	print_vector_int(&(a.os),stdout);
	print_vector_int(&(a.is),stdout);
	printf("ec=%d\t",graph_edges_count(&a));
	printf("vc=%d\n",graph_vertices_count(&a));
	vector_int mem;
	vector_int_init(&mem, vc);
	vector_int_fill(&mem, -1);
	VECTOR(mem)[4] = -2;
	vector_int cs;
	vector_int_init(&cs, 0);
	int cc;
	graph_clusters_strong(&a,&mem,&cs,&cc);
	printf("mem:");
	print_vector_int(&mem, stdout);
	printf("<<<combine vertices\n");
	graph_t b;
	graph_combine_vertices(&a, &mem, &b);
	print_vector_int(&(b.from),stdout);
	print_vector_int(&(b.to),stdout);
	print_vector_int(&(b.oi),stdout);
	print_vector_int(&(b.ii),stdout);
	print_vector_int(&(b.os),stdout);
	print_vector_int(&(b.is),stdout);
	printf("ec=%d\t",graph_edges_count(&b));
	printf("vc=%d\n",graph_vertices_count(&b));

	vector_int_destroy(&mem);
	vector_int_destroy(&cs);
	graph_destroy(&b);
	vector_int_destroy(&v1);
	graph_destroy(&a);
	return 0;
}
