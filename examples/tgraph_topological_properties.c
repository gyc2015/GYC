#include "graph_topological_properties.h"
#include <assert.h>

int main()
{
	graph_t a;
	vector_int v1;
	vector_int_init_value(&v1, 14, 4,5, 2,3, 2,1, 3,1, 4,2, 4,3, 0,0);
	new_graph(&a, &v1, 0, GRAPH_DIRECTED);
	printf("global efficiency = %f\n",graph_global_efficiency(&a, GRAPH_OUT));
	printf("local efficiency = %f\n",graph_local_efficiency(&a, GRAPH_OUT));

	vector_int_destroy(&v1);
	graph_destroy(&a);
	return 0;
}
