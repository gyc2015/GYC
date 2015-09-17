#include "graph.h"


int main()
{
	graph_t a;
	new_graph_empty(&a, 1, 1);
	graph_add_vertices(&a, 8, 0);
	vector_int v1;
	vector_int_init_value_end(&v1, -1, 8,5, 2,5, 3,5, 4,5, 5,6, 5,7, 5,8, 6,0, 7,0, 6,0,-1);
	graph_add_edges(&a, &v1, 0);
	print_vector_int(&(a.from),stdout);
	print_vector_int(&(a.to),stdout);
	print_vector_int(&(a.oi),stdout);
	print_vector_int(&(a.ii),stdout);
	print_vector_int(&(a.os),stdout);
	print_vector_int(&(a.is),stdout);
	printf("ec=%d\t",graph_edges_count(&a));
	printf("vc=%d\n",graph_vertices_count(&a));

	graph_eit_t edgeit;
	graph_eit_create(&a, graph_ess_all(GRAPH_EDGEORDER_ID), &edgeit); 
	int i, j;
	printf(">>>edges\n");
	for (GRAPH_EIT_RESET(edgeit); !GRAPH_EIT_END(edgeit); GRAPH_EIT_NEXT(edgeit)) {
		int eidx = GRAPH_EIT_GET(edgeit);
        /* Create the pairs themselves */
		graph_edge(&a, eidx, &i, &j); 
		printf("i=%d,j=%d\n",i,j);
	}
	graph_eit_destroy(&edgeit);

	graph_destroy(&a);
	return 0;

}
