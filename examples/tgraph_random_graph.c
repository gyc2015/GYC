#include "graph_random_graph.h"
#include "graph_topological_properties.h"
#include "vector_int.h"
#include <stdlib.h>
#include <time.h>
#include <assert.h>

int main()
{
	srandom(time(0));
	graph_t a;
	graph_randomized_graph(&a, 9334, 26841, GRAPH_DIRECTED);
	assert(9334 == graph_vertices_count(&a));
	assert(26841 == graph_edges_count(&a));
	assert(!graph_has_multiple(&a));

	printf("global efficiency = %f\n",graph_global_efficiency(&a, GRAPH_OUT));
	printf("local efficiency = %f\n",graph_local_efficiency(&a, GRAPH_OUT));
	printf("clustering coefficient = %f\n",graph_ave_clustering_directed(&a, graph_vss_all(), GRAPH_TRIANGLE_ALL));
	printf("clustering circle = %f\n",graph_ave_clustering_directed(&a, graph_vss_all(), GRAPH_TRIANGLE_CIRCLE));
	printf("clustering middleman = %f\n",graph_ave_clustering_directed(&a, graph_vss_all(), GRAPH_TRIANGLE_MIDDLEMAN));
	printf("clustering in = %f\n",graph_ave_clustering_directed(&a, graph_vss_all(), GRAPH_TRIANGLE_IN));
	printf("clustering out = %f\n",graph_ave_clustering_directed(&a, graph_vss_all(), GRAPH_TRIANGLE_OUT));


	return 0;
}
