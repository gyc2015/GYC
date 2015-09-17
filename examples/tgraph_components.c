#include "graph_components.h"
#include <assert.h>

int main()
{
	printf("weakly connected components\n");
	vector_int v1;
	vector_int_init_value_end(&v1, -1, 0,1, 0,13, 1,2, 1,3, 2,4, 4,5, 6,7, 6,8, 7,10, 8,9, 11,12, 11,13, -1,7, -1);
	graph_t a;
	new_graph(&a, &v1, 0, GRAPH_DIRECTED);
	print_vector_int(&(a.from),stdout);
	print_vector_int(&(a.to),stdout);
	print_vector_int(&(a.oi),stdout);
	print_vector_int(&(a.ii),stdout);
	print_vector_int(&(a.os),stdout);
	print_vector_int(&(a.is),stdout);
	printf("ec=%d\t",graph_edges_count(&a));
	printf("vc=%d\n",graph_vertices_count(&a));

	int vc = graph_vertices_count(&a);
	vector_int mem;
	vector_int_init(&mem, vc);
	vector_int_fill(&mem, -1);
	VECTOR(mem)[4] = -2;
	vector_int cs;
	vector_int_init(&cs, 0);
	int cc;
	graph_clusters_weak(&a,&mem,&cs,&cc);
	printf("membership:");
	print_vector_int(&mem,stdout);
	printf("component size:");
	print_vector_int(&cs,stdout);
	printf("cc=%d\n",cc);
	assert(3 == cc);

	vector_int_destroy(&cs);
	vector_int_destroy(&mem);
	vector_int_destroy(&v1);
	graph_destroy(&a);

	printf("strongly connected components\n");
	vector_int_init_value_end(&v1, -1, 0,1, 2,0, 1,2, 1,3, 2,4, 4,5, 6,7, 6,8, 7,10, 8,9, 11,12, 11,13, 12,11, -1);
	new_graph(&a, &v1, 0, GRAPH_DIRECTED);
	print_vector_int(&(a.from),stdout);
	print_vector_int(&(a.to),stdout);
	print_vector_int(&(a.oi),stdout);
	print_vector_int(&(a.ii),stdout);
	print_vector_int(&(a.os),stdout);
	print_vector_int(&(a.is),stdout);
	printf("ec=%d\t",graph_edges_count(&a));
	printf("vc=%d\n",graph_vertices_count(&a));

	vector_int_init(&mem, vc);
	vector_int_fill(&mem, -1);
	VECTOR(mem)[4] = -2;
	vector_int_init(&cs, 0);
	graph_clusters_strong(&a,&mem,&cs,&cc);
	printf("membership:");
	print_vector_int(&mem,stdout);
	printf("component size:");
	print_vector_int(&cs,stdout);
	printf("cc=%d\n",cc);

	vector_int_destroy(&cs);
	vector_int_destroy(&mem);
	vector_int_destroy(&v1);
	graph_destroy(&a);


	return 0;
}
