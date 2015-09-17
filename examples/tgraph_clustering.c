#include "graph_centrality.h"
#include "graph_topological_properties.h"
#include "graph_iterator.h"
#include <assert.h>

int main()
{
	graph_t a;
	vector_int v1;
	vector_double v2;
	vector_double_init(&v2, 0);

	printf("/****************************/\n");
	vector_int_init_value_end(&v1, -1, 4,5, 5,6, 6,4, 5,4, 6,5, 4,6, 1,0, 0,3, -1);
	new_graph(&a, &v1, 0, GRAPH_DIRECTED);
	printf("%f\n",graph_v_clustering_circle(&a,4));
	printf("%f\n",graph_v_clustering_middleman(&a, 4));
	printf("%f\n",graph_v_clustering_in(&a, 4));
	printf("%f\n",graph_v_clustering_out(&a, 4));
	graph_clustering_directed(&a, &v2, graph_vss_all(), GRAPH_TRIANGLE_ALL);
	print_vector_double(&v2, stdout);
	printf("clustering coefficient = %f\n",graph_ave_clustering_directed(&a, graph_vss_all(), GRAPH_TRIANGLE_ALL));
	printf("clustering circle = %f\n",graph_ave_clustering_directed(&a, graph_vss_all(), GRAPH_TRIANGLE_CIRCLE));
	printf("clustering middleman = %f\n",graph_ave_clustering_directed(&a, graph_vss_all(), GRAPH_TRIANGLE_MIDDLEMAN));
	printf("clustering in = %f\n",graph_ave_clustering_directed(&a, graph_vss_all(), GRAPH_TRIANGLE_IN));
	printf("clustering out = %f\n",graph_ave_clustering_directed(&a, graph_vss_all(), GRAPH_TRIANGLE_OUT));
	vector_int_destroy(&v1);
	graph_destroy(&a);

	printf("/****************************/\n");
	vector_int_init_value_end(&v1, -1, 1,2, 2,3, 3,1, 1,4, 4,2, 5,6, 6,0, -1);
	new_graph(&a, &v1, 0, GRAPH_DIRECTED);
	printf("%f\n",graph_v_clustering_circle(&a, 4));
	printf("%f\n",graph_v_clustering_middleman(&a, 4));
	printf("%f\n",graph_v_clustering_in(&a, 4));
	printf("%f\n",graph_v_clustering_out(&a, 4));

	printf("%f\n",graph_v_clustering_circle(&a, 6));
	printf("%f\n",graph_v_clustering_middleman(&a, 6));
	printf("%f\n",graph_v_clustering_in(&a, 6));
	printf("%f\n",graph_v_clustering_out(&a, 6));

	printf("%f\n",graph_v_clustering_circle(&a, 5));
	printf("%f\n",graph_v_clustering_middleman(&a, 5));
	printf("%f\n",graph_v_clustering_in(&a, 5));
	printf("%f\n",graph_v_clustering_out(&a, 5));
	printf("clustering:");
	graph_clustering_directed(&a, &v2, graph_vss_all(), GRAPH_TRIANGLE_ALL);
	print_vector_double(&v2, stdout);
	graph_clustering_directed(&a, &v2, graph_vss_all(), GRAPH_TRIANGLE_CIRCLE);
	print_vector_double(&v2, stdout);
	graph_clustering_directed(&a, &v2, graph_vss_all(), GRAPH_TRIANGLE_MIDDLEMAN);
	print_vector_double(&v2, stdout);
	graph_clustering_directed(&a, &v2, graph_vss_all(), GRAPH_TRIANGLE_IN);
	print_vector_double(&v2, stdout);
	graph_clustering_directed(&a, &v2, graph_vss_all(), GRAPH_TRIANGLE_OUT);
	print_vector_double(&v2, stdout);
	printf("degree all nobidir:");
	graph_degree(&a, &v1, graph_vss_all(), GRAPH_ALL_NOBIDIR, GRAPH_NO_LOOPS);
	print_vector_int(&v1, stdout);
	printf("degree bidir:");
	graph_degree(&a, &v1, graph_vss_all(), GRAPH_BIDIR, GRAPH_NO_LOOPS);
	print_vector_int(&v1, stdout);
	printf("triangle:");
	graph_triangle(&a, &v1, graph_vss_all(), GRAPH_TRIANGLE_ALL);
	print_vector_int(&v1, stdout);
	printf("clustering coefficient = %f\n",graph_ave_clustering_directed(&a, graph_vss_all(), GRAPH_TRIANGLE_ALL));
	printf("clustering circle = %f\n",graph_ave_clustering_directed(&a, graph_vss_all(), GRAPH_TRIANGLE_CIRCLE));
	printf("clustering middleman = %f\n",graph_ave_clustering_directed(&a, graph_vss_all(), GRAPH_TRIANGLE_MIDDLEMAN));
	printf("clustering in = %f\n",graph_ave_clustering_directed(&a, graph_vss_all(), GRAPH_TRIANGLE_IN));
	printf("clustering out = %f\n",graph_ave_clustering_directed(&a, graph_vss_all(), GRAPH_TRIANGLE_OUT));

	vector_double cluss;
	vector_double_init(&cluss, 0);
	graph_ave_clustering_directed_degree(&a, &cluss, graph_vss_all(), GRAPH_ALL_NOBIDIR, GRAPH_TRIANGLE_ALL, GRAPH_NO_LOOPS);
	print_vector_double(&cluss, stdout);

	vector_int_destroy(&v1);
	graph_destroy(&a);

	printf("/****************************/\n");
	vector_int_init_value_end(&v1, -1, 4,5, -1);
	new_graph(&a, &v1, 0, GRAPH_DIRECTED);
	printf("%f\n",graph_v_clustering_circle(&a, 4));
	printf("%f\n",graph_v_clustering_middleman(&a, 4));
	printf("%f\n",graph_v_clustering_in(&a, 4));
	printf("%f\n",graph_v_clustering_out(&a, 4));
	graph_clustering_directed(&a, &v2, graph_vss_all(), GRAPH_TRIANGLE_ALL);
	print_vector_double(&v2, stdout);
	printf("clustering:%f\n",graph_ave_clustering_directed(&a, graph_vss_all(), GRAPH_TRIANGLE_ALL));
	vector_int_destroy(&v1);
	graph_destroy(&a);

	printf("/****************************/\n");
	new_graph_empty(&a, 5, GRAPH_DIRECTED);
	printf("%f\n",graph_v_clustering_circle(&a, 4));
	printf("%f\n",graph_v_clustering_middleman(&a, 4));
	printf("%f\n",graph_v_clustering_in(&a, 4));
	printf("%f\n",graph_v_clustering_out(&a, 4));
	printf("clustering:%f\n",graph_ave_clustering_directed(&a, graph_vss_all(), GRAPH_TRIANGLE_ALL));
	vector_int_destroy(&v1);
	graph_destroy(&a);

	vector_double_destroy(&v2);
	printf("\ntgraph_clustering:[OK]\n");
	return 0;
}
