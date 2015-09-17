#include "graph_fastcommunity.h"

int main()
{
	graph_t a;
	new_graph_empty(&a, 1, 1);
	graph_add_vertices(&a, 8, 0);
	vector_int v1;
	vector_int_init_value_end(&v1, -1, 1,5, 2,5, 1,2, 2,1, 0,3, 0,2, 3,4, 3,6, 4,6, 6,4, -1,7, -1);
	graph_add_edges(&a, &v1, 0);
	print_vector_int(&(a.from),stdout);
	print_vector_int(&(a.to),stdout);
	print_vector_int(&(a.oi),stdout);
	print_vector_int(&(a.ii),stdout);
	print_vector_int(&(a.os),stdout);
	print_vector_int(&(a.is),stdout);
	printf("ec=%d\t",graph_edges_count(&a));
	printf("vc=%d\n",graph_vertices_count(&a));

	int ncom = 0;
	double modularity = graph_community_fastgreedy(&a, &v1, &ncom);
	printf("modularity = %f, ncom = %d\n",modularity,ncom);
	print_vector_int(&v1, stdout);

	vector_cedge c;
	vector_cvertex v;
	graph_build_communities(&a, &v1, &c, &v);
	print_vector_cedge(&c,stdout);
	print_vector_cvertex(&v,stdout);

	vector_cedge_destroy(&c);
	vector_cvertex_destroy_all(&v);
	vector_int_destroy(&v1);
	graph_destroy(&a);
	return 0;
}
