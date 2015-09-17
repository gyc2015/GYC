#include "graph_topological_properties.h"
#include "graph_centrality.h"
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
	vector_int_init_value(&v1, 14, 4,5, 2,3, 7,1, 6,1, 4,8, 4,7, 0,0);
	graph_add_edges(&a, &v1, 0);
	assert(10 == graph_vertices_count(&a));
	assert(7 == graph_edges_count(&a));
	assert(1 == graph_is_directed(&a));
	print_vector_int(&(a.from),stdout);
	print_vector_int(&(a.to),stdout);
	print_vector_int(&(a.oi),stdout);
	print_vector_int(&(a.ii),stdout);
	print_vector_int(&(a.os),stdout);
	print_vector_int(&(a.is),stdout);
	printf("ec=%d\t",graph_edges_count(&a));
	printf("vc=%d\n",graph_vertices_count(&a));
	printf("reciprocal = %f \n", graph_reciprocal(&a));


	vector_int deglist;
	vector_int_init(&deglist, 0);
	vector_double degdist;
	vector_double_init(&degdist, 0);
	graph_degree(&a, &deglist, graph_vss_all(), GRAPH_OUT, GRAPH_NO_LOOPS);
	printf("degree out no loops:"); print_vector_int(&(deglist),stdout);
	graph_degree_distribution(&a, &degdist, graph_vss_all(), GRAPH_OUT, GRAPH_NO_LOOPS);
	printf("degree distribution out no loops:"); print_vector_double(&(degdist),stdout);
	int min, max, sum;
	double ave;
	graph_degree_minmax_avesum(&a, &min, &max, &ave, &sum, GRAPH_OUT, GRAPH_NO_LOOPS);
	assert(0 == min);assert(3 == max);assert(6 == sum);assert(0.6 == ave);

	graph_degree(&a, &deglist, graph_vss_all(), GRAPH_OUT, GRAPH_LOOPS);
	printf("degree out loops:"); print_vector_int(&(deglist),stdout);
	graph_degree_distribution(&a, &degdist, graph_vss_all(), GRAPH_OUT, GRAPH_LOOPS);
	printf("degree distribution out loops:"); print_vector_double(&(degdist),stdout);

	graph_degree(&a, &deglist, graph_vss_all(), GRAPH_IN, GRAPH_NO_LOOPS);
	printf("degree in no loops:"); print_vector_int(&(deglist),stdout);
	graph_degree_distribution(&a, &degdist, graph_vss_all(), GRAPH_IN, GRAPH_NO_LOOPS);
	printf("degree distribution in no loops:"); print_vector_double(&(degdist),stdout);


	graph_degree(&a, &deglist, graph_vss_all(), GRAPH_IN, GRAPH_LOOPS);
	printf("degree in loops:"); print_vector_int(&(deglist),stdout);
	graph_degree_distribution(&a, &degdist, graph_vss_all(), GRAPH_IN, GRAPH_LOOPS);
	printf("degree distribution in loops:"); print_vector_double(&(degdist),stdout);


	graph_degree(&a, &deglist, graph_vss_all(), GRAPH_ALL, GRAPH_NO_LOOPS);
	printf("degree all no loops:"); print_vector_int(&(deglist),stdout);
	graph_degree_distribution(&a, &degdist, graph_vss_all(), GRAPH_ALL, GRAPH_LOOPS);
	printf("degree distribution all loops:"); print_vector_double(&(degdist),stdout);

	graph_degree(&a, &deglist, graph_vss_all(), GRAPH_ALL, GRAPH_LOOPS);
	printf("degree all loops:"); print_vector_int(&(deglist),stdout);
	graph_degree_distribution(&a, &degdist, graph_vss_all(), GRAPH_ALL, GRAPH_LOOPS);
	printf("degree distribution all loops:"); print_vector_double(&(degdist),stdout);



	vector_int vlist;
	vector_int_init_value(&vlist, 7, 1,2,3,5,6,7,8);
	print_vector_int(&(deglist),stdout);
	graph_degree(&a, &deglist, graph_vss_vector(&vlist), GRAPH_OUT, GRAPH_NO_LOOPS);
	printf("degree vector out no loops:"); print_vector_int(&(deglist),stdout);
	graph_degree_distribution(&a, &degdist, graph_vss_vector(&vlist), GRAPH_OUT, GRAPH_NO_LOOPS);
	printf("degree distribution vector out no loops:"); print_vector_double(&(degdist),stdout);

	vector_int eids;
	vector_int_init(&eids, 0);
	graph_incident(&a, &eids, 4, GRAPH_OUT);
	printf("graph_incident 4:"); print_vector_int(&(eids),stdout);
	graph_neighbors(&a, &eids, 4, GRAPH_OUT);
	printf("graph_neighbors 4:"); print_vector_int(&(eids),stdout);

	graph_destroy(&a);
	vector_int_destroy(&v1);
	vector_int_init_value_end(&v1, -1, 0,1, 1,0, 1,3, 3,1, 2,3, 2,4, 4,5, 5,1 ,-1);
	new_graph(&a, &v1, 0, GRAPH_DIRECTED);
	print_vector_int(&(a.from),stdout);
	print_vector_int(&(a.to),stdout);
	print_vector_int(&(a.oi),stdout);
	print_vector_int(&(a.ii),stdout);
	print_vector_int(&(a.os),stdout);
	print_vector_int(&(a.is),stdout);
	printf("ec=%d\t",graph_edges_count(&a));
	printf("vc=%d\n",graph_vertices_count(&a));
	printf("reciprocal = %f \n", graph_reciprocal(&a));
	graph_degree(&a, &v1, graph_vss_all(), GRAPH_BIDIR, GRAPH_NO_LOOPS);
	print_vector_int(&v1, stdout);
	assert(VECTOR(v1)[0] == 1);
	assert(VECTOR(v1)[1] == 2);
	graph_degree(&a, &v1, graph_vss_all(), GRAPH_ALL_NOBIDIR, GRAPH_NO_LOOPS);
	printf("GRAPH_ALL_NOBIDIR\n");
	print_vector_int(&v1, stdout);
	assert(VECTOR(v1)[1] == 3);
	assert(VECTOR(v1)[5] == 2);
	printf("GRAPH_OUT\n");
	graph_degree(&a, &v1, graph_vss_all(), GRAPH_OUT, GRAPH_NO_LOOPS);
	print_vector_int(&v1, stdout);
	printf("GRAPH_IN\n");
	graph_degree(&a, &v1, graph_vss_all(), GRAPH_IN, GRAPH_NO_LOOPS);
	print_vector_int(&v1, stdout);
	vector_double inout, outin;
	vector_double_init(&inout, 0);
	vector_double_init(&outin, 0);
	graph_relation_in_out_degree(&a, &inout, &outin, graph_vss_all(), GRAPH_NO_LOOPS);
	print_vector_double(&inout, stdout);
	print_vector_double(&outin, stdout);

	return 0;
}
