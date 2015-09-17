#include "graph_centrality.h"
#include <assert.h>

int main()
{
	graph_t a;
	vector_int v1;

	/****************************/
	vector_int_init_value_end(&v1, -1, 4,5, 5,6, 6,4, 5,4, 6,5, 4,6, 1,0, 0,3, -1);
	new_graph(&a, &v1, 0, GRAPH_DIRECTED);
	assert(2 == graph_v_triangle_circle(&a, 4));
	assert(2 == graph_v_triangle_middleman(&a, 4));
	assert(2 == graph_v_triangle_in(&a, 4));
	assert(2 == graph_v_triangle_out(&a, 4));
	vector_int_destroy(&v1);
	graph_destroy(&a);


	/****************************/
	vector_int_init_value_end(&v1, -1, 4,5, 5,6, 6,4, 5,4, 6,5, 1,0, 0,3, -1);
	new_graph(&a, &v1, 0, GRAPH_DIRECTED);
	assert(1 == graph_v_triangle_circle(&a, 4));
	assert(1 == graph_v_triangle_middleman(&a, 4));
	assert(2 == graph_v_triangle_in(&a, 4));
	assert(0 == graph_v_triangle_out(&a, 4));

	assert(1 == graph_v_triangle_circle(&a, 6));
	assert(1 == graph_v_triangle_middleman(&a, 6));
	assert(0 == graph_v_triangle_in(&a, 6));
	assert(2 == graph_v_triangle_out(&a, 6));

	assert(1 == graph_v_triangle_circle(&a, 5));
	assert(1 == graph_v_triangle_middleman(&a, 5));
	assert(1 == graph_v_triangle_in(&a, 5));
	assert(1 == graph_v_triangle_out(&a, 5));
	vector_int_destroy(&v1);
	graph_destroy(&a);

	/****************************/
	vector_int_init_value_end(&v1, -1, 4,5, -1);
	new_graph(&a, &v1, 0, GRAPH_DIRECTED);
	assert(0 == graph_v_triangle_circle(&a, 4));
	assert(0 == graph_v_triangle_middleman(&a, 4));
	assert(0 == graph_v_triangle_in(&a, 4));
	assert(0 == graph_v_triangle_out(&a, 4));
	vector_int_destroy(&v1);
	graph_destroy(&a);

	/****************************/
	new_graph_empty(&a, 5, GRAPH_DIRECTED);
	assert(0 == graph_v_triangle_circle(&a, 4));
	assert(0 == graph_v_triangle_middleman(&a, 4));
	assert(0 == graph_v_triangle_in(&a, 4));
	assert(0 == graph_v_triangle_out(&a, 4));
	vector_int_destroy(&v1);
	graph_destroy(&a);

	printf("\ntgraph_trangle:[OK]\n");
	return 0;
}
