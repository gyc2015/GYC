#include "graph_topological_properties.h"
#include <assert.h>
#include <stdlib.h>

int graph_randomized_graph(graph_t *graph, int vc, int ec, int directed)
{
	new_graph_empty(graph, vc, directed);
	vector_int edges;
	vector_int_init(&edges, 2);

	graph_neimode_t mode = (directed) ? GRAPH_OUT : GRAPH_ALL;
	int v = 0, w = 0, i = 0;
	while(i < ec) {
		while (v == w || graph_edge_contains(graph, v, w, mode)) {
			v = random() % vc;
			w = random() % vc;
		}
		VECTOR(edges)[0] = v;
		VECTOR(edges)[1] = w;
		graph_add_edges(graph, &edges, 0);
		i++;
	}

	vector_int_destroy(&edges);
	return 0;
}
